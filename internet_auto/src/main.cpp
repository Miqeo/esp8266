#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <EEPROM.h>
#include <eeprom_setting.h>



const byte DNS_PORT = 53;

const char *ssid = "test";
const char *password = "password";

bool broadcastOpen = false;

Credentails credToSend;

ESP8266WebServer server(80);
WiFiClient client;
DNSServer dnsServer;


//blinking
unsigned long previousMillis = 0;
const long interval = 500; 
int ledState = LOW;

void wifiConnect(String name, String passwd);

// String writeString(String cred[2]){
//   String credString = "";
//   for (int i = 0; i <2; i++) {
//     credString += cred[i];
//     credString += ";";
//   }

//   return credString;
// }

// void clearEeprom(){
//   EEPROM.begin(512);
  
//   for (int i = 0; i < 512; i++) {
//     EEPROM.write(i, 0);
//   }
//   EEPROM.end();
//   Serial.println("EEPROM cleared");
//   delay(500);
// }

// void saveCredentials(String SSID, String PASSWORD){
//   clearEeprom();
//   EEPROM.begin(512);
  
//   String cred[2] = {SSID,PASSWORD};
//   String toSave = writeString(cred);
//   Serial.print("Saved to EEPROM ");
//   Serial.print(toSave);
//   Serial.println("");
//   for(int i=0;i<toSave.length();i++){
//     EEPROM.write(0x0F+i, toSave[i]);
//   }
  
//   EEPROM.commit();
//   EEPROM.end();
// }

// Credentails retriveCredentials(){
//   Credentails cred;

//   EEPROM.begin(512);
  
//   int toMax = 0;
//   int numOfCred = 0;

//   while(true){
//     if (toMax > 200){
//       break;
//     }
//     if (char(EEPROM.read(0x0F+toMax)) == NULL){
//       break;
//     }

//     if (char(EEPROM.read(0x0F+toMax)) == ';'){
//       numOfCred++;
//       toMax++;
//     }

//     switch (numOfCred){
//     case 0:
//       //ssid
//       cred.ssid += char(EEPROM.read(0x0F+toMax));
//       break;
//     case 1:
//       //passwd
//       cred.pass += char(EEPROM.read(0x0F+toMax));
//       break;
//     default:
//       break;
//     }

//     toMax++;
//   }

//   EEPROM.end();
//   Serial.println("Retrived from EEPROM");
//   Serial.println(cred.ssid);
//   Serial.println(cred.pass);
//   return cred;
// }



void handleRoot() {

  // server.send(200, "text/html", "<h1>You are connected</h1>");
  // server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
  if (broadcastOpen){
    server.send(200, "text/html", "<html> <head> <title>ESP8266 Page</title> <meta name='viewport' content='width=device-width, initial-scale=1.0'>  <style>h1 {text-align:center; }</style> </head> <body> <p>Set ssid and password</p><form action=\"/connect\" method=\"POST\"><input type=\"text\" name=\"ssid\" placeholder=\"ssid\" value=\"" + credToSend.ssid + "\"></br><input type=\"text\" name=\"password\" placeholder=\"password\" value=\"" + credToSend.pass + "\"></br><input type=\"submit\" value=\"Submit\"> </form> </body> </html>");
  } 
  else{
    server.send(200, "text/html", "<h1>You are connected to local wifi!</h1>");
  }
}

void broadcastEnd(){
  broadcastOpen = false;
  dnsServer.stop();
  WiFi.softAPdisconnect(false);
  WiFi.enableAP(false);
  Serial.println("Broadcast stopped");
}

void handleConnect() {     
  String ssid = server.arg("ssid");
  String passwd = server.arg("password");
  
  
  Serial.println(ssid);
  Serial.println(passwd);

  server.sendHeader("Location","/");
  server.send(303);

  credToSend = retriveCredentials();
  broadcastEnd();
  wifiConnect(ssid, passwd);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
  server.sendHeader("Location","/");
}

void serverStart(){
  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.onNotFound(handleNotFound);
  server.begin(); 
}

void serverStop(){
  server.stop();
}

void broadcastStart(){
  
  credToSend = retriveCredentials();

  broadcastOpen = true;
  WiFi.enableAP(true);
  WiFi.softAP(ssid, password);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  IPAddress myIP = WiFi.softAPIP();
  dnsServer.start(DNS_PORT, "8266.com", myIP);
  Serial.println("router IP:");
  Serial.println(myIP);
  
  Serial.println("Broadcast started");
}

void setupStart(){
  credToSend = retriveCredentials();
  
  if (credToSend.ssid.length() == 0){
    broadcastStart();
  }
  else{
    wifiConnect(credToSend.ssid, credToSend.pass);
  }

  serverStart();
}

void wifiConnect(String name, String passwd){

  digitalWrite(D4, 0);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  saveCredentials(name, passwd);

  WiFi.begin(name, passwd);

  int retries = 0;
  int maxTries = 40;
  while ((WiFi.status() != WL_CONNECTED) && (retries < maxTries + 1)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (retries > maxTries) {
      Serial.println(F("WiFi connection FAILED"));
      // clearEeprom(); //<--- to remove config after unsuccessful connection to wifi
      broadcastStart();
  }
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    Serial.println(F("WiFi connected, Local IP:"));
    Serial.println(WiFi.localIP());
  }
}






void setup() {

  delay(1000);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D4, OUTPUT);

  
  Serial.begin(9600);
  Serial.println();

  setupStart();
}


void ledBlink(){
  unsigned long currentMillis = millis();
 
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
 
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
 
    // set the LED with the ledState of the variable:
    digitalWrite(D4, ledState);
  }
}


void loop() {
  if (broadcastOpen){
    dnsServer.processNextRequest();
    server.handleClient();
    ledBlink();
  }

  
  
}
