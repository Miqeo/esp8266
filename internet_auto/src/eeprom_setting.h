#include <Arduino.h>
#include <EEPROM.h>

struct Credentails{
  String ssid;
  String pass;
};

String writeString(String cred[2]){
  String credString = "";
  for (int i = 0; i <2; i++) {
    credString += cred[i];
    credString += ";";
  }

  return credString;
}

void clearEeprom(){
  EEPROM.begin(512);
  
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();
  Serial.println("EEPROM cleared");
  delay(500);
}

void saveCredentials(String SSID, String PASSWORD){
  clearEeprom();
  EEPROM.begin(512);
  
  String cred[2] = {SSID,PASSWORD};
  String toSave = writeString(cred);
  Serial.print("Saved to EEPROM ");
  Serial.print(toSave);
  Serial.println("");
  for(int i=0;i<toSave.length();i++){
    EEPROM.write(0x0F+i, toSave[i]);
  }
  
  EEPROM.commit();
  EEPROM.end();
}

Credentails retriveCredentials(){
  Credentails cred;

  EEPROM.begin(512);
  
  int toMax = 0;
  int numOfCred = 0;

  while(true){
    if (toMax > 200){
      break;
    }
    if (char(EEPROM.read(0x0F+toMax)) == NULL){
      break;
    }

    if (char(EEPROM.read(0x0F+toMax)) == ';'){
      numOfCred++;
      toMax++;
    }

    switch (numOfCred){
    case 0:
      //ssid
      cred.ssid += char(EEPROM.read(0x0F+toMax));
      break;
    case 1:
      //passwd
      cred.pass += char(EEPROM.read(0x0F+toMax));
      break;
    default:
      break;
    }

    toMax++;
  }

  EEPROM.end();
  Serial.println("Retrived from EEPROM");
  Serial.println(cred.ssid);
  Serial.println(cred.pass);
  return cred;
}