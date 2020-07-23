/*
 *  This sketch is implementation of Smart Bell and Home Security
 *
 */
//#include<WiFi.h>
#include <ESP8266WiFi.h>
#include<String.h>
#include "ThingSpeak.h"

//------------------------------------------
//       Variables
//------------------------------------------
int intruder=0,guest=4;                     // pin 0 as intruder alert and pin 4 as guest alert
const char* ssid     = "OPPOReno";          //your SSID of WIFI
const char* password = "albenzapper";       // password of Wifi

const char* logServer = "api.pushingbox.com";
const char* host = "api.thingspeak.com";
String message="Hello from ESP8266!";
String IapiKey = "ZYY6DQIWOY78DWTH";
String GapiKey = "BREDXJ0LT15LEUUG";
String intDeviceId = "v3DFDF8A1928E1E6";
String gueDeviceId = "v68628102ABCCD87";
WiFiClient client;
//--------------------------------------------


void setup() {
  Serial.begin(115200);
  ThingSpeak.begin(client);
  pinMode(guest,INPUT);
  delay(10);

  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
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


int value = 0;
int x,y=0,z=0,k=0;
void loop() {
  delay(1000);
  ++value;

  Serial.print("Connecting to ");
  Serial.println(host);
  
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  
 //============================================================================================================
 //                                              Intruder alert
 //============================================================================================================
 x=digitalRead(intruder);
 Serial.println(x);
  if(x==0)
  {
    y=0;
  }
  if(x==1&&y==0)
  {
    Serial.println("Sending Alert Please Wait.....");
    delay(1000);
      if (client.connect(host, httpPort)) {

          Serial.println("Sending data to ThingSpeak");
          
          String postStr = "api_key="+IapiKey+"&field1=1";
      
          client.println("POST /update HTTP/1.1");
          
          client.println("Host: api.thingspeak.com");
          
          client.println("Connection: close");
          
          client.println("Content-Type: application/x-www-form-urlencoded");
          
          client.print("Content-Length: ");
          
          client.println(postStr.length());
          
          client.println();
          
          client.println(postStr); 
      }

      Serial.println("Connecting to pushing server: " + String(logServer));
      if (client.connect(logServer, 80)) {
        Serial.println("Succesfully connected");
        
        String postStr = "devid=";
        postStr += String(intDeviceId);
        postStr += "&message_parameter=";
        postStr += String(message);
        postStr += "\r\n\r\n";
        
        Serial.println("Sending data...");
        
        client.print("POST /pushingbox HTTP/1.1\n");
        client.print("Host: api.pushingbox.com\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
      }

  // cclient.stop();
  // Serial.println("Stopping the client");


/*   
  String host1="api.pushingbox.com";
    cclient.print(String("GET ")  + "/pushingbox?devid=v9D6514BD029DD30 HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");                       // executing pushing box api
    cclient.print(String("GET ")  + "/update?api_key=ZYY6DQIWOY78DWTH&field1=1 HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                        //updating intruder notification
               
*/   
    delay(1000);
    y=1;
  } 

//============================================================================================================
//                                         End of Intruder alert
//============================================================================================================
//============================================================================================================
//                                             Guest alert
//============================================================================================================
 long t1;
 z=digitalRead(guest);
 Serial.println(z);
  if(z==0 /* && (millis()>=t1+300000)*/) // wait for 5 min to avoid multiple alert sending due to multiple door bell press
  {
    k=0;
  }
  if(z== HIGH &&k==0)
  {
    //t1=millis();         // storing curent time stamp in t1
    Serial.println("Sending Alert Please Wait.....");
    //delay(1000);                      //delay for sending alert and switch debouncing
          if (client.connect(host, httpPort)) {

          Serial.println("Sending data to ThingSpeak");
          
          String postStr = "api_key="+GapiKey+"&field1=1";
      
          client.println("POST /update HTTP/1.1");
          
          client.println("Host: api.thingspeak.com");
          
          client.println("Connection: close");
          
          client.println("Content-Type: application/x-www-form-urlencoded");
          
          client.print("Content-Length: ");
          
          client.println(postStr.length());
          
          client.println();
          
          client.println(postStr); 
      }

      Serial.println("Connecting to pushing server: " + String(logServer));
      if (client.connect(logServer, 80)) {
        Serial.println("Succesfully connected");
        
        String postStr = "devid=";
        postStr += String(gueDeviceId);
        postStr += "&message_parameter=";
        postStr += String(message);
        postStr += "\r\n\r\n";
        
        Serial.println("Sending data...");
        
        client.print("POST /pushingbox HTTP/1.1\n");
        client.print("Host: api.pushingbox.com\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
      }
    
/*  String host1="api.pushingbox.com";
    client.print(String("GET ")  + "/pushingbox?devid=vEDE436603F64B15 HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");                       // executing pushing box api
    client.print(String("GET ")  + "/update?api_key=BREDXJ0LT15LEUUG&field1=1 HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");                        //updating guest notification
 */               
    delay(1000);
    k=1;
  } 

//============================================================================================================
//                                          End of Guest alert
//============================================================================================================

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  //Serial.println("Closing connection");
}
