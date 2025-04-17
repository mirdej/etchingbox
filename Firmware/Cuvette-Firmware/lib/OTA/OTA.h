#ifndef OTA_H
#define OTA_H

#ifdef ESP32
#include <WiFi.h>
#define DEFAULT_PORT 3232
#elif defined(ESP8266)
#define DEFAULT_PORT 8266
#include <string>
#endif
#include <ArduinoOTA.h>
#include <utility>
#include <vector>
#include <numeric>
#include "WirelessSerial.h"

static std::function<void()> disableInterruptsCallback = nullptr;

class OTA {
public:
    static void setupWiFi(const char *ssid, const char *pass) {
        Telnet.beginLocally();

        Telnet.println("\nConnecting to Wi-Fi...");
        Telnet.println(std::string("SSID: ") + ssid);
        Telnet.println(std::string("Password: ") + pass);

        WiFi.begin(ssid, pass);
        WiFi.setAutoReconnect(true);
        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
            delay(5000);
            Telnet.println("Failed to connect to Wi-Fi");
            ESP.restart();
        }
    }

    static void start(const char *hostname,
                      const char *password = "",
                      unsigned int port = DEFAULT_PORT) {
        Telnet.begin();

        ArduinoOTA.setPort(port);
        ArduinoOTA.setHostname(hostname);
        ArduinoOTA.setPassword(password);

        ArduinoOTA.onStart([]() {
            Telnet.println("Start updating...");
            if (disableInterruptsCallback == nullptr) {
                Telnet.println("\nDisabling all pins to prevent interrupts");
                disableInterruptsAllPins();
            } else {
                Telnet.println("\nRunning custom callback to disable interrupts");
                disableInterruptsCallback();
            }
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Telnet.println("Start updating " + type);
        });
        ArduinoOTA.onEnd([]() {
            Telnet.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Telnet.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Telnet.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Telnet.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Telnet.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Telnet.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Telnet.println("Receive Failed");
            else if (error == OTA_END_ERROR) Telnet.println("End Failed");

            Telnet.println("Rebooting...");
            delay(100);
            ESP.restart();
        });

        ArduinoOTA.begin();

        Telnet.println("Ready");
        Telnet.print("IP address: ");
        Telnet.println(WiFi.localIP().toString());
    }

    static void setDisableInterruptsCallback(std::function<void()> _disableInterruptsCallback) {
        disableInterruptsCallback = std::move(_disableInterruptsCallback);
    }

    static void setDisableInterruptsCallback(const std::vector<int> &_pins) {
        disableInterruptsCallback = std::bind(disableInterruptsDefaultCallback, _pins);
    }

    static void handle() {
        ArduinoOTA.handle();
    }

private:
    static void disableInterruptsAllPins() {
        std::vector<int> pins(40);
        std::iota(pins.begin(), pins.end(), 0);
        disableInterruptsDefaultCallback(pins);
    }

    static void disableInterruptsDefaultCallback(const std::vector<int> &pins) {
        for (int pin: pins) {
            Telnet.println("Disabling pin " + String(pin));
            detachInterrupt(pin);
        }
    }
};

#endif //OTA_H