#include <String.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#include <ThingSpeak.h>


String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;

#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 2    // define 2 as data input 
DHT dht(dht_dpin, DHTTYPE); 
#define Light 12              // pin 12 to control light
#define fan 14                // pin 14 to control fan

char c,e;                       // stores the bytes coming from the server

float temp,h;                   //stores float value of temp and humididty
String humidity,temperature,line;    //stores string value of temp and humididty
int co=0;                       //control the sending of Alert

int p=0,q=0;        //these variables prevent switching ON/OFF of already turned ON/OFF devices

//char auth[] = "i75KbvY576QOo58Pel3X90AK08Q7udoq";
const char* ssid     = "OPPOReno";            //your SSID of WIFI
const char* password = "albenzapper";       // password of Wifi
//--------------------------------------------

String message="Hello from ESP8266!";
String wDeviceId = "v79FB952D0AE38AA";
String FCcapiKey = "HN60HWOSFV74HFHD";
String THapiKey = "OMKPPZ21KVNRQ388";
String DCreadapiKey = "MAY7XJO4YMGKCDA8";
unsigned long channelNumber = 867441;
const char* host = "api.thingspeak.com";
const char* logServer = "api.pushingbox.com";
const char* THWriteapiKey = "OMKPPZ21KVNRQ388";


WiFiClient client;
HTTPClient http; 

void setup() {
  dht.begin();
  

  Serial.begin(115200);
  Serial.println("Home Monitoring System");

  pinMode(Light,OUTPUT);
  pinMode(fan,OUTPUT);
  digitalWrite(Light,HIGH);
  digitalWrite(fan,HIGH);
  
  ThingSpeak.begin(client);
  
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
/* 
      if (client.connect(host, httpPort)) {

          Serial.println("Sending data to ThingSpeak");
          
          String postStr = "api_key="+GCBVI9QTZVLIN4M6+"&field1=1";
      
          client.println("POST /update HTTP/1.1");
          
          client.println("Host: api.thingspeak.com");
          
          client.println("Connection: close");
          
          client.println("Content-Type: application/x-www-form-urlencoded");
          
          client.print("Content-Length: ");
          
          client.println(postStr.length());
          
          client.println();
          
          client.println(postStr); 

      }

*/
void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  Serial.println("Requesting URL: ");
 //============================================================================================================
 //                                            Weather Station
 //============================================================================================================
  
  h = dht.readHumidity();                    //reads humidity and temperature
  temp = dht.readTemperature();
   Serial.print("Temperature: ");
   Serial.print(temp);
   Serial.print(" Degrees Celsius, Humidity: ");
   Serial.print(h);
   Serial.println("%. Sending to Thingspeak.");
  if(temp<50.0)
  {
    co=0;
  }
  if(temp>=50.0&&co==0)      // send alert if temperature is above 50 degrees and co is used to avoid multiple alert sending
  {
    Serial.println("Sending Alert please wait.......");
    delay(1000);
      Serial.println("Connecting to pushing server: " + String(logServer));
      if (client.connect(logServer, 80)) {
        Serial.println("Succesfully connected");
        
        String postStr = "devid=";
        postStr += String(wDeviceId);
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
      
      if (client.connect(host, httpPort)) {

     //   Serial.println("Sending data to ThingSpeak");
          
          String postStr = "api_key="+FCcapiKey+"&field1=1";
      
          client.println("POST /update HTTP/1.1");
          
          client.println("Host: api.thingspeak.com");
          
          client.println("Connection: close");
          
          client.println("Content-Type: application/x-www-form-urlencoded");
          
          client.print("Content-Length: ");
          
          client.println(postStr.length());
          
          client.println();
          
          client.println(postStr); 

      }

  // client.stop();
  // Serial.println("Stopping the client");

  co=1;
  }
  
  //humidity=String(h);
  //temperature=String(temp); 
       if (client.connect(host, httpPort)) {

          Serial.println("Sending data to ThingSpeak");
          
          String postStr = "api_key="+THapiKey+"&field1="+String(temp)+"&field2="+String(h);
      
          client.println("POST /update HTTP/1.1");
          
          client.println("Host: api.thingspeak.com");
          
          client.println("Connection: close");
          
          client.println("Content-Type: application/x-www-form-urlencoded");
          
          client.print("Content-Length: ");
          
          client.println(postStr.length());
          
          client.println();
          
          client.println(postStr); 

      }
           delay(1000);     


  if (client.connect("api.thingspeak.com",httpPort)) {
      // int sensorValue = analogRead(A0);
      // Convert the analog reading 
      // On Uno,Mega,YunArduino:  0 - 1023 maps to 0 - 5 volts
      // On ESP8266:  0 - 1023 maps to 0 - 1 volts
      // On MKR1000,Due: 0 - 4095 maps to 0 - 3.3 volts
        
     // float voltage1 = round((1/(log((float)(1023-temp)*10000/temp)/10000)/3975+1/298.15)-273.15);
     // ThingSpeak.writeField(channelNumber, 1, voltage1, THWriteapiKey);
     // delay(50);
     // float voltage2 = round((1/(log((float)(1023-h)*10000/h)/10000)/3975+1/298.15)-273.15);
     // ThingSpeak.writeField(channelNumber, 2, voltage2, THWriteapiKey);
                // Update the 2 ThingSpeak fields with the new data
                //ThingSpeak.setField(1, (float)temperature);
                //ThingSpeak.setField(2, (float)humidity);
                
                // Write the fields that you've set all at once.
                //ThingSpeak.writeFields(channelNumber, THWriteapiKey);
   
  //      request_string = thingSpeakAddress;
  //      request_string += "key=";
  //      request_string += "OMKPPZ21KVNRQ388";
  //      request_string += "&";
  //      request_string += "field2";
  //   
  //      request_string += "=";
  //      request_string += round((1/(log((float)(1023-analogRead(A0))*10000/analogRead(A0))/10000)/3975+1/298.15)-273.15);
  //      http.begin(request_string);
  //      http.GET();
  //      http.end();
         Serial.println("Data has Been Uploaded");
  
      }
      delay(300);
/*
      Serial.println("Received Light Status is :  ");
      Serial.println((ThingSpeak.readIntField(867432,1,"X24ZKK22LJ61DTMH")));
      int s=ThingSpeak.readIntField(867432,1,"X24ZKK22LJ61DTMH");
      Serial.println(s);
     
      if (s==-1)
      {
        digitalWrite(RL1, LOW);
        digitalWrite(LED1, HIGH);
        Serial.println("Turning On Light");
        delay(1000);   
        digitalWrite(LED1, LOW);
        Serial.println("Turning On Light");
       }
            else 
            {
                digitalWrite(RL1, HIGH);
                Serial.println("Turning Off Light");
            }
      
      
      Serial.println("Received Fan Status is :  ");
      Serial.println((ThingSpeak.readIntField(874413,1,"OVDYYFK60V4S1M6W")));
      s=ThingSpeak.readIntField(874413,1,"OVDYYFK60V4S1M6W");
      
      if (s==-1)
      {
        digitalWrite(RL2, LOW);
        digitalWrite(LED1, HIGH);
        Serial.println("Turning On Fan");
        delay(1000);   
        digitalWrite(LED1, LOW);
     }
             else 
             {
                  digitalWrite(RL2, HIGH);
                  Serial.println("Turning Off Fan");
             }
}

*/
//============================================================================================================
//                                         End of Weather Station
//============================================================================================================


 //============================================================================================================
 //                                        Device Control
 //============================================================================================================

 // Performing required operations based on the value of channel
    //-------------------------------------------------------------
    //                        LIGHT
    //-------------------------------------------------------------

     Serial.println("Received Light Status is :  ");
     //Serial.println((ThingSpeak.readIntField(867432,1,"X24ZKK22LJ61DTMH")));
     int s=ThingSpeak.readIntField(867432,1,"X24ZKK22LJ61DTMH");
     Serial.println(s);  
    if(s==1&&p==0)         //Light ON            
    {
      digitalWrite(Light,LOW);
      p=1;
    }
     if(s==0&&p==1)        //Light off
    {
      digitalWrite(Light,HIGH);
      p=0;
    }
    
    //-------------------------------------------------------------
    //                         FAN
    //-------------------------------------------------------------
    Serial.println("Received Fan Status is :  ");
    //Serial.println((ThingSpeak.readIntField(874413,1,"OVDYYFK60V4S1M6W")));
    s=ThingSpeak.readIntField(874413,1,"OVDYYFK60V4S1M6W");
    Serial.println(s);
    if(s==1&&q==0)
    {
      digitalWrite(fan,LOW);
      q=1;
    }
    if(s==0&&q==1)
    {
      digitalWrite(fan,HIGH);
      q=0;
    }
  }
