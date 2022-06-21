#include <ESP8266WiFi.h>
#include "DHT.h"

// replace with your WiFi credentials
const char* ssid = "xxxxxxx";
const char* password = "xxxxxx";
WiFiServer server(80);

#define DHTTYPE DHT11 
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);


static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];


void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}


void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
          
            float h = dht.readHumidity();
            float t = dht.readTemperature();
            
            // Check if any reads failed and exit
            if (isnan(h) || isnan(t)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);                  
              dtostrf(h, 6, 2, humidityTemp);
            }
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature: ");
            client.println(celsiusTemp);
            client.println("&deg;C</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          blank_line = true;
        }
        else if (c != '\r') {
          blank_line = false;
        }
      }
    }  
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   
