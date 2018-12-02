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
#define PIN_RELAY_HEAT 	4
#define PIN_RELAY_HEAT_2 	5
#define PIN_UV 				3
#define	PIN_LIGHT			1
#define	PIN_BUBBLES			2
#define PIN_SWITCH			7
#define	PIN_PIEZO			0
#define PIN_ONE_WIRE_BUS 	6

//-------------------------------- Errors
#define ERR_NO_ERR			0
#define ERR_TIMEOUT			1
#define ERR_NO_SENS_1		10

//-------------------------------- Globals

OneWire 						oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature 				temperature_sensors(&oneWire);
DeviceAddress 					acid_temperature_sensor_address = ACID_TEMPERATURE_SENSOR_ADDRESS;
DeviceAddress					ambient_temperature_sensor_address;
DeviceAddress					water_bath_temperature_sensor_address = WATER_BATH_TEMPERATURE_SENSOR_ADDRESS;


float 							water_bath_temperature,
								acid_temperature,
								ambient_temperature;

char							lighpad_power, uv_power, bubble_power;
char							lighpad_target_power, uv_target_power, bubble_target_power;


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
	tone(PIN_PIEZO,800,100);
	
	// init OLED display
	u8g2.begin();
	u8g2.setPowerSave(0); 
	u8g2.setFont(u8g2_font_logisoso22_tr);

	// init dallas temperature sensors
	temperature_sensors.begin();

	// set up timed functions called by t.update() repeatedly in loop
	t.every(20000,	check_wifi);
	t.every(10, 	check_button);
	t.every(2500, 	check_temperatures);
	t.every(20000, 	check_bubbles);
	t.every(10, 	check_ramps);
	t.every(50, 	update_display);

	check_wifi();
	intro();
	hardware_test();
	
	lighpad_target_power 	= LIGHTPAD_POWER;
	uv_target_power			= 0;
	bubble_target_power		= BUBBLE_SPEED_IDLE;
	
	last_user_input = millis();
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				UTILITIES

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
	digitalWrite(PIN_RELAY_HEAT_2, 	HIGH);			
	digitalWrite(PIN_UV, 				HIGH);			
	digitalWrite(PIN_LIGHT, 			HIGH);			
	digitalWrite(PIN_BUBBLES,	 		HIGH);			

}

//----------------------------------------------------------------------------------------
//																	Nice and warm Welcome
void intro() {
	u8g2.clearBuffer();
	u8g2.setCursor(8,16);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.drawBitmap(0, 0, 16, 64,bitmap_logo);

u8g2.sendBuffer();
delay (2000);
}



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



//----------------------------------------------------------------------------------------
//											 						Display error message
void error (const char* err) {
		u8g2.clearBuffer();
		u8g2.setCursor(8,16);
		u8g2.setFont(u8g2_font_logisoso16_tr);
		u8g2.print("Sorry there's an error");
		u8g2.setCursor(32,63);
		u8g2.print(err);
		u8g2.sendBuffer();
		for (char i = 0; i < 4; i++) {
			tone(PIN_PIEZO,1200,1000);
			delay(500);
		}
}



//========================================================================================
//----------------------------------------------------------------------------------------
//																		TIMER FUNCTIONS

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
	
	tone(PIN_PIEZO,880,100);
	last_user_input = millis();
	
	if (uv_state) {
		uv_state = 0;
		uv_target_power = 0;
	} else {
		uv_state = 1;
		uv_target_power = UV_POWER;
		uv_stop_time = millis() + ((unsigned long)UV_TIME * (unsigned long)1000);
	}
}

//----------------------------------------------------------------------------------------
//																				temperature
void check_temperatures() {	
  	temperature_sensors.requestTemperatures();
	water_bath_temperature 	= temperature_sensors.getTempC(water_bath_temperature_sensor_address);
	acid_temperature		= temperature_sensors.getTempC(acid_temperature_sensor_address);
	ambient_temperature		= temperature_sensors.getTempC(ambient_temperature_sensor_address);
	
	if (acid_temperature > -50.) {
		Serial.println(acid_temperature);
		if (acid_temperature < ACID_MINIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT,HIGH);
		if (acid_temperature > ACID_MAXIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT,LOW);
	} else {
		// Problem With Sensor ? Nothing connected? Wrong address ?
		digitalWrite(PIN_RELAY_HEAT,LOW);
	}

	if (water_bath_temperature > -50.) {
		Serial.println(water_bath_temperature);
		if (water_bath_temperature < WATER_BATH_MINIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT_2,HIGH);
		if (water_bath_temperature > WATER_BATH_MAXIMUM_TEMPERATURE) digitalWrite(PIN_RELAY_HEAT_2,LOW);
	} else {
		// Problem With Sensor ? Nothing connected? Wrong address ?
		digitalWrite(PIN_RELAY_HEAT_2,LOW);
	}
}

//----------------------------------------------------------------------------------------
//																				temperature
void check_bubbles() {	
	long time_since_last_click = millis() - last_user_input;
	if (time_since_last_click > BUBBLE_IDLE_TIMEOUT * 60000) {
		bubble_target_power = BUBBLE_SPEED_IDLE;
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
	analogWrite(PIN_BUBBLES, bubble_power);
}
//----------------------------------------------------------------------------------------
//																		uv flashing done
void stop_flash() {
	uv_state = 0;
	uv_target_power = 0;
	tone(PIN_PIEZO,690,100);				
	delay(200);
	tone(PIN_PIEZO,690,100);
	delay(100);
	tone(PIN_PIEZO,880,400);
	
	delay(1000);
	bubble_target_power =  BUBBLE_SPEED_NORMAL;
	last_user_input = millis();
}

//----------------------------------------------------------------------------------------
//																				mm:ss
void print_time(long time) {
		time /= 1000;
		u8g2.print(time/60);

		u8g2.print(":");

		time = time % 60;
		if (time < 10) 		 u8g2.print("0");
		u8g2.print(time);
	}
//----------------------------------------------------------------------------------------
//																				display
void update_display() {	
	unsigned long time;
	time = millis() - last_user_input;

	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_6x12_tr);
	next_line(0);
	next_line(16);
	u8g2.print("Aeztbox          ");
	print_time(time);
		
	next_line(12);
	u8g2.print("Acid       : ");
	
	if (acid_temperature > -100.) {
		u8g2.print(acid_temperature);
		if (digitalRead(PIN_RELAY_HEAT)) {
			u8g2.print("  H");
		}
	} else {
		u8g2.print("Offline");
	}

	next_line(12);
	u8g2.print("Water Bath : ");
	
	if (water_bath_temperature > -100.) {
		u8g2.print(water_bath_temperature);
		if (digitalRead(PIN_RELAY_HEAT_2)) {
			u8g2.print("  H");
		}
	} else {
		u8g2.print("Offline");
	}
	
	next_line(12);
	u8g2.print("Ambient    : ");
	
	if (ambient_temperature > -100.) {
		u8g2.print(ambient_temperature);
	} else {
		u8g2.print("Offline");
	}	
	
	next_line(12);
	if (uv_state) {
		time = uv_stop_time - millis();
	
		if (time < 500) {
				stop_flash();
		}
		
		print_time(time);
	}
	
   	u8g2.sendBuffer();
}



//========================================================================================
//----------------------------------------------------------------------------------------
//																				loop

void loop() {
	t.update();
}

