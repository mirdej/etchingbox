#include "AsyncTelnet.h"


AsyncTelnet::AsyncTelnet(uint16_t port) : server(port) {
    server_port = port;
}

bool AsyncTelnet::begin(bool checkConnection /* = true */, bool mDNS) {
    if (checkConnection) {
        // connected to WiFi or is ESP in AP mode?
        if (WiFi.status() != WL_CONNECTED) return false;
        if (mDNS) {
            if (!MDNS.begin("esp32")) {
                Serial.println("Error setting up MDNS responder!");
                while (1) {
                    delay(1000);
                }
            }
            MDNS.addService("http", "tcp", 80);
        }
    }
    server.onClient([=](void *, AsyncClient *c) {
        if (c == NULL) return;
        client = c;
        // client->setRxTimeout(10);
        ip = client->remoteIP();
        if (on_connect != NULL) on_connect(NULL, client);
        c->onDisconnect([=](void *, AsyncClient *c) {
            if (on_disconnect != NULL) on_disconnect(c);
            c->close(true);
            c->free();
            delete c;
        }, this);
        if (on_incoming_data != NULL) {
            c->onData([=](void *, AsyncClient *, void *data, size_t len) {
                for (size_t i = 0; i < len; i++) {
                    char incoming = ((const char *) data)[i];

                    if (incoming == '\b' || incoming == 0x7F) { // Handle backspace
                        if (!buffer.empty()) {
                            buffer.pop_back(); // Remove the last character from the buffer
                            write("\b \b");    // Provide visual feedback (backspace + clear + backspace)
                        }
                    } else if (incoming == '\n') { // Handle newline
                        if (!buffer.empty() && buffer.back() == '\r') {
                            buffer.pop_back(); // Remove preceding \r if present
                        }
                        write("\r\n"); // Provide newline feedback
                        if (!buffer.empty()) { // Only process non-empty input
                            on_incoming_data(buffer); // Process the complete input
                        }
                        buffer.clear(); // Clear the buffer for the next input
                    } else if (incoming == '\r') {
                        // Ignore bare carriage returns, processed along with '\n' if present
                    } else {
                        buffer += incoming; // Append other characters to the buffer
                    }
                }
            }, this);
        }
        client->setNoDelay(true);
    }, this);
    server.setNoDelay(true);
    server.begin();
    return true;
}

void AsyncTelnet::stop() {
    server.end();
}

bool AsyncTelnet::isClientConnected(AsyncClient *Client) {
    return (Client->connected());
}

void AsyncTelnet::disconnectClient() {
    client->stop();
    isConnected = false;
}

size_t AsyncTelnet::write(const char *data) {
    if (data == NULL) {
        return 0;
    }
    return client->write(data, strlen(data));
}

size_t AsyncTelnet::write(const char *data, size_t size, uint8_t apiflags) {
    size_t will_send = -client->add(data, size, apiflags);
    if (!will_send || !client->send()) {
        return 0;
    }
    return will_send;
}

void AsyncTelnet::onConnect(ConnHandler callbackFunc) {
    on_connect = callbackFunc;
}

void AsyncTelnet::onDisconnect(DisconnHandler callbackFunc) {
    on_disconnect = callbackFunc;
}

void AsyncTelnet::onIncomingData(IncomingDataHandler callbackFunc) {
    on_incoming_data = callbackFunc;
}
