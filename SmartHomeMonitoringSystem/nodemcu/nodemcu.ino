#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ThingSpeak.h>
WiFiClient client;
String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;
HTTPClient http;

String deviceId = "v3E41C57C497845A";
const char* logServer = "api.pushingbox.com";
String message="Hello World from ESP8266!";
int co=0;

#include<DHT.h>
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
String apiKey = "VQ19FQ88QL5YBDFD";     //  Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";
float h,t;

const int RL1 = 12;   
const int RL2 = 14;   
const int LED1 = 5;   
const int LED2 = 16;   

unsigned long myChannelNumber = 599768;
const char * myWriteAPIKey = "VQ19FQ88QL5YBDFD";

static boolean data_state = false; 

void setup()
{
    Serial.begin(115200);
    dht.begin();
    Serial.println("HOME MONITORING SYSTEM");
    
    pinMode(RL1, OUTPUT);
    pinMode(RL2, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    digitalWrite(RL1, HIGH);
    digitalWrite(RL2, HIGH);

    ThingSpeak.begin(client);

    WiFi.disconnect();
    WiFi.begin("netgear","rachana123");
    Serial.println("Connected to Network");
    while ((!(WiFi.status() == WL_CONNECTED)))
      {
      delay(300);
      }

}


void loop()
{
     //float h = 55;
     // float t = 24;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
                    if(t>50.0 && co==0)
  {
     
  if (client.connect(logServer,80))
  {
    Serial.println("- succesfully connected");
    
    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";
    
    Serial.println("- sending data...");
    
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
  //Serial.println("- stopping the client");
  delay(1000);
    co=1; 
}
            
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                        }

                            delay(1000);




        
    if (client.connect("api.thingspeak.com",80)) {
    //   int sensorValue = analogRead(A0);
    // Convert the analog reading 
    // On Uno,Mega,YunArduino:  0 - 1023 maps to 0 - 5 volts
    // On ESP8266:  0 - 1023 maps to 0 - 1 volts
    // On MKR1000,Due: 0 - 4095 maps to 0 - 3.3 volts
      
      float voltage = round((1/(log((float)(1023-analogRead(A0))*10000/analogRead(A0))/10000)/3975+1/298.15)-273.15);
      ThingSpeak.writeField(myChannelNumber, 1, voltage, myWriteAPIKey);
      delay(50);
      float voltage2 = round((1/(log((float)(1023-analogRead(A0))*10000/analogRead(A0))/10000)/3975+1/298.15)-273.15);
      ThingSpeak.writeField(myChannelNumber, 2, voltage2, mvyWriteAPIKey);

 
//      request_string = thingSpeakAddress;
//      request_string += "key=";
//      request_string += "VQ19FQ88QL5YBDFD";
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
    Serial.println("Received Light_status is :  ");
    Serial.println((ThingSpeak.readIntField(599358,1,"LXUP889BX8W2TP0F")));
    int s=ThingSpeak.readIntField(599358,1,"LXUP889BX8W2TP0F");
    Serial.println(s);
   
    if (s==1)
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
    
    
    Serial.println("Received Fan_status is :  ");
    Serial.println((ThingSpeak.readIntField(707195,1,"SRBLCCUZ37BNT39Z")));
    
     s=ThingSpeak.readIntField(707195,1,"SRBLCCUZ37BNT39Z");
    
    if (s==1)
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
