
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
                                 //rotation, e, rw, rs, rst
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, D6, D7, D8, D5);

#define logo_width 128
#define logo_height 64
#define octets 1024

String img = "";
String readString;
// static unsigned char rows[] U8X8_PROGMEM = {
// 0xff, 0xff, 
// 0xff, 0xff, 
// 0xff, 0xff, 
// 0xff, 0xff};
const uint8_t rows[] U8X8_PROGMEM = {
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};


uint8_t rows2[octets] U8X8_PROGMEM = {};


void multiTok(String data, int length){
  uint8_t toSend[octets] U8X8_PROGMEM;
  // Serial.println(data);
  toSend[0] = data.substring(0, length).toInt();
  
  int previous = 0;
  int next = 0;
  for(int i = 1; i < octets; i++){
    
    next = i * length;
    // uint8_t int8 = data.substring(commaIndex + 1, nextComma).toInt();
    toSend[i] = data.substring(previous, next).toInt();
    // Serial.println(next);
    // Serial.println(toSend[i]);
    
    // toSend[i] = data.substring(commaIndex + 1, nextComma).toInt();
    previous = next;
    yield();
  }
  
  for(int i = 0; i < octets; i++){
    rows2[i] = toSend[i];
    
    yield();
  }

}

// void multiTok(String data, char delimiter){
//   uint8_t toSend[octets] U8X8_PROGMEM;
  
//   int commaIndex = data.indexOf(delimiter);
//   toSend[0] = data.substring(0, commaIndex).toInt();
//   int nextComma = 0;
//   for(int i = 1; i < octets; i++){
//     nextComma = data.indexOf(delimiter, commaIndex + 1);

//     uint8_t int8 = data.substring(commaIndex + 1, nextComma).toInt();
//     toSend[i] = int8;

//     // toSend[i] = data.substring(commaIndex + 1, nextComma).toInt();
//     commaIndex = nextComma;
//     yield();
//   }
  
//   for(int i = 0; i < octets; i++){
//     rows2[i] = toSend[i];
//     yield();
//   }
  
//   // return toSend;
// }

int countx(String s) {
  // int count = 0;

  // for (int i = 0; i < s.length(); i++)
  //   count++;

  return s.length()/3;
}

void drawLogo();

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(1);
  Serial.println("Started");
  u8g2.begin();
  drawLogo();
}

void drawLogo() {
 u8g2.firstPage();
 do {
   u8g2.drawXBMP(0, 0, logo_width, logo_height, rows); 
 } while ( u8g2.nextPage() );

//  multiTok("255245235", 3);
//     u8g2.firstPage();
//     do {
//       u8g2.drawXBMP(0, 0, logo_width, logo_height, rows2); 
//       yield();
      
//     } while ( u8g2.nextPage() );
 
}
void loop(void) {

  // while (!Serial.available());
  
  while (Serial.available())
  {
    
    if (Serial.available() >0)
    {
      char c = Serial.read();
      readString += c;
    }
    yield();
  }

  if (countx(readString) == (octets - 1)){

    // Serial.println("to display");
    multiTok(readString, 3);
    

    
    // for(int i = 0; i < octets; i++){
    //   // rows2[i] = row[i];//multiTok(readString, 'x')[i];
    //   // Serial.print(" ");
    //   // Serial.print(rows2[i]);
    //   yield();
    // }
    // Serial.println("");
    readString = "";
    
    
    u8g2.firstPage();
    do {
      u8g2.drawXBMP(0, 0, logo_width, logo_height, rows2); 
      yield();
      
    } while ( u8g2.nextPage() );
    Serial.println("Printed");
  }
  
}
  
  // const uint8_t rows2[] U8X8_PROGMEM = {
  // multiTok(img, 'x')[0], multiTok(img, 'x')[1], 
  // multiTok(img, 'x')[2], multiTok(img, 'x')[3],
  // multiTok(img, 'x')[4], multiTok(img, 'x')[5], 
  // multiTok(img, 'x')[6], multiTok(img, 'x')[7]};
  

  
  // const uint8_t rows2[] U8X8_PROGMEM = {
  // 129,128,
  // 129,128,
  // 129,128,
  // 129,128};
  

  // for(int i = 0; i < octets; i++){
  //   Serial.println(rows2[i]);
  // }
  
  

  
  // rows = multiTok(img, 'x');
  // u8g2.firstPage();
  // do {
  //   u8g2.setFont(u8g2_font_ncenB14_tr);
  //   u8g2.drawStr(0,24,"Hello World!");
  //   
  // } while ( u8g2.nextPage() );





// void setup()
// {
// LCDA.Initialise(); // The screen is initialized
// delay(100);
// }

// void loop()
// {
// LCDA.CLEAR();//Clear screen
// delay(100);
// LCDA.DisplayString(0,2,show0,AR_SIZE(show0));//The first line of the third grid starts, showing
// delay(100);
// LCDA.DisplayString(1,1,show1,AR_SIZE(show1));//The first line of the second line starts, display
// delay(100);
// LCDA.DisplayString(2,0,show2,AR_SIZE(show2));//The third line of the first grid, display text
// delay(100);
// LCDA.DisplayString(3,0,show3,AR_SIZE(show3));//The third line of the first grid, display text
// delay(5000);
// LCDA.CLEAR();//Clear screen
// delay(100);
// LCDA.DrawFullScreen(logo0);
// delay(5000);
// }


// #ifdef U8X8_HAVE_HW_SPI
// #include <SPI.h>
// #endif
// #ifdef U8X8_HAVE_HW_I2C
// #include <Wire.h>
// #endif
// // U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0,D8);
// U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// void setup(void) {
//   u8g2.begin();  
// }

// uint8_t m = 24;

// void loop(void) {
//   char m_str[3];
//   strcpy(m_str, u8x8_u8toa(m, 2));		/* convert m to a string with two digits */
//   u8g2.firstPage();
//   do {
//     u8g2.setFont(u8g2_font_logisoso62_tn);
//     u8g2.drawStr(0,63,"9");
//     u8g2.drawStr(33,63,":");
//     u8g2.drawStr(50,63,m_str);
//   } while ( u8g2.nextPage() );
//   delay(1000);
//   m++;
//   if ( m == 60 )
//     m = 0;
// }




// #include <LiquidCrystal.h>

// // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
// LiquidCrystal lcd(14, 12, 16, 5, 4, 0);
// //                d5  d6  d0  d1  d2  d3
// String img = "";

// int* rows;

  
// void setup() {
//   pinMode(13, HIGH);
// //        d7
//   lcd.begin(16, 2);

  
//   lcd.clear();

//   Serial.begin(19200);
//   Serial.setTimeout(1);
  
  
// }

// int* multiTok(String data, char delimiter){
//   static int toSend[64];
  
//   int commaIndex = data.indexOf(delimiter);
//   toSend[0] = data.substring(0, commaIndex).toInt();
//   int nextComma = 0;
//   for(int i = 1; i < 64; i++){
//     nextComma = data.indexOf(delimiter, commaIndex + 1);
//     toSend[i] = data.substring(commaIndex + 1, nextComma).toInt();
//     commaIndex = nextComma;
//   }
  

  
//   return toSend;
// }

// void loop() {
  
//   while (!Serial.available());
  
  
  
// //  for(int i = 1; i < 7; i++){
// //    lcd.setCursor(0, 1);
// //    lcd.write(byte(i));
// //    delay(100);
// //  }

//   delay(100);
  
//   img = Serial.readString();//"31x17x17x17x17x17x17x31";//
//   rows = multiTok(img, 'x');
  
//   byte G1[] = {
//     rows[0],
//     rows[1],
//     rows[2],
//     rows[3],
//     rows[4],
//     rows[5],
//     rows[6],
//     rows[7]
//   };

//   byte G2[] = {
//     rows[8],
//     rows[9],
//     rows[10],
//     rows[11],
//     rows[12],
//     rows[13],
//     rows[14],
//     rows[15]
//   };

//   byte G3[] = {
//     rows[16],
//     rows[17],
//     rows[18],
//     rows[19],
//     rows[20],
//     rows[21],
//     rows[22],
//     rows[23]
//   };

//   byte G4[] = {
//     rows[24],
//     rows[25],
//     rows[26],
//     rows[27],
//     rows[28],
//     rows[29],
//     rows[30],
//     rows[31]
//   };

//   byte G5[] = {
//     rows[32],
//     rows[33],
//     rows[34],
//     rows[35],
//     rows[36],
//     rows[37],
//     rows[38],
//     rows[39]
//   };

//   byte G6[] = {
//     rows[40],
//     rows[41],
//     rows[42],
//     rows[43],
//     rows[44],
//     rows[45],
//     rows[46],
//     rows[47]
//   };

//   byte G7[] = {
//     rows[48],
//     rows[49],
//     rows[50],
//     rows[51],
//     rows[52],
//     rows[53],
//     rows[54],
//     rows[55]
//   };

//   byte G8[] = {
//     rows[56],
//     rows[57],
//     rows[58],
//     rows[59],
//     rows[60],
//     rows[61],
//     rows[62],
//     rows[63]
//   };
  
//   lcd.createChar(0, G1);
//   lcd.createChar(1, G2);
//   lcd.createChar(2, G3);
//   lcd.createChar(3, G4);
//   lcd.createChar(4, G5);
//   lcd.createChar(5, G6);
//   lcd.createChar(6, G7);
//   lcd.createChar(7, G8);
  
//   lcd.setCursor(0, 0);
//   lcd.write(byte(0));
//   lcd.setCursor(1, 0);
//   lcd.write(byte(1));
//   lcd.setCursor(2, 0);
//   lcd.write(byte(2));
//   lcd.setCursor(3, 0);
//   lcd.write(byte(3));
//   lcd.setCursor(0, 1);
//   lcd.write(byte(4));
//   lcd.setCursor(1, 1);
//   lcd.write(byte(5));
//   lcd.setCursor(2, 1);
//   lcd.write(byte(6));
//   lcd.setCursor(3, 1);
//   lcd.write(byte(7));
// //  lcd.setCursor(0, 0);
// //  lcd.print(rows[0]);
  
// //  lcd.print("\\");
// //  delay(500);
// //  lcd.clear();
// //  lcd.print("/");
// //  delay(500);
// //  lcd.clear();
// //  lcd.print("-");
// //  delay(500);
// //  lcd.clear();
  
// }