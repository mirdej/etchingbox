//---------------------------------------------------------------------------------------------
//	Etchingbox Controller										ATMEGA88
//
//	Part of the EtchingBox Project
// 	see: http://wiki.sgmk-ssam.ch/index.php/EtchingBox
//
// 	2014 Michael Egger, Licensed under GNU GPLv3
//
//--------------------------------------------------------------------------------------------
// ==============================================================================
// includes
// ------------------------------------------------------------------------------
// AVR Libc (see http://www.nongnu.org/avr-libc/)
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/wdt.h>		// include watchdog timer support
#include <avr/interrupt.h>	// include interrupt support

// ==============================================================================
// Globals
// ------------------------------------------------------------------------------
volatile uint32_t	ticks;
volatile uint8_t	did_tick;

uint16_t			display_value;
uint8_t				display_buf[3];

uint8_t				ptc1,ptc2;
uint16_t			temp1,temp2;


uint16_t			millis;
uint8_t				countdown;
uint8_t				seconds;
uint8_t				ad_idx;


#define ADC_MUX_MASK			0x1F

#define DISP_PORTB_MASK 0x3F
#define DISP_PORTD_MASK 0x80
// Set Display Brightness 0-100
#define DISPLAY_BRIGHTNESS	85	
volatile uint16_t display_last_ticks;
volatile uint8_t display_idx;


#define led_toggle()	PORTD ^= (1 << 2)

// ==============================================================================
// Utility Functions
// ------------------------------------------------------------------------------

void ad_set_channel (unsigned char mux) {
 	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (mux & ADC_MUX_MASK);		// set channel
 }

void ad_start_conversion(void) {
 		ADCSRA |= (1 << ADIF);			// clear hardware "conversion complete" flag 
		ADCSRA |= (1 << ADSC);			// start conversion
}

uint16_t ad_Read10bit (void) {
 	return (ADCL | ADCH << 8);
 }

// ------------------------------------------------------------------------------
// Init
// ------------------------------------------------------------------------------
void init (void) {
	/* PORTB
		7/6	Cathodes 1/2
		5 	SCK
		4 	MISO
		3	MOSI
		2 	Heater 1 Control
		1 	Piezo
		0 	Heater 2 Control
	*/
	
	DDRB 	= (1 << 7) | (1 << 6) | (1 << 2) | (1 << 1) | (1 << 0);
	PORTB 	= 0;

	
	/* PORTC
		4-7 -> AD Channels
		1-3	-> Switches
		0	-> Exposure Control
	*/

	DDRC 	= (1 << 0);
	PORTC 	= (1 << 3) | (1 << 2) | (1 << 1);

	/* PORTD 
		7 		Cathode 3
		0-6		Anodes
	*/
	
	DDRD = 0xff;

	// timer 0	-> millisecond clock
	ticks 	= 0;
	TCCR0A 	= (1 << WGM01);									// CTC
	TCCR0B 	= (0 << CS02) | (1 << CS01) | (1 << CS00);		// 64 prescaler @ 8Mhz -> 125'000 Hz
	TIMSK0 	= (1 << OCIE0A);								// enable interrupt
	OCR0A	= 124;											// 1000 Hz -> Milliseconds

	// timer 1 -> 16bit pwm
/*	TCCR1A	= (1 << COM1A1);
	TCCR1B = (1 << CS10);
*/
	TCCR1A = (1 << COM1A1)  | (1 << WGM10);			// fast pwm 8bit, clear OC1A on compare match
	TCCR1B = (1 << CS10) 	| (1 << WGM12);			// start timer no prescaler -> 8Mhz / 256 -> 32 khz PWM

	// setup analog converter. 
	ADCSRA 	= (1 << ADEN) | (1 << ADPS2); 		// enable adc, prescaler 16
	ADMUX = (1 << REFS0);
	
	ad_idx = 4;
	ad_set_channel(4);
	
	ad_start_conversion();

	wdt_enable(WDTO_30MS);			// enable watchdog timer
	sei();
	
	ptc1 = 255;	ptc2 = 255;
}

// ------------------------------------------------------------------------------
// 7 Segment Display
// ------------------------------------------------------------------------------

unsigned char get_digit(unsigned char d, unsigned int n) {
    if (d == 0) return n % 10;
    n = n / 10;
    if (d == 1) return n % 10;
    n = n / 10;
    return n % 10;
}

unsigned char num_to_7seg (unsigned char n) {
	switch (n) {
		  // connectoions are  gcedfab
			 case 0: return 0b0111111; break;
			 case 1: return 0b0100001; break;
			 case 2: return 0b1011011; break;
			 case 3: return 0b1101011; break;
			 case 4: return 0b1100101; break;
			 case 5: return 0b1101110; break;
			 case 6: return 0b1111110; break;
			 case 7: return 0b0100011; break;
			 case 8: return 0b1111111; break;
			 case 9: return 0b1101111; break;
			 default: break;
		   }
		return 0;
}

void update_display_buffer(void) {
	
	unsigned char i;
	for (i = 0; i < 3; i++) {
			display_buf[i] = num_to_7seg(get_digit(i,display_value));
	}
	// dont display leading zero if we're not counting down
	if (!countdown) {display_buf[2] = 0;}
	
}

void refresh_display(void){

	display_idx++;
	display_idx %= 3;//103-DISPLAY_BRIGHTNESS;
	
	PORTB &= DISP_PORTB_MASK; 
	PORTD = display_buf[display_idx] & 0x7F;
	
	
	switch (display_idx) {
		case 0: 		PORTD |= (1 << 7); 	break;
		case 1: 		PORTB |= (1 << 7);  break;
		case 2: 		PORTB |= (1 << 6);  break;
		default: break;
	}
}

// ------------------------------------------------------------------------------
// Audio input

void check_ad(void) {
		
	if ((ADCSRA & (1 << ADSC))) return; // not finished with last conversion


	uint16_t temp;
	temp = ad_Read10bit();
	
	
	switch (ad_idx) {

		case 4:
			temp /= 4;
			ptc1 = temp;
			break;
			
		case 5:
			temp /= 4;
			ptc2 = temp;
			break;
	
		case 6:
			temp = (temp * 45) / 100 - 44;
			if (temp1) temp = ((3 * temp1) + temp) / 4;			//don't lowpass filter values if nothing measured yet
			temp1 = temp;
			break;

		case 7:
			temp = (temp * 45) / 100 - 44;
			if (temp2) temp = ((3 * temp2) + temp) / 4;
			temp2 = temp;
			break;
	}
	
	
	ad_idx++;
	if (ad_idx > 7) ad_idx = 4;
	ad_set_channel (ad_idx);
	ad_start_conversion();
	
				
}

// ------------------------------------------------------------------------------
// heater control

void heater_on(uint8_t n) {
	if (n == 1) PORTB |= (1 << 2);
	if (n == 2) PORTB |= (1 << 0);
}

void heater_off(uint8_t n) {
	if (n == 1) PORTB &= ~(1 << 2);
	if (n == 2) PORTB &= ~(1 << 0);	
}


// ------------------------------------------------------------------------------
// Count Milliseconds

ISR(TIMER0_COMPA_vect)
{
	ticks++;
	did_tick = 1;
	refresh_display();
}


// ==============================================================================
// main
// ------------------------------------------------------------------------------

int main (void) {


	init();
	update_display_buffer();

	while(1) {
		wdt_reset();
		check_ad();

		// check heating status
		if (temp1 < 46) heater_on(1);
		if (temp2 < 20) heater_on(2);

		if (temp1 > 48) heater_off(1);
		if (temp2 > 24) heater_off(2);
		
		// safety off if there's no liquid 
		if (ptc1 < 70) {heater_off(1);}
		if (ptc2 < 70) {heater_off(2);}
		
		if (did_tick) {			// happens every millisecond
			did_tick = 0;
			millis++;
			

			if (millis >= 999) {
				millis = 0;
				if (countdown) {
					countdown--;
					display_value = countdown;
				} else {
					seconds++;
					if (seconds < 4) display_value = temp1;
					else if (seconds < 8) display_value = temp2;
					else seconds = 0;
				}		
			}
						
			update_display_buffer();

		}

	}
	return 0;
}	