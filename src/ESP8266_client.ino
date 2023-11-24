/**
   ESPNOW - Basic communication - Client
   Date: 26th September 2017
   Author: Arvind Ravulavaru <https://github.com/arvindr21>
   Purpose: ESPNow Communication between a Server ESP32 and multiple ESP32 Clients
   Description: This sketch consists of the code for the Client module.
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Client >>

   Flow: Server
   Step 1 : ESPNow Init on Server and set it in STA mode
   Step 2 : Start scanning for Client ESP32 (we have added a prefix of `Client` to the SSID of Client for an easy setup)
   Step 3 : Once found, add Client as peer
   Step 4 : Register for send callback
   Step 5 : Start Transmitting data from Server to Client(s)

   Flow: Client
   Step 1 : ESPNow Init on Client
   Step 2 : Update the SSID of Client with a prefix of `Client`
   Step 3 : Set Client in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor

   Note: Server and Client have been defined to easily understand the setup.
         Based on the ESPNOW API, there is no concept of Server and Client.
         Any devices can act as server or client.
*/

#include <espnow.h>
#include <ESP8266WiFi.h>


#define CHANNEL 1

//Seven segment pins attachecd with nodemcu pins  
int a = 0;  //Gpio-0 with a of 7 segment display   
int b = 1;  //Gpio-1 with b of 7 segment display    
int c = 2;  //Gpio-2 with c of 7 segment display  
int d = 3;  //Gpio-3 with d of 7 segment display  
int e = 4;  //Gpio-4 with e of 7 segment display   
int f = 5;  //Gpio-5 with f of 7 segment display  
int g = 16; //Gpio-16 with g of 7 segment display

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ERR_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  String Prefix = "Client:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

void setup() {
  analogWriteResolution(12);
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Client Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Client in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(D5, OUTPUT); // unidade
  pinMode(D6, OUTPUT); // dezena
//  pinMode(D7, OUTPUT); // centena
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
//  digitalWrite(D7, HIGH);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

int dez, uni;
// callback when data is recv from Server
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.print(data[0]); Serial.println(data[1]);
  dez = data[0];
  uni = data[1];
  Serial.println("");
}

void loop() {
  digitalWrite(D5, HIGH);
  digitalWrite(D6, LOW);
  handle7seg(uni);
  delay(10);
  digitalWrite(D5, LOW);
  digitalWrite(D6, HIGH);
  handle7seg(dez);
  delay(10);
}

void handle7seg(int num) {
  if (num == 0) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, HIGH);
  }
  else if (num == 1) {
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
  }
  else if (num == 2) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, HIGH);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
  }
  else if (num == 3) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
  }
  else if (num == 4) {
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
  }
  else if (num == 5) {
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
  }
  else if (num == 6) {
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
  }
  else if (num == 7) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
  }
  else if (num == 8) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
  }
  else if (num == 9) {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
  }
}