#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid     = "winter_stark"; // you have to change this setting according to yours 
const char* password = "12345678#"; // you have to change this setting according to yours

const char* host = "api.pushbullet.com";
//https://api.pushbullet.com/v2/pushes
const int httpsPort = 443;

const int gpio_pir_sensor = 16 ;  // GPIO 16

const char* accessToken = "your__token_just replace_it"; // your private token from pushbullet 
const char* fingerprint = "your__key_just replace_it"; // thumbprint from https://api.pushbullet.com/

void setup() {
  
  Serial.begin(115200);
  delay(10);
  pinMode(gpio_pir_sensor, INPUT);
  
  // We start by connecting to a WiFi network with active internet connetion

  Serial.println();
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}
void sendSignalAlert()
{     
      Serial.println("Motion detected ! sending alert request...");
      String url = "/v2/pushes";
      
      WiFiClientSecure client;  // Use WiFiClientSecure class to create TLS connection
      client.setInsecure();

      //Serial.print("connecting to ");
      //Serial.println(host);
      
      if (!client.connect(host, httpsPort)) {
           Serial.println("connection failed");
            return;
      }

      //Serial.print("requesting URL: ");
      //Serial.println(url);
   
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Access-Token: " + accessToken + "\r\n" +
               "Content-length: 62\r\n"
               "Content-Type: application/json\r\n" +
               "Connection: close\r\n\r\n" +
               "{\"body\":\"Someone is nearby !!!\",\"title\":\"Alert\",\"type\":\"note\"}"
              );

      Serial.println("alert request sent !");
}

void loop() 
{
  if(digitalRead(gpio_pir_sensor)==HIGH)
  {
    sendSignalAlert();
    delay(5000);
  }
}
