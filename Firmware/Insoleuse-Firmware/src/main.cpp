/*========================================================================================

[ a n y m a ]
AETZBOX
Insoleuse


© 2024 Michael Egger AT anyma.ch

==========================================================================================*/
//                                                                                      LIB
#include <Arduino.h>
#include <FastLED.h>
#include <U8g2lib.h>

#include "Logo.h"
#include <OTA.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "Preferences.h"

//----------------------------------------------------------------------------------------
//                                                                                  CONFIG

#define HW_VERSION 3

#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))
#define ALIGN_LEFT 0

#define MAIN_FILE_SYSTEM LittleFS
#define NUM_CAPSENS 5

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
const int PIN_BTN[NUM_BTNS] = {17, 18, 48, 37};
const int PIN_CAP_SENS[NUM_CAPSENS] = {9, 10, 11, 12, 13};
#define PIN_SCL 21
#define PIN_SDA 14
#define PIN_TC1047 -1
#define PIN_ONE_WIRE_BUS 8
#define PIN_PWM_HEAT 20
#define PIN_UV_LEDS 47
#define PIN_PWM_AUX -1
#define PIN_PIX 15
#endif

#define NUM_PIXELS 1
#define BTN_MODIFY 0
#define BTN_CANCEL 1
#define BTN_DOWN 2
#define BTN_UP 3

enum State
{
  FRESH,
  RUN,
  DONE,
  MODIFY

};

int exposure_target_seconds = 120;
int exposure_seconds = 0;
long exposure_start_millis;
long exposure_stop_millis;
State state = FRESH;

#pragma message("Building with device name:" DEVICE_NAME)

//----------------------------------------------------------------------------------------
//                                                                                 GLOBALS

CRGB pixel[NUM_PIXELS];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
Preferences preferences;

//----------------------------------------------------------------------------------------

void startExposure()
{
  state = RUN;
  Telnet.println("Exposure start");

  exposure_start_millis = millis();
  digitalWrite(PIN_UV_LEDS, HIGH);
}

//----------------------------------------------------------------------------------------

void stopExposure()
{
  state = DONE;
  Telnet.println("Exposure done");
  exposure_stop_millis = millis();
  digitalWrite(PIN_UV_LEDS, LOW);
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

  delay(600);
  u8g2.clearBuffer();
  u8g2.drawBitmap(32, 0, 8, 48, bitmap_box);
  u8g2.setFont(u8g2_font_missingplanet_tf);
  u8g2.setCursor(38, 64);
  u8g2.print("INSOLEUSE");
  u8g2.sendBuffer();
}

void display_status(int time)
{

  u8g2.clearBuffer();
  u8g2.setCursor(0, 12);
  //  u8g2.setFont(u8g2_font_amstrad_cpc_extended_8r);
  u8g2.setFont(u8g2_font_missingplanet_tf);

  u8g2.print("MODIFY");
  u8g2.setCursor(100, 12);
  u8g2.setCursor(0, 64);
  u8g2.print("CANCEL");
  //u8g2.setCursor(48, 64);
  //u8g2.setCursor(100, 64);
  //u8g2.print("OFF");

  u8g2.setFont(u8g2_font_logisoso38_tf);

  char buf[128];
  sprintf(buf, "%ds", time);
  u8g2.setCursor(ALIGN_CENTER(buf), 54);
  u8g2.print(buf);

  u8g2.sendBuffer();
}

//----------------------------------------------------------------------------------------

void displayWaiting()
{
  int lastrun = (exposure_stop_millis - exposure_start_millis) / 1000;
  u8g2.clearBuffer();
  u8g2.drawBitmap(32, 0, 8, 48, bitmap_box);
  u8g2.setFont(u8g2_font_missingplanet_tf);

  char buf[128];
  sprintf(buf, "Runtime %d s", lastrun);
  u8g2.setCursor(ALIGN_CENTER(buf), 64);

  u8g2.printf(buf);
  u8g2.sendBuffer();
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
  static int last_exposure_time;
  int last_button_state[NUM_BTNS];
  int button_state;

  for (int i = 0; i < NUM_BTNS; i++)
  {
    pinMode(PIN_BTN[i], INPUT_PULLUP);
    last_button_state[i] = digitalRead(PIN_BTN[i]);
  }

  log_v("Running Button task");

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
          if (i == BTN_MODIFY)
          {
            if (state == MODIFY)
            {
              preferences.putInt("time", exposure_target_seconds);
              state = DONE;
            }
            else
            {
              last_exposure_time = exposure_target_seconds;
              state = MODIFY;
            }
          }

          else
          {
            if (state != MODIFY)
            {
              if (state != RUN)
              {
                startExposure();
              }

              else if (state == RUN)
              {
                if (i == BTN_CANCEL)
                {
                  stopExposure();
                }
              }
            }
            else
            {
              if (i == BTN_CANCEL)
              {
                exposure_target_seconds = last_exposure_time;
                state = DONE;
              }
              else if (i == BTN_DOWN)
              {
                exposure_target_seconds -= 10;
                if (exposure_target_seconds < 15)
                  exposure_target_seconds = 10;
              }
              else if (i == BTN_UP)
              {
                exposure_target_seconds += 10;
              }
            }
          }
          Telnet.printf("Button %d pressed", i);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//========================================================================================
//----------------------------------------------------------------------------------------
//																				                                           Setup

void setup()
{
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));

  log_v("________________________");
  log_v("Setup");

  pinMode(PIN_UV_LEDS, OUTPUT);

  xTaskCreatePinnedToCore(wifi_task, "Wifi-Task", 8096, NULL, 0, NULL, ARDUINO_RUNNING_CORE ? 0 : 1);

  u8g2.begin();
  u8g2.setPowerSave(0);
  intro();

  preferences.begin("anyma");
  exposure_target_seconds = preferences.getInt("time", 120);

  xTaskCreate(button_task, "BuTTON Task", 8192, NULL, 1, NULL);

  log_v("Setup Done");
  log_v("________________________");
}
//========================================================================================
//----------------------------------------------------------------------------------------
//																				Loop

void loop()
{

  int runtime = 0;
  if (state == RUN)
  {
    runtime = (millis() - exposure_start_millis) / 1000;
    if (runtime >= exposure_target_seconds)
    {
      stopExposure();
    }
  }

  switch (state)
  {
  case FRESH:
    break;
  case RUN:
    display_status(exposure_target_seconds - runtime);
    break;
  case MODIFY:
    display_status(exposure_target_seconds);
    break;
  case DONE:
    displayWaiting();
    break;
  default:
    break;
  }
  vTaskDelay(pdMS_TO_TICKS(200));
}