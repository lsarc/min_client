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
}

// callback when data is recv from Server
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.print(data[0]); Serial.println(data[1]);
  Serial.println("");
}

void loop() {
  // Chill
}


/* // Setup the client to send sensor data to the server
#include <ESP8266WiFi.h>
#include <Keypad.h>



byte pinosLinhas[4]  = {D0, D1, D2, D3};

byte pinosColunas[4] = {D4, D5, D6, D7};

char teclas[4][4] = {{'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};

Keypad teclado1 = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, 4, 4);  

// Initializes the variable "senha"
int senha, senha_nova;

// Network parameters
const char *ssid = "NodeMCUServer_ESP8266";
const char *password = "triedro7seg";
const char *host = "192.168.11.4";

// Setup the client object
WiFiClient client;

// Configure deep sleep in between measurements
const int sleepTimeSeconds = 2;


char tecla;
int senha_digitada, cent, dez, uni, ok;

void setup() {
  // Initializes the variable "senha"
  senha = -1;
  senha_nova = 0;

  // Configure the sensor pin 

  // Connect to the server
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("CONECTADO!");
  ESP.wdtDisable();
}

int atoi(char tecla) {
  int num = tecla - '0';
  return num;
}

int isNumber(char tecla) {
  if (atoi(tecla) >= 0 && atoi(tecla) <= 9)
    return 1;
  return 0;
}
  
void loop() {
  Serial.println("Nova senha: ");
//  while (Serial.available() == 0) {}
//  senha_nova = Serial.parseInt();

  tecla = '*';
  ok = 0;
  
  while (tecla != 'A') {
    if (ok == 0) {
      while (!isNumber(tecla)) {
        tecla = teclado1.getKey();
        yield();
        ESP.wdtFeed();
      }
      dez = atoi(tecla);
      Serial.println(tecla);
      tecla = '*';
  
      while (!isNumber(tecla)) {
        tecla = teclado1.getKey();
        yield();
        ESP.wdtFeed();
      }
      uni = atoi(tecla);
      Serial.println(tecla);
      tecla = '0';
      ok = 1;
    }

    //tecla = NULL;
    while (tecla != 'A' && tecla != 'C') {
       tecla = teclado1.getKey();
       yield();
       ESP.wdtFeed();
       if (tecla) {
         Serial.print("Loop = ");
         Serial.println(tecla);
       }
    }
    Serial.println(tecla);
    Serial.println("OK = ");
    Serial.print(ok);
    
//    if (tecla != 'A')
//      ok = 0;
  }
  Serial.println("ALOOO");
  senha_nova = 10 * dez + uni;
  //ok = 1;

  Serial.println(senha_nova);
  
  if (ok) {
    Serial.print("IP Address (access point): ");
    Serial.println(WiFi.localIP());
    senha = senha_nova;
    Serial.println("Senha: ");
    Serial.print(senha);
    Serial.println();
    Serial.println();
    // Get a measurement from the sensor
    senha = Serial.parseInt();
    

    // Connect to the server and send the data as a URL parameter
    if (client.connect(host, 80)) {
      String url = "/update?value=";
      url += String(senha);

      // Minunum set of required URL headers
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: keep-alive\r\n\r\n");
      delay(10);
      // Read all the lines of the response and print them to Serial
      Serial.println("Response: ");
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
    }
    Serial.println("---------------------------------------");
    ESP.wdtFeed();
    Serial.println("ESP8266 in sleep mode");
    ESP.deepSleep(sleepTimeSeconds * 1e6);
  } 
}
 */