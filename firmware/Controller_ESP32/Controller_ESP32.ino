//========================================================================================
//----------------------------------------------------------------------------------------
//
//	ÄtzBox Controller Firmware		
//						
//		Target MCU: ESP32 WROOM module
//		Copyright:	2021 Michael Egger, me@anyma.ch
//		License: 	This is FREE software (as in free speech, not necessarily free beer)
//					published under gnu GPL v.3
//
//----------------------------------------------------------------------------------------

#include <SPI.h>
#include <SPIFFS.h>
#include <U8g2lib.h>
#include <Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Preferences.h>
#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <AsyncElegantOTA.h>

#include "Logo.h" 



//-------------------------------- Pins
#define PIN_RELAY_HEAT 		16
#define PIN_RELAY_HEAT_2 	17
#define PIN_UV 				23
#define	PIN_LIGHT			18
#define	PIN_BUBBLES			19
#define PIN_SWITCH			33
#define	PIN_PIEZO			25
#define PIN_ONE_WIRE_BUS 	4


#define UV_CHANNEL          0
#define LIGHT_CHANNEL       1
#define BUBBLE_CHANNEL      2

//-------------------------------- Errors
#define NO_ERROR			0
#define ERROR_NO_HEAT		1
#define ERROR_NO_SENSOR		2
//-------------------------------- Settings

int STANDBY_TIME;
int UV_TIME;
int UV_POWER;
int LIGHTPAD_POWER;
int ACID_MINIMUM_TEMPERATURE;
int ACID_MAXIMUM_TEMPERATURE;
int BUBBLE_SPEED_IDLE;
int BUBBLE_SPEED_NORMAL;
int BUBBLE_IDLE_TIMEOUT;
int WATER_BATH_MINIMUM_TEMPERATURE;
int WATER_BATH_MAXIMUM_TEMPERATURE;
int LOG_TEMPERATURES;

#define HARDWARE_CHECK_TEMPERATURE  0
#define    HARDWARE_CHECK_RELAY     0
//-------------------------------- Globals

bool                            wifi_enabled;

Preferences                     preferences;
String                          hostname;
WiFiManager                     wm;
WiFiManagerParameter            custom_field;
AsyncWebServer                  server(80);
AsyncWebSocket ws("/ws");


OneWire 						oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature 				temperature_sensors(&oneWire);
DeviceAddress 					acid_temperature_sensor_address;
DeviceAddress					ambient_temperature_sensor_address;
DeviceAddress					water_bath_temperature_sensor_address;


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
												/* clock=*/ 22, 
												/* data=*/ 21);   // ESP32 Thing, HW I2C with pin remapping
												
Timer										t;

char 										uv_state = 0;
long 										uv_stop_time;
char 										in_standby;


//========================================================================================
//----------------------------------------------------------------------------------------
//																				SETUP

void get_preferences(){
    preferences.begin("anyma", false);
    hostname = preferences.getString("hostname", WiFi.macAddress());
    // time in minutes after which the machine shuts down if there wasn't any user input
    STANDBY_TIME                        = preferences.getUInt("standby_time",	 45);
    UV_TIME                             = preferences.getUInt("uv_time",		    120);
    UV_POWER                            = preferences.getUInt("uv_power",		255);
    LIGHTPAD_POWER                      = preferences.getUInt("light_power",		200);
    ACID_MINIMUM_TEMPERATURE            = preferences.getUInt("acid_min",         45);
    ACID_MAXIMUM_TEMPERATURE            = preferences.getUInt("acid_max",		 48);
    BUBBLE_SPEED_IDLE                   = preferences.getUInt("bubble_idle",      40);
    BUBBLE_SPEED_NORMAL                 = preferences.getUInt("bubble_normal",   200);
    BUBBLE_IDLE_TIMEOUT                 = preferences.getUInt("bubble_timeout",   30);
    WATER_BATH_MINIMUM_TEMPERATURE      = preferences.getUInt("water_min",		 25);
    WATER_BATH_MAXIMUM_TEMPERATURE      = preferences.getUInt("water_max",		 32);
    LOG_TEMPERATURES                    = preferences.getUInt("log_temp",		  1);

    preferences.getBytes("acid_addr",acid_temperature_sensor_address,8);
    uint8_t t[] =  {0x28, 0xE8, 0x03, 0x45, 0x92, 0x11, 0x02, 0x4A};
    memcpy(acid_temperature_sensor_address,t,8);
    
    preferences.getBytes("ambi_addr",ambient_temperature_sensor_address,8);
    preferences.getBytes("water_addr",water_bath_temperature_sensor_address,8);
    hostname = preferences.getString("hostname");

    preferences.end();
}

void setup() {
	pinMode(PIN_RELAY_HEAT, 	OUTPUT);			
	pinMode(PIN_RELAY_HEAT_2, 	OUTPUT);			
	pinMode(PIN_UV, 			OUTPUT);			
	pinMode(PIN_LIGHT, 			OUTPUT);			
	pinMode(PIN_BUBBLES,	 	OUTPUT);			
	pinMode(PIN_PIEZO, 			OUTPUT);			
	pinMode(PIN_SWITCH, 		INPUT_PULLUP);			

    //setup PWM Outputs
    ledcSetup(UV_CHANNEL, 5000, 8);              // ledcSetup(ledChannel, freq, resolution););
    ledcAttachPin(PIN_UV, UV_CHANNEL);           // ledcAttachPin(ledPin, ledChannel);
  
    ledcSetup(LIGHT_CHANNEL, 5000, 8);      
    ledcAttachPin(PIN_LIGHT, LIGHT_CHANNEL); 

    ledcSetup(BUBBLE_CHANNEL, 50, 8);      
    ledcAttachPin(PIN_BUBBLES, BUBBLE_CHANNEL);

	Serial.begin(115200);			
	SPIFFS.begin();
	
	sound_welcome();
    get_preferences();

	// init OLED display
	u8g2.begin();
	u8g2.setPowerSave(0); 
	u8g2.setFont(u8g2_font_logisoso22_tr);

	intro();

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

	
	lighpad_target_power 	= LIGHTPAD_POWER;
	uv_target_power			= 0;
	bubble_target_power		= BUBBLE_SPEED_IDLE;
	
	last_user_input = millis();
	check_temperatures();			// do this once to be sure to get data
	
    // use Wifi-Manager to check if we have Wifi connection
    
    wm.setDarkMode(true);
    wm.setCountry("CH"); 
    wm.setShowPassword(true);
    wm.setHostname(hostname.c_str());
    wm.setConnectTimeout(20); // how long to try to connect for before continuing
    wm.setConfigPortalTimeout(40); // auto close configportal after n seconds
    wm.setAPClientCheck(true); // avoid timeout if client connected to softap
    
    new (&custom_field) WiFiManagerParameter("hostname", "Hostname", hostname.c_str(), 40,"placeholder=\"Custom Field Placeholder\"");
    wm.addParameter(&custom_field);
    wm.setSaveParamsCallback(saveParamCallback);
  
    String apname ="Homeless-"+hostname;
    bool res;
    res = wm.autoConnect(apname.c_str());
    
    if(!res) {
        Serial.println("Failed to connect to WIFI");
        wifi_enabled = false;
    } else {
        //if you get here you have connected to the WiFi  
        wifi_enabled = true;
  
        Serial.println("connected...yeey :)");
        Serial.print("Ätzbox Ready! Use 'http://");
        Serial.print(WiFi.localIP());
        Serial.println("' to connect");
        
        if (!MDNS.begin(hostname.c_str())) {
            Serial.println("Error setting up MDNS responder!");
        } else {
            Serial.println("mDNS responder started");
            Serial.printf ("Hostname: %s\n",hostname);    
            MDNS.addService("http", "tcp", 80); 
        }
        
        //websocket
        ws.onEvent(onEvent);
        server.addHandler(&ws);
        server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
        server.serveStatic("/src/", SPIFFS, "/src/");
        AsyncElegantOTA.begin(&server);    // Start ElegantOTA
        server.begin();
    }
    hardware_test();    
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				Wifi Manager

String getParam(String name){
    //read parameter from server, for customhmtl input
    String value;
    if(wm.server->hasArg(name)) { value = wm.server->arg(name);}
  return value;
}

void saveParamCallback(){
    Serial.println("[CALLBACK] saveParamCallback fired");
    Serial.println("PARAM hostname = " + getParam("hostname"));
    preferences.begin("anyma");
    hostname =  getParam("hostname");
    preferences.putString("hostname", hostname);
    preferences.end();
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				Websocket

void notifyClients() {
    if (!wifi_enabled) return;
    
    String response;
    response = "TEMP:" +String(acid_temperature) + "," +String(water_bath_temperature) + "," +String(ambient_temperature);
    ws.textAll(response);
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      notifyClients();
    }
  }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
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
	digitalWrite(PIN_RELAY_HEAT, 		HIGH);			
	digitalWrite(PIN_RELAY_HEAT_2, 		HIGH);			
	ledcWrite(UV_CHANNEL, 				  255);			
	ledcWrite(LIGHT_CHANNEL, 			    255);			
	bubble_power = 255;
	bubble_target_power =255;
}

void all_off() {
	digitalWrite(PIN_RELAY_HEAT, 		LOW);			
	digitalWrite(PIN_RELAY_HEAT_2, 		LOW);			
	ledcWrite(UV_CHANNEL, 				0);			
	ledcWrite(LIGHT_CHANNEL, 				0);			
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
/*	Serial.print("SSID: ");
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
	Serial.println(ip);*/
}



//========================================================================================
//----------------------------------------------------------------------------------------
//																		TIMER FUNCTIONS

//----------------------------------------------------------------------------------------
//											 						 		Safety check
void safety_check() {
	long time_since_last_click = millis() - last_user_input;

// TODO Water sensing

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
	
        notifyClients();  //    send temperature data via websocket
	
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
	ledcWrite(LIGHT_CHANNEL, lighpad_power);

	if (uv_power < uv_target_power) uv_power++;
	if (uv_power > uv_target_power) uv_power--;
	ledcWrite(UV_CHANNEL, uv_power);
	
	
	if (bubble_power < bubble_target_power) bubble_power++;
	if (bubble_power > bubble_target_power) bubble_power--;
    ledcWrite(BUBBLE_CHANNEL, bubble_power);
}
//----------------------------------------------------------------------------------------
//																		uv flashing done
void stop_flash() {
	uv_state = 0;
	uv_target_power = 0;
	uv_power = 0;						// full stop. do not ramp down
	ledcWrite(UV_CHANNEL, uv_power);
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
}

void sound_end_flash() {
}

void sound_cancel_flash() {
}

void sound_welcome() {
}

void sound_timeout() {
}

void sound_standby() {
}

void sound_error() {
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
    ws.cleanupClients();
	AsyncElegantOTA.loop();
}






//----------------------------------------------------------------------------------------
//																			test hardware

void hardware_test() {
	if (!(HARDWARE_CHECK_RELAY | HARDWARE_CHECK_TEMPERATURE))return;
	int i; 
	delay(1000);
	if (HARDWARE_CHECK_RELAY) {
		u8g2.clearBuffer();
		next_line(0);
		next_line(10);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.print("Hardware Check:");
		u8g2.sendBuffer();
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
			ledcWrite(LIGHT_CHANNEL,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			ledcWrite(LIGHT_CHANNEL,i);
			delay(1);
		}
		ledcWrite(LIGHT_CHANNEL,0);


		delay(500);
		next_line(9);
		u8g2.print("UV Light");
		u8g2.sendBuffer();
		for(i = 0; i < 255; i++) {
			ledcWrite(UV_CHANNEL,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			ledcWrite(UV_CHANNEL,i);
			delay(1);
		}
		ledcWrite(UV_CHANNEL,0);

		delay(500);
		next_line(9);
		u8g2.print("Bubbles");
		u8g2.sendBuffer();
		for(i = 0; i < 255; i++) {
			ledcWrite(BUBBLE_CHANNEL,i);
			delay(1);
		}
		for(i = 255; i > 0; i--) {
			ledcWrite(BUBBLE_CHANNEL,i);
			delay(1);
		}
		ledcWrite(BUBBLE_CHANNEL,0);
		delay(500);
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
		Serial.print(tc, DEC);
		u8g2.print(" Temp. Sensors found");
		Serial.println(" Temp. Sensors found");
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

	boolean wait = true;
	long wait_until = millis() + 10000;

	while (wait) {
		if (millis() > wait_until) wait = false;
		if (digitalRead(PIN_SWITCH) == LOW)  wait = false;
	}
}