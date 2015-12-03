#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
// set the LCD address to 0x3F for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10,11,90,231);
IPAddress myDns(10,10,10,10);
IPAddress gateway(10, 11,90,254);
IPAddress subnet(255, 255, 255, 0);

const long port = 50001;

EthernetServer server(port);
EthernetClient client;

// DS1820 Variablen
byte i;
byte present = 0;
byte type_s1 = 0;
byte type_s2 = 0;
byte data1[12];
byte data2[12];
byte data3[12];
byte data4[12];
byte data5[12];
byte data6[12];
byte addr1[8];
byte addr2[8];
byte addr3[8];
byte addr4[8];
byte addr5[8];
byte addr6[8];

bool ignore = false;

char tmpString[10];

float celsius = 0;
float tmpFloat1, tmpFloat2, tmpFloat3, tmpFloat4, tmpFloat5, tmpFloat6 = 0;

String str_data = "";

int ventilator1State = LOW;
int ventilator2State = LOW;

int globalI = 0;

OneWire ds1(2);
OneWire ds2(3);
OneWire ds3(4);
OneWire ds4(5);
OneWire ds5(6);
OneWire ds6(7);

int relay1 = 8;
int relay2 = 9;

int upperTemp1 = 34;
int upperTemp1v2 = 38;
int lowerTemp1 = 30;
int upperTemp2 = 32;
int upperTemp2v1 = 34;
int lowerTemp2 = 27;
int upperTemp1v2Test = 0;
int upperTemp2v1Test = 0;

const int sensorDelay = 0;

long currTime = 0;
long currTime2 = 0;

void printDataToLcd();

void setup()
{
  Serial.begin(115200);
  lcd.begin(20,4);
  
  lcd.setCursor(0, 0);
  
  if ( !ds1.search(addr1)) {
    ds1.reset_search();
    Serial.println("ERROR #1");
    return;
  }
  if ( !ds2.search(addr2)) {
    ds2.reset_search();
    Serial.println("ERROR #2");
    return;
  }
  if ( !ds3.search(addr3)) {
    ds3.reset_search();
    Serial.println("ERROR #3");
    return;
  }
  if ( !ds4.search(addr4)) {
    ds4.reset_search();
    Serial.println("ERROR #4");
    return;
  }
  if ( !ds5.search(addr5)) {
    ds5.reset_search();
    Serial.println("ERROR #5");
    return;
  }
  if ( !ds6.search(addr6)) {
    ds6.reset_search();
    Serial.println("ERROR #6");
    return;
  }
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  server.begin();

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  tmpFloat1 = getTemp(1);
  tmpFloat2 = getTemp(2);
  tmpFloat3 = getTemp(3);
  tmpFloat4 = getTemp(4);
  tmpFloat5 = getTemp(5);
  tmpFloat6 = getTemp(6);

  printDataToLcd();
}

void loop() {
  client = server.available();
  if (millis() - currTime >= 500) {
    tmpFloat1 = getTemp(1);
    tmpFloat2 = getTemp(2);
    tmpFloat3 = getTemp(3);
    tmpFloat4 = getTemp(4);
    tmpFloat5 = getTemp(5);
    tmpFloat6 = getTemp(6);
    currTime = millis();
  }
  control();
  if (millis() - currTime2 >= 10000) {
    printDataToLcd();
    currTime2 = millis();
  }
  if (client) {
    if (client.available() > 0) {
      char c = client.read();
      str_data += c;
    }
    if (client.available() == 0) {
      Serial.println(str_data);
      String str_split1 = str_data.substring(0, 7);
      int sliderVal = str_data.substring(8, 10).toInt();
      if(str_split1 == "slider1"){
        upperTemp1 = sliderVal;
      }else if(str_split1 == "slider2"){
        lowerTemp1 = sliderVal;
      }else if(str_split1 == "slider3"){
        upperTemp2 = sliderVal;
      }else if(str_split1 == "slider4"){
        lowerTemp2 = sliderVal;
      }else if(str_split1 == "slider5"){
        upperTemp1v2 = sliderVal;
        upperTemp1v2Test == 1;
      }else if(str_split1 == "slider6"){
        upperTemp2v1 = sliderVal;
        upperTemp2v1Test == 1;
      }
      if (str_data == "getTemp1") {
        //dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);
        server.write(dtostrf(tmpFloat1, 0, 2, tmpString));
      } else if (str_data == "getTemp2") {
        server.write(dtostrf(tmpFloat2, 0, 2, tmpString));
      } else if (str_data == "getTemp3") {
        server.write(dtostrf(tmpFloat3, 0, 2, tmpString));
      } else if (str_data == "getTemp4") {
        server.write(dtostrf(tmpFloat4, 0, 2, tmpString));
      } else if (str_data == "getTemp5") {
        server.write(dtostrf(tmpFloat5, 0, 2, tmpString));
      } else if (str_data == "getTemp6") {
        server.write(dtostrf(tmpFloat6, 0, 2, tmpString));
      } else if (str_data == "getVent1") {
        if (ventilator1State == HIGH) {
          server.write("v1on");
        } else {
          server.write("v1off");
        }
      } else if (str_data == "getVent2") {
        if (ventilator2State == HIGH) {
          server.write("v2on");
        } else {
          server.write("v2off");
        }
      } else if (str_data == "customModeOn") {
        ignore = true;
      } else if (str_data == "customModeOff") {
        ignore = false;
        ventilator1State = LOW;
        ventilator2State = LOW;
      } else if (str_data == "setVent1On") {
        //V 1 AN
        digitalWrite(relay1, HIGH);
        ventilator1State = HIGH;
      } else if (str_data == "setVent1Off") {
        //V 1 AUS
        digitalWrite(relay1, LOW);
        ventilator1State = LOW;
      } else if (str_data == "setVent2On") {
        //V 2 AN
        digitalWrite(relay2, HIGH);
        ventilator2State = HIGH;
      } else if (str_data == "setVent2Off") {
        //V 2 AUS
        digitalWrite(relay2, LOW);
        ventilator2State = LOW;
      }
      str_data = "";
    }
  }
}

// Read Temperature from DS1820
float getTemp(int int_switch) {
  if (int_switch == 1) {
    ds1.reset();
    ds1.select(addr1);
    ds1.write(0x44);

    delay(sensorDelay);

    present = ds1.reset();
    ds1.select(addr1);
    ds1.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = ds1.read();
    }
    int16_t raw = (data1[1] << 8) | data1[0];
    celsius = (float)raw / 16.0;
  } else if (int_switch == 2) {
    ds2.reset();
    ds2.select(addr2);
    ds2.write(0x44);

    delay(sensorDelay);

    present = ds2.reset();
    ds2.select(addr2);
    ds2.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data2[i] = ds2.read();
    }

    int16_t raw = (data2[1] << 8) | data2[0];
    celsius = (float)raw / 16.0;
  }else if (int_switch == 3) {
    ds3.reset();
    ds3.select(addr3);
    ds3.write(0x44);

    delay(sensorDelay);

    present = ds3.reset();
    ds3.select(addr3);
    ds3.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data3[i] = ds3.read();
    }

    int16_t raw = (data3[1] << 8) | data3[0];
    celsius = (float)raw / 16.0;
  }else if (int_switch == 4) {
    ds4.reset();
    ds4.select(addr4);
    ds4.write(0x44);

    delay(sensorDelay);

    present = ds4.reset();
    ds4.select(addr4);
    ds4.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data4[i] = ds4.read();
    }

    int16_t raw = (data4[1] << 8) | data4[0];
    celsius = (float)raw / 16.0;
  }else if (int_switch == 5) {
    ds5.reset();
    ds5.select(addr5);
    ds5.write(0x44);

    delay(sensorDelay);

    present = ds5.reset();
    ds5.select(addr5);
    ds5.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data5[i] = ds5.read();
    }

    int16_t raw = (data5[1] << 8) | data5[0];
    celsius = (float)raw / 16.0;
  }else if (int_switch == 6) {
    ds6.reset();
    ds6.select(addr6);
    ds6.write(0x44);

    delay(sensorDelay);

    present = ds6.reset();
    ds6.select(addr6);
    ds6.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data6[i] = ds6.read();
    }

    int16_t raw = (data6[1] << 8) | data6[0];
    celsius = (float)raw / 16.0;
  }
  return celsius;
}

void control() {

  if (!ignore) {
    if (tmpFloat1 > upperTemp1) {
      //V 1 AN
      digitalWrite(relay1, HIGH);
      ventilator1State = HIGH;
    }
    if (tmpFloat1 > upperTemp1v2) {
      //V 2 AN
      digitalWrite(relay2, HIGH);
      ventilator2State = HIGH;
    }
    if (tmpFloat2 > upperTemp2) {
      //V 2 AN
      digitalWrite(relay2, HIGH);
      ventilator2State = HIGH;
    }
    if (tmpFloat2 > upperTemp2v1) {
      //V 1 AN
      digitalWrite(relay1, HIGH);
      ventilator1State = HIGH;
    }

    if (tmpFloat1 < lowerTemp1 && upperTemp1v2Test == 1) {
      //V 1 AUS
      digitalWrite(relay1, LOW);
      ventilator1State = LOW;
      //V 2 AUS
      digitalWrite(relay2, LOW);
      ventilator2State = LOW;
      upperTemp1v2Test == 0;
    }else if(tmpFloat1 < lowerTemp1){
      //V 1 AUS
      digitalWrite(relay1, LOW);
      ventilator1State = LOW;
      //V 2 AUS
      digitalWrite(relay2, LOW);
      ventilator2State = LOW;
    }
    if(tmpFloat2 < lowerTemp2 && upperTemp2v1Test == 1) {
      //V 1 AUS
      digitalWrite(relay1, LOW);
      ventilator1State = LOW;
      //V 2 AUS
      digitalWrite(relay2, LOW);
      ventilator2State = LOW;
      upperTemp2v1Test == 0;
    }else if(tmpFloat2 < lowerTemp2){
      //V 1 AUS
      digitalWrite(relay1, LOW);
      ventilator1State = LOW;
      //V 2 AUS
      digitalWrite(relay2, LOW);
      ventilator2State = LOW;
      upperTemp2v1Test == 0;
    }
    
  }
}

void printDataToLcd(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DC");
  lcd.print(" ");
  lcd.print("1");
  lcd.setCursor(6, 0);
  lcd.print("Room");
  lcd.print(" ");
  lcd.print("1");
  lcd.setCursor(14, 0);
  lcd.print("Rack");
  lcd.print(" ");
  lcd.print("1");

  //Temperature

  lcd.setCursor(0, 1);
  lcd.print("STL ");
  lcd.print(tmpFloat1);
  lcd.setCursor(0, 2);
  lcd.print("SCL ");
  lcd.print(tmpFloat2);
  lcd.setCursor(0, 3);
  lcd.print("SBL ");
  lcd.print(tmpFloat3);
  lcd.setCursor(11, 1);
  lcd.print("STR ");
  lcd.print(tmpFloat4);
  lcd.setCursor(11, 2);
  lcd.print("SCR ");
  lcd.print(tmpFloat5);
  lcd.setCursor(11, 3);
  lcd.print("SBR ");
  lcd.print(tmpFloat6);
}

