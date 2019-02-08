//========================================================================================
//----------------------------------------------------------------------------------------
//
//	ÄtzBox Controller Firmware		
//						
//		Personal Settings
//
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
//																				WIFI
// if you want to enable Wifi create a file named Passwords.h and put your wifi acces code
// in there, don't let your private information spill out on GitHub !!!
// Secondly: uncomment the following line to enable Wifi Code:
//#define __WIFI_ENABLED

#ifdef __WIFI_ENABLED
	#include "Passwords.h"
#else 
	#define SECRET_SSID "DO NOT CHANGE THIS HERE"
	#define SECRET_PASS "DO NOT PUT YOUR PASSWORD ON THE INTERNET"
#endif
//----------------------------------------------------------------------------------------
//																				Safety

// time in minutes after which the machine shuts down if there wasn't any user input
#define STANDBY_TIME		120

//----------------------------------------------------------------------------------------
//																				Exposure
//
// UV_TIME	sets UV exposure time in seconds. Default is 90 
#define UV_TIME				120
#define UV_POWER			255
#define LIGHTPAD_POWER		200

//----------------------------------------------------------------------------------------
//																				Acid Bath
//
// Recommended settings for acid bath are 42 and 48 degrees.
// Dallas temperature sensors all have a unique device address.
// Copy the address for your sensor from the Arduino serial monitor and paste below
#define ACID_MINIMUM_TEMPERATURE		45
#define ACID_MAXIMUM_TEMPERATURE		48
#define ACID_TEMPERATURE_SENSOR_ADDRESS	{0x28, 0x6C, 0xD7, 0x45, 0x92, 0x11, 0x02, 0x3D}

// This sets the PWM of the bubble pump: 0 = No Airflow, 255 Maximum Airflow
// Idle Speed is when machine starts up, or no user input for BUBBLE_IDLE_TIMEOUT (in minutes)
// You should not set this to 0 in order to always keep water flowing around the heater
// Normal speed starts after UV exposure finishes and lasts for BUBBLE_IDLE_TIMEOUT minutes
#define BUBBLE_SPEED_IDLE					40
#define BUBBLE_SPEED_NORMAL					200
#define BUBBLE_IDLE_TIMEOUT					1

//----------------------------------------------------------------------------------------
//																				Water Bath
//
// If you use a water bath (secondary heater) for development set minimal and maximal 
// temperatures here in degrees celsius. Recommended settings are 25 and 32 degrees.
// Dallas temperature sensors all have a unique device address.
// Copy the address for your sensor from the Arduino serial monitor and paste below
#define WATER_BATH_MINIMUM_TEMPERATURE		25
#define WATER_BATH_MAXIMUM_TEMPERATURE		32
#define WATER_BATH_TEMPERATURE_SENSOR_ADDRESS	 {0x28, 0xAC, 0x56, 0x45, 0x92, 0x0F, 0x02, 0x94}


//----------------------------------------------------------------------------------------
//																				Ambient temperature
#define AMBIENT_TEMPERATURE_SENSOR_ADDRESS	 {0x28, 0xD5, 0xB2, 0x45, 0x92, 0x03, 0x02, 0x12}

//----------------------------------------------------------------------------------------
//																				Logging
//
// Log Temperatures to Serial Monitor
#define LOG_TEMPERATURES				true

// Perform different hardware checks at startup? Set first one to true to disable all checks
#define DISABLE_HARDWARE_CHECK			true

#define HARDWARE_CHECK_RELAY			true
#define HARDWARE_CHECK_TEMPERATURE		true 