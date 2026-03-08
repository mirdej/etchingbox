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
//																				Exposure
//
// UV_TIME	sets UV exposure time in seconds. Default is 90 
#define UV_TIME				10
#define UV_POWER			200
#define LIGHTPAD_POWER		50

//----------------------------------------------------------------------------------------
//																				Acid Bath
//
// Recommended settings for acid bath are 42 and 48 degrees.
// Dallas temperature sensors all have a unique device address.
// Copy the address for your sensor from the Arduino serial monitor and paste below
#define ACID_MINIMUM_TEMPERATURE		42
#define ACID_MAXIMUM_TEMPERATURE		48
#define ACID_TEMPERATURE_SENSOR_ADDRESS	{0x28, 0x78, 0x87, 0x45, 0x92, 0x18, 0x02, 0x9D}
//#define ACID_TEMPERATURE_SENSOR_ADDRESS  {0x28, 0xFA, 0xD6, 0x45, 0x92, 0x11, 0x02, 0xF3}

// This sets the PWM of the bubble pump: 0 = No Airflow, 255 Maximum Airflow
// Idle Speed is when machine starts up, or no user input for BUBBLE_IDLE_TIMEOUT (in minutes)
// You should not set this to 0 in order to always keep water flowing around the heater
// Normal speed starts after UV exposure finishes and lasts for BUBBLE_IDLE_TIMEOUT minutes
#define BUBBLE_SPEED_IDLE					0
#define BUBBLE_SPEED_NORMAL					130
#define BUBBLE_IDLE_TIMEOUT					20

//----------------------------------------------------------------------------------------
//																				Water Bath
//
// If you use a water bath (secondary heater) for development set minimal and maximal 
// temperatures here in degrees celsius. Recommended settings are 22 and 26 degrees.
// Dallas temperature sensors all have a unique device address.
// Copy the address for your sensor from the Arduino serial monitor and paste below
#define WATER_BATH_MINIMUM_TEMPERATURE		28
#define WATER_BATH_MAXIMUM_TEMPERATURE		30
#define WATER_BATH_TEMPERATURE_SENSOR_ADDRESS	 {0x28, 0xDD, 0x62, 0x45, 0x92, 0x02, 0x02, 0x29}

// the single one for prototyping
//#define WATER_BATH_TEMPERATURE_SENSOR_ADDRESS   {0x28, 0xFA, 0xD6, 0x45, 0x92, 0x11, 0x02, 0xF3}



//----------------------------------------------------------------------------------------
//																				Ambient temperature
#define AMBIENT_TEMPERATURE_SENSOR_ADDRESS	{0x28, 0xFA, 0xD6, 0x45, 0x92, 0x11, 0x02, 0xF3}


//----------------------------------------------------------------------------------------
//																				Logging
//
// Perform different hardware checks at startup? Set first one to true to disable all checks
#define DISABLE_HARDWARE_CHECK			true
#define HARDWARE_CHECK_RELAY			true
#define HARDWARE_CHECK_TEMPERATURE		true 
