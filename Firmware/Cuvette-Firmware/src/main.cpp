/*========================================================================================

[ a n y m a ]
AETZBOX
Cuvette

© 2024 Michael Egger AT anyma.ch

==========================================================================================*/
//                                                                                      LIB
#include <Arduino.h>
#include <FastLED.h>
#include <U8g2lib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Logo.h"
#include <OTA.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager



//----------------------------------------------------------------------------------------
//                                                                                  CONFIG
#define HW_VERSION 3
#define MAIN_FILE_SYSTEM LittleFS

#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))
#define ALIGN_LEFT 0

#define NUM_CAPSENS 5
#define WATERLEVEL_CAP_THRESH 4

#if HW_VERSION == 2
#define NUM_BTNS 4
const int PIN_BTN[NUM_BTNS] = {18, 17, 47, 37};
const int PIN_CAP_SENS[NUM_CAPSENS] = {9, 10, 11, 12, 13};
#define PIN_SCL 21
#define PIN_SDA 14
#define PIN_TC1047 2
#define PIN_ONE_WIRE_BUS 20
#define PIN_PWM_HEAT 16
#define PIN_PWM_BUBBLES 38
#define PIN_PWM_AUX 48
#define PIN_PIX 15
#endif

#if HW_VERSION == 3
#define NUM_BTNS 4
const int PIN_BTN[NUM_BTNS] = {18, 17, 48, 37};
const int PIN_CAP_SENS[NUM_CAPSENS] = {9, 10, 11, 12, 13};
#define PIN_SCL 21
#define PIN_SDA 14
#define PIN_TC1047 -1
#define PIN_ONE_WIRE_BUS 8
#define PIN_PWM_HEAT 20
#define PIN_PWM_BUBBLES 47
#define PIN_PWM_AUX -1
#define PIN_PIX 15
#endif

#define LED_FREQ 5000
#define LED_RESOLUTION 8
#define LED_CHANN 0

#define BUBBLES_FREQ 50
#define BUBBLES_RESOLUTION 8
#define BUBBLES_CHANN 1

#define DISPLAY_MODE_IDLE 0
#define DISPLAY_MODE_INFO 1
#define DISPLAY_TIMEOUT 10000

#define NUM_PIXELS 48

#pragma message("Building with device name:" DEVICE_NAME)
//----------------------------------------------------------------------------------------
//                                                                                 GLOBALS

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
OneWire oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
CRGB pixel[NUM_PIXELS];

char message[128] = {0};
int display_mode;

int sens[NUM_CAPSENS];
int water_level;

float bubbles_target = 1.;
float bubbles_pwm = 0.5;

float led_target = 1.;
float led_pwm = 0.5;

float temperature_water_target = 46.;
float temperature_ambient, temperature_water;

//----------------------------------------------------------------------------------------

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void print_sensor_values()
{
  Telnet.printf("Waterlevel %d\n", water_level);
  Telnet.print("Water sensor values -  ");
  for (int i = 0; i < NUM_CAPSENS; i++)
  {
    Telnet.printf("%d - ", sens[i]);
  }
  Telnet.println();
}
//----------------------------------------------------------------------------------------

int get_water_level()
{
  int waterlevel = 0;
  for (int i = 0; i < NUM_CAPSENS; i++)
  {
    sens[i] = touchRead(PIN_CAP_SENS[i]);
  }

  int reference = sens[0];
  if (reference < 10000)
  {
    Telnet.println("Weird Capacitive reference");
    waterlevel = -1;
    return waterlevel;
  }

  if (sens[4] / reference > WATERLEVEL_CAP_THRESH)
  {
    waterlevel = 1;
    if (sens[3] / reference > WATERLEVEL_CAP_THRESH)
    {
      waterlevel = 2;
      if (sens[2] / reference > WATERLEVEL_CAP_THRESH)
      {
        waterlevel = 3;
        if (sens[1] / reference > WATERLEVEL_CAP_THRESH)
        {
          waterlevel = 4;
        }
      }
    }
  }
  return waterlevel;
}

//----------------------------------------------------------------------------------------
//																	Nice and warm Welcome
void intro()
{

  u8g2.clearBuffer();
  u8g2.drawBitmap(0, 24, 16, 16, bitmap_anyma);
  u8g2.sendBuffer();
  delay(500);
  u8g2.drawBitmap(32, 0, 8, 48, bitmap_box);
  u8g2.sendBuffer();
  delay(50);
  u8g2.clearBuffer();
  u8g2.drawBitmap(32, 0, 8, 48, bitmap_box);
  u8g2.setFont(u8g2_font_missingplanet_tf);
  u8g2.setCursor(46, 64);
  u8g2.sendBuffer();
  delay(400);
  u8g2.print("Mark II");

  u8g2.sendBuffer();
  for (int hue = 0; hue < 360; hue++)
  {
    fill_rainbow(pixel, NUM_PIXELS, hue, 7);
    delay(5);
    FastLED.show();
  }
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				                                           TASKS

void wifi_task(void *)
{

  Telnet.printf("Start WIFI Task on core %d", xPortGetCoreID());

  // Wifi Manager: Try to connect, create access point on failure
  WiFiManager wm;
  bool res;
  res = wm.autoConnect(DEVICE_NAME); // anonymous ap

  if (!res)
  {
    Telnet.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Telnet.println("connected...yeey :)");

    OTA::start(DEVICE_NAME);

    while (1)
    {
      OTA::handle();
      delay(200);
    }
  }
}

//----------------------------------------------------------------------------------------
//																				                                      Button Task
void button_task(void *)
{

  Telnet.printf("Start Button Task on core %d", xPortGetCoreID());

  int last_button_state[NUM_BTNS];
  int button_state;

  for (int i = 0; i < NUM_BTNS; i++)
  {
    pinMode(PIN_BTN[i], INPUT_PULLUP);
    last_button_state[i] = digitalRead(PIN_BTN[i]);
  }

  Telnet.println("Running Button task");

  while (1)
  {
    for (int i = 0; i < NUM_BTNS; i++)
    {
      button_state = digitalRead(PIN_BTN[i]);
      if (button_state != last_button_state[i])
      {
        last_button_state[i] = button_state;
        if (!button_state)
        {
          Telnet.printf("Button %d pressed\n", i);

          if (display_mode == DISPLAY_MODE_INFO)
          {
            display_mode = DISPLAY_MODE_IDLE;
          }
          else
          {
            display_mode = DISPLAY_MODE_INFO;
          }
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//----------------------------------------------------------------------------------------
//																				                                      DISPLAY

void display_normal()
{
  u8g2.setCursor(0, 26);
  u8g2.setDrawColor(1);

  if (strlen(message))
  {
    u8g2.setFont(u8g2_font_logisoso18_tf);
    u8g2.setCursor(ALIGN_CENTER("-ERROR-"), 32);

    u8g2.print("-ERROR-");
    u8g2.setFont(u8g2_font_missingplanet_tf);
    u8g2.setCursor(ALIGN_CENTER(message), 48);
    u8g2.print(message);
  }
  else
  {

    if (digitalRead(PIN_PWM_HEAT))
    {
      u8g2.drawBox(0, 0, 128, 64);
      u8g2.setDrawColor(0);
    }
    u8g2.setFont(u8g2_font_logisoso38_tf);
    sprintf(message, "%.1f", temperature_water);

    u8g2.setFont(u8g2_font_logisoso38_tf);
    u8g2.setCursor(ALIGN_CENTER(message), 54);
    u8g2.print(message);
    u8g2.setDrawColor(1);
  }
}
//----------------------------------------------------------------------------------------
void display_wifi()
{
  u8g2.setCursor(0, 18);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_missingplanet_tf);
  if (WiFi.status() == WL_CONNECTED)
  {
    u8g2.printf("%s\n", WiFi.SSID().c_str());
    u8g2.setCursor(0, 36);
    u8g2.printf("%s.local\n", DEVICE_NAME);
  }
  else
  {
    u8g2.print("Wifi Not Connected");
  }
  u8g2.setCursor(0, 48);

  u8g2.printf("%s\n", FIRMWARE_VERSION);
  
}

//----------------------------------------------------------------------------------------
void update_display(void * = NULL)
{
  static long last_display_normal;
  if (millis() - last_display_normal > DISPLAY_TIMEOUT)
  {
    display_mode = DISPLAY_MODE_IDLE;
  }

  u8g2.clearBuffer();
  if (display_mode == DISPLAY_MODE_IDLE)
  {
    display_normal();
    last_display_normal = millis();
  }
  else
  {
    display_wifi();
  }
  u8g2.sendBuffer();
}

//----------------------------------------------------------------------------------------
//																				                                      CUVETTE Task

void cuvette_task(void *)
{
  Telnet.printf("Start Cuvette Task on core %d", xPortGetCoreID());

  pinMode(PIN_PWM_HEAT, OUTPUT);
  pinMode(PIN_PWM_BUBBLES, OUTPUT);

  digitalWrite(PIN_PWM_HEAT, HIGH);
  digitalWrite(PIN_PWM_BUBBLES, HIGH);
  delay(200);
  digitalWrite(PIN_PWM_HEAT, LOW);
  digitalWrite(PIN_PWM_BUBBLES, LOW);

  while (1)
  {
    message[0] = 0;
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C)
    {
      temperature_water = tempC;
      if (temperature_water < 5.)
      {
        sprintf(message, "Water Freezing?");
      }

      if (temperature_water > 65.)
      {
        sprintf(message, "Water Too Hot!");
      }
    }
    else
    {
      sprintf(message, "Temp Sens Error");
      Telnet.println("Error: Could not read temperature data");
    }

    // check water level
    water_level = get_water_level();
    if (water_level < 0)
    {
      sprintf(message, "Water Sens Error");
    }
    else if (water_level < 2)
    {
      sprintf(message, "Water Low");
    }
    else if (water_level > 3)
    {
      sprintf(message, "Water Too High");
    }

    if (strlen(message))
    {
      Telnet.printf("Err: %d, %s\n", strlen(message), message);
    }

    if (strlen(message))
    {
      digitalWrite(PIN_PWM_HEAT, LOW);
      digitalWrite(PIN_PWM_BUBBLES, LOW);
      fill_solid(pixel, NUM_PIXELS, CRGB::OrangeRed);
    }
    else
    {
      fill_solid(pixel, NUM_PIXELS, CRGB::FloralWhite);

      if (temperature_water < temperature_water_target)
      {
        digitalWrite(PIN_PWM_HEAT, HIGH);
      }
      else
      {
        digitalWrite(PIN_PWM_HEAT, LOW);
      }
    }

    update_display();
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				                                           Setup

void setup()
{
  FastLED.addLeds<WS2813, PIN_PIX, RGB>(pixel, NUM_PIXELS);
  FastLED.setBrightness(255);
  fill_solid(pixel, NUM_PIXELS, CRGB::FloralWhite);
  FastLED.show();

  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));

  Telnet.println("________________________");
  Telnet.printf("RESET Reason: %d\n", esp_reset_reason());
  Telnet.println("Setup");
  fill_solid(pixel, NUM_PIXELS, CRGB::Black);
  FastLED.show();

  xTaskCreatePinnedToCore(wifi_task, "Wifi-Task", 8096, NULL, 0, NULL, ARDUINO_RUNNING_CORE ? 0 : 1);

  u8g2.begin();
  u8g2.setPowerSave(0);
  fill_solid(pixel, NUM_PIXELS, CRGB::FloralWhite);
  FastLED.show();
  intro();

  xTaskCreate(cuvette_task, "Cuvette-Task", 8096, NULL, 0, NULL);
  xTaskCreate(button_task, "Button-Task", 8096, NULL, 0, NULL);

  display_mode = DISPLAY_MODE_INFO;

  Telnet.println("Setup Done");
  Telnet.println("________________________");
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				Loop
void loop()
{
  print_sensor_values();
  delay(4000);
}
