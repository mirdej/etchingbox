//========================================================================================
//----------------------------------------------------------------------------------------
//
//	ÄtzBox Controller Firmware		
//						
//		Target MCU: Arduino MKR Wifi 1010
//		Copyright:	2018 Michael Egger, me@anyma.ch
//		License: 	This is FREE software (as in free speech, not necessarily free beer)
//					published under gnu GPL v.3
//
//----------------------------------------------------------------------------------------

#include <SPI.h>
#include <WiFiNINA.h>
#include <U8g2lib.h>
#include <Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Settings.h" 
#include "Logo.h" 



//-------------------------------- Pins
#define PIN_RELAY_HEAT 		4
#define PIN_RELAY_HEAT_2 	5
#define PIN_UV 				3
#define	PIN_LIGHT			1
#define	PIN_BUBBLES			2
#define PIN_SWITCH			7
#define	PIN_PIEZO			0
#define PIN_ONE_WIRE_BUS 	6

//-------------------------------- Errors
#define NO_ERROR			0
#define ERROR_NO_HEAT		1
#define ERROR_NO_SENSOR		2

//-------------------------------- Globals

OneWire 						oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature 				temperature_sensors(&oneWire);
DeviceAddress 					acid_temperature_sensor_address = ACID_TEMPERATURE_SENSOR_ADDRESS;
DeviceAddress					ambient_temperature_sensor_address= AMBIENT_TEMPERATURE_SENSOR_ADDRESS;
DeviceAddress					water_bath_temperature_sensor_address = WATER_BATH_TEMPERATURE_SENSOR_ADDRESS;


float 							water_bath_temperature,
								acid_temperature,
								ambient_temperature;
								
long							acid_heat_start_time;
float							acid_heat_start_temperature;
char							acid_error;
char							acid_no_sensor_count;

long							water_bath_heat_start_time;
float							water_bath_heat_start_temperature;
char							water_bath_error;
char							water_no_sensor_count;

char							lighpad_power, uv_power;
char							lighpad_target_power, uv_target_power;
int 							bubble_power, bubble_target_power;


long 							last_user_input;		// millis() timestamp of last time button clicked

U8G2_SSD1306_128X64_NONAME_F_HW_I2C 		u8g2(U8G2_R0, 
												/* reset=*/ U8X8_PIN_NONE, 
												/* clock=*/ 12, 
												/* data=*/ 11);   // ESP32 Thing, HW I2C with pin remapping
Timer										t;
WiFiServer 									server(80);
 	
char ssid[] = SECRET_SSID;			
char pass[] = SECRET_PASS;		
int keyIndex = 0;									// your network key Index number (needed only for WEP)
int wifi_sftatus = WL_IDLE_STATUS;

char 										uv_state = 0;
long 										uv_stop_time;
char 										in_standby;


//========================================================================================
//----------------------------------------------------------------------------------------
//																				SETUP

void setup() {
	pinMode(LED_BUILTIN, 		OUTPUT);			
	pinMode(PIN_RELAY_HEAT, 	OUTPUT);			
	pinMode(PIN_RELAY_HEAT_2, 	OUTPUT);			
	pinMode(PIN_UV, 			OUTPUT);			
	pinMode(PIN_LIGHT, 			OUTPUT);			
	pinMode(PIN_BUBBLES,	 	OUTPUT);			
	pinMode(PIN_PIEZO, 			OUTPUT);			
	pinMode(PIN_SWITCH, 		INPUT_PULLUP);			

	Serial.begin(9600);			
	
	sound_welcome();
	// SLow PWM on TC4 for air pump
	setup_timer();

	// init OLED display
	u8g2.begin();
	u8g2.setPowerSave(0); 
	u8g2.setFont(u8g2_font_logisoso22_tr);

	// init dallas temperature sensors
	temperature_sensors.begin();
 	temperature_sensors.setWaitForConversion(true);
  	temperature_sensors.setResolution(acid_temperature_sensor_address, 8);
  	temperature_sensors.setResolution(ambient_temperature_sensor_address, 8);
  	temperature_sensors.setResolution(water_bath_temperature_sensor_address, 8);
    temperature_sensors.requestTemperatures(); 

	// set up timed functions called by t.update() repeatedly in loop
	t.every(10, 	check_button);
	t.every(2500, 	check_temperatures);
	t.every(1000, 	check_bubbles);
	t.every(10, 	check_ramps);
	t.every(50, 	update_display);
	t.every(10000, 	safety_check);

#ifdef __WIFI_ENABLED
	check_wifi();
	 t.every(20000,	check_wifi);
#endif

	intro();
	hardware_test();
	
	lighpad_target_power 	= LIGHTPAD_POWER;
	uv_target_power			= 0;
	bubble_target_power		= BUBBLE_SPEED_IDLE;
	
	last_user_input = millis();
	check_temperatures();			// do this once to be sure to get data
	
	
	if (LOG_TEMPERATURES) {
		Serial.println("------");
		Serial.println("Temperature Log");
		Serial.println("Time\tAmbient Temperature\tAcid Heater On\tAcid Temperature\tWater Heater On\tWater Temperature ");
		t.every(30000, 	log_temperatures);
	}

}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				UTILITIES
void log_temperatures() {
	long time = millis()/1000;
	if (time/60 < 10)		Serial.print("0");
	Serial.print(time/60);
	Serial.print(":");
	time = time % 60;
	if (time < 10) 		 Serial.print("0");
	Serial.print(time);
	Serial.print("\t");
	Serial.print(ambient_temperature);
	Serial.print("\t");
	Serial.print(digitalRead(PIN_RELAY_HEAT));
	Serial.print("\t");
	Serial.print(acid_temperature);
	Serial.print("\t");
	Serial.print(digitalRead(PIN_RELAY_HEAT_2));
	Serial.print("\t");
	Serial.print(water_bath_temperature);
	Serial.println();
}

//----------------------------------------------------------------------------------------
//												pulse a relay on/off to hear if it works
void test_pulse_pin(byte pin){
	digitalWrite(pin,	 	HIGH);
	delay(500);
	digitalWrite(pin,		LOW);
	delay(500);
}

//----------------------------------------------------------------------------------------
//															move down n pixels on display
void next_line(char height) {
	static char y;
	if (height == 0) y = 0;
	else {
		y+= height;
		u8g2.setCursor(0,y);  	
	}
}

//----------------------------------------------------------------------------------------
// 										print a dallas temperture sensor device address
void print_address(DeviceAddress deviceAddress) {
	Serial.print("{");

  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) {
    	Serial.print("0");
    	u8g2.print("0");
	}
    u8g2.print(deviceAddress[i], HEX);
    Serial.print(deviceAddress[i], HEX);
    if (i < 7)     Serial.print(", ");
  }
  Serial.println("}");
}

//----------------------------------------------------------------------------------------
//																		Measure power draw
void all_on() {
	digitalWrite(LED_BUILTIN, 			HIGH);			
	digitalWrite(PIN_RELAY_HEAT, 		HIGH);			
	digitalWrite(PIN_RELAY_HEAT_2, 		HIGH);			
	analogWrite(PIN_UV, 				255);			
	analogWrite(PIN_LIGHT, 				255);			
	bubble_power = 255;
	bubble_target_power =255;
}

void all_off() {
	digitalWrite(LED_BUILTIN, 			LOW);			
	digitalWrite(PIN_RELAY_HEAT, 		LOW);			
	digitalWrite(PIN_RELAY_HEAT_2, 		LOW);			
	analogWrite(PIN_UV, 				0);			
	analogWrite(PIN_LIGHT, 				0);			
	bubble_power = 0;
	bubble_target_power =0;
	digitalWrite(PIN_LIGHT, 		LOW);			


}
//----------------------------------------------------------------------------------------
//																	Nice and warm Welcome
void intro() {

	u8g2.clearBuffer();
	u8g2.drawBitmap(0, 0, 16, 16,bitmap_anyma);
	u8g2.drawBitmap(32, 16, 8, 48,bitmap_osh);
	u8g2.sendBuffer();
 	delay(2000);

	u8g2.drawBitmap(32, 16, 8, 48,bitmap_box);

	u8g2.sendBuffer();
	delay (2000);
}


//----------------------------------------------------------------------------------------
//																			   Wifi Status
void printWifiStatus() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your board's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	u8g2.clearBuffer();
	u8g2.setCursor(0,16);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.print(ip);
	u8g2.setFont(u8g2_font_logisoso22_tr);
	u8g2.setCursor(30,40);
	u8g2.print("ATZ");
	u8g2.setCursor(30,63);
	u8g2.print("BOX");
   	u8g2.sendBuffer();


	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
	// print where to go in a browser:
	Serial.print("To see this page in action, open a browser to http://");
	Serial.println(ip);
}



//========================================================================================
//----------------------------------------------------------------------------------------
//																		TIMER FUNCTIONS

//----------------------------------------------------------------------------------------
//											 						 		Safety check
void safety_check() {
	long time_since_last_click = millis() - last_user_input;
	
	if (acid_heat_start_time) {
		if ((millis() - acid_heat_start_time) > ACID_TEMPERATURE_CHECK_WAIT * 60000) {
			if ((acid_temperature - acid_heat_start_temperature) > ACID_TEMPERATURE_CHECK_RISE) {
				// passed check. don't check again until next heating period
				acid_heat_start_time = 0;
				acid_error = NO_ERROR;
			} else {
				acid_error = ERROR_NO_HEAT;
				sound_error();
			}
		}
	}

	if (water_bath_heat_start_time) {
		if ((millis() - water_bath_heat_start_time) > WATER_BATH_TEMPERATURE_CHECK_WAIT * 60000) {
			if ((water_bath_temperature - water_bath_heat_start_temperature) > WATER_BATH_TEMPERATURE_CHECK_RISE) {
				// passed check. don't check again until next heating period
				water_bath_heat_start_time = 0;
				water_bath_error = NO_ERROR;
			} else {
				water_bath_error = ERROR_NO_HEAT;
				sound_error();
			}
		}
	}
		
	if (time_since_last_click > STANDBY_TIME * 60000) {
		sound_standby();
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.setCursor(8,10);
		u8g2.print("Standby");
		u8g2.sendBuffer();
		
		all_off();
		all_off();	
		in_standby = 1;
		
		char state = digitalRead(PIN_SWITCH);			// stay in loop until button is clicked
		char old_state = state;
		while (state == old_state) {
			state = digitalRead(PIN_SWITCH);
			delay(200);
		}
		in_standby = 0;
		bubble_target_power = BUBBLE_SPEED_IDLE;
		last_user_input = millis();
		sound_welcome();
		intro();
	}

}

//----------------------------------------------------------------------------------------
//											 						Display error message
void check_wifi() {
	Serial.println("Wifi?");
	Serial.println(ssid);
	return;
		#ifdef __WIFI_ENABLED

		static boolean server_running;
		if (server_running) return;
	
		// check for the WiFi module:
		if (WiFi.status() == WL_NO_MODULE) {
			error("No Communication with WiFi module");
			return;
		}

		String fv = WiFi.firmwareVersion();
		if (fv < "1.0.0") {
			error("Please upgrade the firmware");
			return;
		}
	
		// attempt to connect to Wifi network:
		if (WiFi.status() != WL_CONNECTED) {
			WiFi.begin(ssid, pass);
		} else {
		}
	#endif
}

//----------------------------------------------------------------------------------------
//																				button

void check_button() {	
	static char old_state;
	char state = digitalRead(PIN_SWITCH);
	if (state == old_state) return;
	old_state = state;
	if (state) return;
	
	last_user_input = millis();
	
	if (uv_state) {
		sound_cancel_flash();
		stop_flash();
	} else {
			
		if 			(acid_error == ERROR_NO_HEAT) {
			acid_error = NO_ERROR;
			acid_heat_start_time = 0;
			
		} else if 	(water_bath_error == ERROR_NO_HEAT) {
			water_bath_error = NO_ERROR;
			water_bath_heat_start_time = 0;
			
		} else {

			sound_start_flash();
			uv_state = 1;
			uv_target_power = UV_POWER;
			uv_stop_time = millis() + ((unsigned long)UV_TIME * (unsigned long)1000);
		}
	}
}

//----------------------------------------------------------------------------------------
//																				temperature
void check_temperatures() {	
  	static char did_request;
  	
  	if (!did_request) {
  		temperature_sensors.requestTemperatures();
  		did_request = 1;
  		
  	} else {
  	  	did_request = 0;
  	  	
		water_bath_temperature 	= temperature_sensors.getTempC(water_bath_temperature_sensor_address);
		acid_temperature		= temperature_sensors.getTempC(acid_temperature_sensor_address);
		ambient_temperature		= temperature_sensors.getTempC(ambient_temperature_sensor_address);
	
	
		// ----------------------------------------- ACID
		if (acid_temperature > -50.) {
			acid_no_sensor_count = 0;
			if (acid_error != ERROR_NO_HEAT) acid_error = NO_ERROR;
			if (acid_temperature < ACID_MINIMUM_TEMPERATURE) {
				if (acid_error == NO_ERROR) {
					if (digitalRead(PIN_RELAY_HEAT) == LOW) {
						digitalWrite(PIN_RELAY_HEAT,HIGH);
						acid_heat_start_time = millis();
						acid_heat_start_temperature = acid_temperature;
					}
				}
			}
			if (acid_temperature > ACID_MAXIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT,LOW);
		} else {
			if (acid_no_sensor_count < 5) acid_no_sensor_count++;		// sometimes they fail to read??
			if (acid_no_sensor_count >= 5) {
				acid_error = ERROR_NO_SENSOR;
			}
		}
		
		if (acid_error != NO_ERROR)	digitalWrite(PIN_RELAY_HEAT,LOW);
	

		// ----------------------------------------- WATER
		if (water_bath_temperature > -50.) {
			water_no_sensor_count = 0;
			if (water_bath_error != ERROR_NO_HEAT) water_bath_error = NO_ERROR;
			if (water_bath_temperature < WATER_BATH_MINIMUM_TEMPERATURE) {
				if (water_bath_error == NO_ERROR) {
					if (digitalRead(PIN_RELAY_HEAT_2) == LOW) {
						digitalWrite(PIN_RELAY_HEAT_2,HIGH);
						water_bath_heat_start_time = millis();
						water_bath_heat_start_temperature = water_bath_temperature;
					}
				}
			}
			if (water_bath_temperature > WATER_BATH_MAXIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT_2,LOW);
		} else {
			if (water_no_sensor_count < 5) water_no_sensor_count++;		// sometimes they fail to read??
			if (water_no_sensor_count >= 5) {
				water_bath_error = ERROR_NO_SENSOR;
			}
		}
		
		if (water_bath_error != NO_ERROR)	digitalWrite(PIN_RELAY_HEAT_2,LOW);
	}
}

//----------------------------------------------------------------------------------------
//																				temperature
void check_bubbles() {	
	long time_since_last_click = millis() - last_user_input;
	static char sound_played;
	if (time_since_last_click > BUBBLE_IDLE_TIMEOUT * 60000) {
			bubble_target_power  = BUBBLE_SPEED_IDLE;
		if (!sound_played) sound_timeout();
		sound_played = 1;
	} else {
		sound_played = 0;
	}
}


//----------------------------------------------------------------------------------------
//																				ramp analog outs
void check_ramps() {	
	if (lighpad_power < lighpad_target_power) lighpad_power++;
	if (lighpad_power > lighpad_target_power) lighpad_power--;
	analogWrite(PIN_LIGHT, lighpad_power);

	if (uv_power < uv_target_power) uv_power++;
	if (uv_power > uv_target_power) uv_power--;
	analogWrite(PIN_UV, uv_power);
	
	
	if (bubble_power < bubble_target_power) bubble_power++;
	if (bubble_power > bubble_target_power) bubble_power--;
}
//----------------------------------------------------------------------------------------
//																		uv flashing done
void stop_flash() {
	uv_state = 0;
	uv_target_power = 0;
	uv_power = 0;						// full stop. do not ramp down
	analogWrite(PIN_UV, uv_power);
	delay(1000);
	bubble_target_power =  BUBBLE_SPEED_NORMAL;
	last_user_input = millis();
}

//----------------------------------------------------------------------------------------
//																				mm:ss
void print_time(long time) {
		time /= 1000;
		if (time/60 < 10)		u8g2.print("0");

		u8g2.print(time/60);

		u8g2.print(":");

		time = time % 60;
		if (time < 10) 		 u8g2.print("0");
		u8g2.print(time);
	}
//----------------------------------------------------------------------------------------
//																				sounds
//----------------------------------------------------------------------------------------
void sound_start_flash() {
	tone(PIN_PIEZO,880,100);
}

void sound_end_flash() {
	tone(PIN_PIEZO,659,100);				
	delay(200);
	tone(PIN_PIEZO,659,100);
	delay(100);
	tone(PIN_PIEZO,880,400);
}

void sound_cancel_flash() {
	tone(PIN_PIEZO,880,100);				
	delay(200);
	tone(PIN_PIEZO,659,100);

}

void sound_welcome() {
	tone(PIN_PIEZO,800,100);
}

void sound_timeout() {
	tone(PIN_PIEZO,261,200);				
	delay(100);	
	tone(PIN_PIEZO,233,200);				
	delay(100);	
	tone(PIN_PIEZO,220,200);				
	delay(100);	
	tone(PIN_PIEZO,195,200);				
	delay(100);	
	tone(PIN_PIEZO,174,800);				
}

void sound_standby() {
	tone(PIN_PIEZO,880,100);				
	delay(200);
	tone(PIN_PIEZO,659,50);
}

void sound_error() {
	tone(PIN_PIEZO,400,100);				
	delay(200);
	tone(PIN_PIEZO,400,100);				
	delay(200);
	tone(PIN_PIEZO,400,100);				
}
//----------------------------------------------------------------------------------------
//																				display
//----------------------------------------------------------------------------------------
//																				display
void update_display() { 
	unsigned long time;
	time = millis() - last_user_input;

	u8g2.clearBuffer();

	u8g2.setFont(u8g2_font_fub14_tf);
	if (uv_state) {
		u8g2.setCursor(5, 14);
		u8g2.print("!! UV ON !!");
	}
	else {
		u8g2.setCursor(1, 14);
		u8g2.print(ambient_temperature, 0);
		u8g2.print("\xb0");
		u8g2.setCursor(70, 14);
		print_time(time);
	}
	//u8g2.setFont(u8g2_font_6x12_tr);

	if (uv_state) {
		u8g2.setFont(u8g2_font_logisoso30_tf);
		u8g2.setCursor(24, 58);
		u8g2.drawFrame(18,22,96,42);
		time = uv_stop_time - millis();
	
		if (time < 500) {
			stop_flash();
			sound_end_flash();
		}
		
		print_time(time);
	} else {
	
		u8g2.drawFrame(0,16,103,48);
		u8g2.drawFrame(102,16,26,48);
		u8g2.setCursor(3, 38);
		// ------------------------------ Acid
		u8g2.setFont(u8g2_font_fub14_tf);
		u8g2.print("Acid   : ");
		
		switch (acid_error) {
		
			case NO_ERROR:
				u8g2.drawCircle(115, 30, 7, U8G2_DRAW_ALL);
				u8g2.print(acid_temperature, 0);
		
				if (digitalRead(PIN_RELAY_HEAT)) {
					u8g2.drawDisc(115, 30, 5, U8G2_DRAW_ALL);
				}
				break;
			
			case ERROR_NO_HEAT:
				u8g2.setCursor(115, 38);
				u8g2.print("X");
				break;
			
			case ERROR_NO_SENSOR:
				u8g2.print("--");
				break;
		}		

		// ------------------------------ Water
		u8g2.setCursor(3, 60);
		u8g2.print("Water: ");
	
		switch (water_bath_error) {
		
			case NO_ERROR:
				u8g2.drawCircle(115, 52, 7, U8G2_DRAW_ALL);
				u8g2.print(water_bath_temperature, 0);
		
				if (digitalRead(PIN_RELAY_HEAT_2)) {
					u8g2.drawDisc(115, 52, 5, U8G2_DRAW_ALL);
				}
				break;
			
			case ERROR_NO_HEAT:
				u8g2.setCursor(115, 60);
				u8g2.print("X");
				break;
			
			case ERROR_NO_SENSOR:
				u8g2.print("--");
				break;
		}		
	}
	u8g2.sendBuffer();
}





//========================================================================================
//----------------------------------------------------------------------------------------
//																				loop

void loop() {
	t.update();
}





//----------------------------------------------------------------------------------------
//																				Slow PWM on SAMD Timer 4$
//																				to prevent whining of air pump

void setup_timer() {
   // Set up the generic clock (GCLK4) used to clock timers
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
 
  REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC4_COUNT8_CC0 = 0x55;                      // Set the TC4 CC0 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_CC1 = 0xAA;                      // Set the TC4 CC1 register to some arbitary value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC4_COUNT8_PER = 0xFF;                      // Set the PER (period) register to its maximum value
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC4_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts
 
  REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV64 |     // Set prescaler to 64, 16MHz/64 = 256kHz
                   TC_CTRLA_ENABLE;               // Enable TC4
  while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}

//----------------------------------------------------------------------------------------
// 									Interrupt Service Routine (ISR) for timer TC4 (Bubble PWM)
void TC4_Handler() {     
	static char cnt;
  // Check for overflow (OVF) interrupt
  if (TC4->COUNT8.INTFLAG.bit.OVF && TC4->COUNT8.INTENSET.bit.OVF)             
  {
  	if (in_standby) digitalWrite(PIN_BUBBLES,LOW);
  	else {
   		cnt++;
		if (cnt < (bubble_power / 2)) digitalWrite(PIN_BUBBLES,HIGH);
   		else digitalWrite(PIN_BUBBLES,LOW);
  	 	if (cnt >= 127) cnt = 0;
  	 }
  }
  // Clear all interrupt flags
    REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        

}


//----------------------------------------------------------------------------------------
//																			test hardware
void hardware_test() {
	if (DISABLE_HARDWARE_CHECK) return;
	
	int i; 
	delay(1000);
	if (HARDWARE_CHECK_RELAY) {
		u8g2.clearBuffer();
		next_line(0);
		next_line(10);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.print("Hardware Check:");
		u8g2.sendBuffer();
		tone(PIN_PIEZO,800,100);
		delay(500);

		next_line(16);
		u8g2.print("Main Heater Relay");
		u8g2.sendBuffer();
		test_pulse_pin(PIN_RELAY_HEAT);

		next_line(9);
		u8g2.print("Secondary Heater Relay");
		u8g2.sendBuffer();
		test_pulse_pin(PIN_RELAY_HEAT_2);

		delay(500);
		next_line(9);
		u8g2.print("Lightpad");
		u8g2.sendBuffer();

		for(i = 0; i < 255; i++) {
			analogWrite(PIN_LIGHT,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			analogWrite(PIN_LIGHT,i);
			delay(1);
		}
		analogWrite(PIN_LIGHT,0);


		delay(500);
		next_line(9);
		u8g2.print("UV Light");
		u8g2.sendBuffer();
		for(i = 0; i < 255; i++) {
			analogWrite(PIN_UV,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			analogWrite(PIN_UV,i);
			delay(1);
		}
		analogWrite(PIN_UV,0);

		delay(500);
		next_line(9);
		u8g2.print("Bubbles");
		u8g2.sendBuffer();
		for(i = 0; i < 255; i++) {
			analogWrite(PIN_BUBBLES,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			analogWrite(PIN_BUBBLES,i);
			delay(1);
		}
		analogWrite(PIN_BUBBLES,0);
		delay(500);
		tone(PIN_PIEZO,880,100);
	}
	
	if (HARDWARE_CHECK_TEMPERATURE) {
		u8g2.clearBuffer();
		next_line(0);
		next_line(10);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.print("Hardware Check:");
		u8g2.sendBuffer();
		temperature_sensors.requestTemperatures(); 
		next_line(16);

		int tc;
		tc = temperature_sensors.getDeviceCount();
		DeviceAddress temporary_address;
		float tempC;
	
		u8g2.print(tc, DEC);
		u8g2.print(" Temp. Sensors found");
		if (tc) u8g2.print(":");
		else u8g2.print(".");
		u8g2.sendBuffer();
		next_line(16);
	

		for ( char i = 0; i < tc; i++)	{
			temperature_sensors.getAddress(temporary_address, i);

				Serial.print("Temperature sensor ");
				Serial.print(i);
				Serial.print(" address: ");


			print_address(temporary_address);
			u8g2.print(" @ ");
			tempC = temperature_sensors.getTempCByIndex(i);
			u8g2.print(tempC);
			u8g2.print("C");
			next_line(16);

		}
		u8g2.sendBuffer();
	} 
	
	delay(500);
	tone(PIN_PIEZO,900,100);

	boolean wait = true;
	long wait_until = millis() + 10000;

	while (wait) {
		if (millis() > wait_until) wait = false;
		if (digitalRead(PIN_SWITCH) == LOW)  wait = false;
	}

	tone(PIN_PIEZO,900,100);
	delay(200);
	tone(PIN_PIEZO,900,100);
}
