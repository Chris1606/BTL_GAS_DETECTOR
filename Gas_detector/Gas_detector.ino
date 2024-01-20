
//Change Blynk Authenticaation Details
#define BLYNK_TEMPLATE_ID "TMPL6lGa09_ux"
#define BLYNK_TEMPLATE_NAME "MQ2 SENSOR"
#define BLYNK_AUTH_TOKEN "1Ve1KRuGAzvxneVsn_FWXkKtILPNduMv"
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>



char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "SnappyS";  
char pass[] = "20050103";  

int data = 0;
#define GAS_SENSOR 33
#define MOSFET 19
#define LED 21



BlynkTimer timer;
int read_sensor(){
     data = analogRead(GAS_SENSOR);
     return data; 
}

void blink_led(){
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
void sendSensor(){
 
  int data = read_sensor();
  Blynk.virtualWrite(V1, data);
  Serial.print("Pin A0: ");
  Serial.println(data);


  if(data > 1500)     // Change the Trashold value
  {
    Blynk.logEvent("gas_alert","Gas Leakage Detected");
  }
}

void setup(){
  pinMode(MOSFET, OUTPUT);
  pinMode(GAS_SENSOR, INPUT);
 
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  //dht.begin();
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  int flag = read_sensor();
  if ( flag > 2000 ){

     digitalWrite(MOSFET, HIGH);   
  }
     Blynk.run();
     timer.run();

}
