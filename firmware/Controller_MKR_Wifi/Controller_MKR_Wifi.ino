#include <SPI.h>
#include <WiFiNINA.h>
#include <U8g2lib.h>
#include <Timer.h>

#include "arduino_secrets.h" 

//-------------------------------- Pins
#define PIN_RELAIS_HEAT 	4
#define PIN_RELAIS_HEAT_2 	5
#define PIN_UV 				3
#define	PIN_LIGHT			1
#define	PIN_BUBBLES			2
#define PIN_SWITCH			7
#define	PIN_TEMPERATURE		6
#define	PIN_PIEZO			0

//-------------------------------- Default Settings
#define UV_TIME				90

//-------------------------------- ERRORS
#define ERR_NO_ERR			0
#define ERR_TIMEOUT			1
#define ERR_NO_SENS_1		10


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 12, /* data=*/ 11);   // ESP32 Thing, HW I2C with pin remapping
Timer	t;
 	
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;				// your network SSID (name)
char pass[] = SECRET_PASS;		 // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;									// your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
	
	
//----------------------------------------------------------------------------------------------------
//																				SETUP

void setup() {
	Serial.begin(9600);			
	
	u8g2.begin();
	u8g2.setPowerSave(0); 
	u8g2.setFont(u8g2_font_logisoso22_tr);

	pinMode(LED_BUILTIN, 		OUTPUT);			
	pinMode(PIN_RELAIS_HEAT, 	OUTPUT);			
	pinMode(PIN_RELAIS_HEAT_2, 	OUTPUT);			
	pinMode(PIN_UV, 			OUTPUT);			
	pinMode(PIN_LIGHT, 			OUTPUT);			
	pinMode(PIN_BUBBLES,	 	OUTPUT);			
	pinMode(PIN_PIEZO, 			OUTPUT);			
	pinMode(PIN_SWITCH, 		INPUT_PULLUP);			
	
	intro();
	//hardware_test();
	//init_Wifi();
}

void test_pulse_pin(byte pin){
	digitalWrite(pin,	 	HIGH);
	delay(500);
	digitalWrite(pin,		LOW);
	delay(500);
}
//----------------------------------------------------------------------------------------------------
void hardware_test() {
	int i;
   	tone(PIN_PIEZO,800,100);
	delay(500);
	
	test_pulse_pin(PIN_RELAIS_HEAT);
	test_pulse_pin(PIN_RELAIS_HEAT_2);
	test_pulse_pin(PIN_UV);			
	test_pulse_pin(PIN_LIGHT);			
	test_pulse_pin(PIN_BUBBLES);			
   	tone(PIN_PIEZO,880,100);
   	
   	delay(200);
   	/*
	for(i = 0; i < 255; i++) {
		analogWrite(PIN_LIGHT,i);
		delay(1);
	}
	for(i = 255; i > 0; i--) {
		analogWrite(PIN_LIGHT,i);
		delay(1);
	}
	for(i = 0; i < 255; i++) {
		analogWrite(PIN_UV,i);
		delay(1);
	}
	for(i = 255; i > 0; i--) {
		analogWrite(PIN_UV,i);
		delay(1);
	}
	for(i = 0; i < 255; i++) {
		analogWrite(PIN_BUBBLES,i);
		delay(1);
	}
	for(i = 255; i > 0; i--) {
		analogWrite(PIN_BUBBLES,i);
		delay(1);
	}
	*/
}

void all_on() {
	digitalWrite(LED_BUILTIN, 		HIGH);			
	digitalWrite(PIN_RELAIS_HEAT, 	HIGH);			
	digitalWrite(PIN_RELAIS_HEAT_2, 	HIGH);			
	digitalWrite(PIN_UV, 			HIGH);			
	digitalWrite(PIN_LIGHT, 			HIGH);			
	digitalWrite(PIN_BUBBLES,	 	HIGH);			

}

//----------------------------------------------------------------------------------------------------
void intro() {
	u8g2.clearBuffer();
	u8g2.setCursor(8,16);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.print("Welcome");
	u8g2.setFont(u8g2_font_logisoso22_tr);
	u8g2.setCursor(32,40);
	u8g2.print("ATZ");
	u8g2.setCursor(320,63);
	u8g2.print("BOX");
   	u8g2.sendBuffer();
}


//----------------------------------------------------------------------------------------------------
//																				loop

void loop() {
	all_on();//hardware_test();
	delay(1000);
}




void init_Wifi() {
	// check for the WiFi module:
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Communication with WiFi module failed!");
		// don't continue
		while (true);
	}

	String fv = WiFi.firmwareVersion();
	if (fv < "1.0.0") {
		Serial.println("Please upgrade the firmware");
	}
	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to Network named: ");
		Serial.println(ssid);										// print the network name (SSID);

		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(ssid, pass);
		// wait 10 seconds for connection:
		delay(10000);
	}
	server.begin();														// start the web server on port 80
	printWifiStatus();												// you're connected now, so print out the status
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