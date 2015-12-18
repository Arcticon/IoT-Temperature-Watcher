#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*
 * LCD Display
 */
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/*
 * Set variables for the TCP connection
 */
byte macAdress[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10,11,90,231);
IPAddress dnsServer(10,10,10,10);
IPAddress gateway(10, 11,90,254);
IPAddress subnet(255, 255, 255, 0);

/*
 * Set Temperature Sensors
 */
OneWire temperatureSensor1(2);
OneWire temperatureSensor2(3);
OneWire temperatureSensor3(8);
OneWire temperatureSensor4(5);
OneWire temperatureSensor5(6);
OneWire temperatureSensor6(7);
byte sensorCount = 6;
bool isSensor1Enabled = true;
bool isSensor2Enabled = true;
bool isSensor3Enabled = true;
bool isSensor4Enabled = true;
bool isSensor5Enabled = true;
bool isSensor6Enabled = true;

bool isFan1Enabled = true;
bool isFan2Enabled = true;


/* 
 * temperatureSensor1820 Variables
 */
byte i;
byte present = 0;
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

/*
 * Temperature variables
 */
float celsius = 0;
float temperature1, temperature2, temperature3, temperature4, temperature5, temperature6 = 0;

/*
 * Variables for Datacenter, Room & Rack number
 */
byte datacenterNumber = 1;
byte roomNumber = 1;
byte rackNumber = 1;

/*
 * TCP Port number
 */
const unsigned int port = 50001;

/*
 * Set TCP Server and Client
 */
EthernetServer server(port);
EthernetClient client;

char stringBuffer[10];

String str_data = "";

bool fan1State = false;
bool fan2State = false;

/*
 * Pins for the Fans
 */
byte fan1Pin = 4;
byte fan2Pin = 9;

/*
 * Thresholds for the Fans to start
 */
byte thresholdUpper1Fan1 = 34;
byte thresholdUpper1Fan2 = 38;
byte thresholdLower1 = 30;
byte thresholdUpper2Fan2 = 32;
byte thresholdUpper2Fan1 = 34;
byte thresholdLower2 = 27;
bool thresholdUpper1Fan2Control = false;
bool thresholdUpper2Fan1Control = false;

/*
 * Text Thresholds
 */
byte thresholdSensorTopHigh = 40;
byte thresholdSensorTopLow = 30;
byte thresholdSensorCenterHigh = 40;
byte thresholdSensorCenterLow = 30;
byte thresholdSensorBottomHigh = 40;
byte thresholdSensorBottomLow = 30;

const int sensorDelay = 0;

long currTime = 0;
long currTime2 = 0;

void setTemperatures();

/*
 * Function to print Data to the LCD Display
 */
void printDataToLcd();

void setup()
{
  Serial.begin(115200);
  
  lcd.begin(20,4);
  lcd.setCursor(0, 0);

  if ( !temperatureSensor1.search(addr1)) {
    temperatureSensor1.reset_search();
    Serial.println("ERROR #1");
    return;
  }
  if ( !temperatureSensor2.search(addr2)) {
    temperatureSensor2.reset_search();
    Serial.println("ERROR #2");
    return;
  }
  if ( !temperatureSensor3.search(addr3)) {
    temperatureSensor3.reset_search();
    Serial.println("ERROR #3");
    return;
  }
  if ( !temperatureSensor4.search(addr4)) {
    temperatureSensor4.reset_search();
    Serial.println("ERROR #4");
    return;
  }
  if ( !temperatureSensor5.search(addr5)) {
    temperatureSensor5.reset_search();
    Serial.println("ERROR #5");
    return;
  }
  if ( !temperatureSensor6.search(addr6)) {
    temperatureSensor6.reset_search();
    Serial.println("ERROR #6");
    return;
  }

  Ethernet.begin(macAdress, ip, dnsServer, gateway, subnet);
  server.begin();

  pinMode(fan1Pin, OUTPUT);
  pinMode(fan2Pin, OUTPUT);

  setTemperatures();

  printDataToLcd();
}

void loop() {
  client = server.available();
  
  if (millis() - currTime >= 500) {
    setTemperatures();
    Serial.println(temperature1);
    Serial.println(temperature2);
    Serial.println(temperature3);
    Serial.println(temperature4);
    Serial.println(temperature5);
    Serial.println(temperature6);
    Serial.println("------------------------------------");
    Serial.print(thresholdSensorTopLow);
    Serial.print("\t");
    Serial.print(thresholdSensorCenterLow);
    Serial.print("\t");
    Serial.println(thresholdSensorBottomLow);
    Serial.print(thresholdSensorTopHigh);
    Serial.print("\t");
    Serial.print(thresholdSensorCenterHigh);
    Serial.print("\t");
    Serial.println(thresholdSensorBottomHigh);
    Serial.println("------------------------------------");
    Serial.print(thresholdUpper1Fan1);
    Serial.print("\t");
    Serial.print(thresholdUpper1Fan2);
    Serial.print("\t");
    Serial.println(thresholdLower1);
    Serial.print(thresholdUpper2Fan1);
    Serial.print("\t");
    Serial.print(thresholdUpper2Fan2);
    Serial.print("\t");
    Serial.println(thresholdLower2);
    Serial.println("------------------------------------");
    Serial.print(isFan1Enabled);
    Serial.print("\t");
    Serial.println(isFan2Enabled);
    Serial.print(isSensor1Enabled);
    Serial.print("\t");
    Serial.print(isSensor2Enabled);
    Serial.print("\t");
    Serial.print(isSensor3Enabled);
    Serial.print("\t");
    Serial.print(isSensor4Enabled);
    Serial.print("\t");
    Serial.print(isSensor5Enabled);
    Serial.print("\t");
    Serial.println(isSensor6Enabled);
    Serial.println("------------------------------------");
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
      //Serial.println(str_data);
      String str_split1 = str_data.substring(0, 7);
      int sliderVal = str_data.substring(8, 10).toInt();
      if(str_split1 == "slider1"){
        thresholdUpper1Fan1 = sliderVal;
      }else if(str_split1 == "slider2"){
        thresholdUpper1Fan2 = sliderVal;
        thresholdUpper1Fan2Control == true;
      }else if(str_split1 == "slider3"){
        thresholdLower1 = sliderVal;
      }else if(str_split1 == "slider4"){
        thresholdUpper2Fan1 = sliderVal;
        thresholdUpper2Fan1Control == true;
      }else if(str_split1 == "slider5"){
        thresholdUpper2Fan2 = sliderVal;
      }else if(str_split1 == "slider6"){
        thresholdLower2 = sliderVal;
      }else if(str_split1 == "settsth"){
        Serial.println(sliderVal);
        thresholdSensorTopHigh = sliderVal;
      }else if(str_split1 == "settstl"){
        Serial.println(sliderVal);
        thresholdSensorTopLow = sliderVal;
      }else if(str_split1 == "settsch"){
        Serial.println(sliderVal);
        thresholdSensorCenterHigh = sliderVal;
      }else if(str_split1 == "settscl"){
        Serial.println(sliderVal);
        thresholdSensorCenterLow = sliderVal;
      }else if(str_split1 == "settsbh"){
        Serial.println(sliderVal);
        thresholdSensorBottomHigh = sliderVal;
      }else if(str_split1 == "settsbl"){
        Serial.println(sliderVal);
        thresholdSensorBottomLow = sliderVal;
      }
      
      if (str_data == "getTemp1") {
        //dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);
        server.write(dtostrf(temperature1, 0, 2, stringBuffer));
      } else if (str_data == "getTemp2") {
        server.write(dtostrf(temperature2, 0, 2, stringBuffer));
      } else if (str_data == "getTemp3") {
        server.write(dtostrf(temperature3, 0, 2, stringBuffer));
      } else if (str_data == "getTemp4") {
        server.write(dtostrf(temperature4, 0, 2, stringBuffer));
      } else if (str_data == "getTemp5") {
        server.write(dtostrf(temperature5, 0, 2, stringBuffer));
      } else if (str_data == "getTemp6") {
        server.write(dtostrf(temperature6, 0, 2, stringBuffer));
      } else if (str_data == "getFan1") {
        if(isFan1Enabled){
          if (fan1State == true) {
            server.write("f1on");
          } else {
            server.write("f1off");
          }
        }else{
          server.write("f1nc");
        }
      } else if (str_data == "getFan2") {
        if(isFan2Enabled){
          if (fan2State == true) {
            server.write("f2on");
          } else {
            server.write("f2off");
          }
        }else{
          server.write("f2nc");
        }
      } else if (str_data == "setFan1On") {
        //Fan 1 ON
        digitalWrite(fan1Pin, HIGH);
        fan1State = true;
      } else if (str_data == "setFan1Off") {
        //Fan 1 OFF
        digitalWrite(fan1Pin, LOW);
        fan1State = false;
      } else if (str_data == "setFan2On") {
        //Fan 2 ON
        digitalWrite(fan2Pin, HIGH);
        fan2State = true;
      } else if (str_data == "setFan2Off") {
        //Fan 2 OFF
        digitalWrite(fan2Pin, LOW);
        fan2State = false;
      } else if (str_data == "gettsth") {
        server.write(dtostrf(thresholdSensorTopHigh, 0, 2, stringBuffer));
      } else if (str_data == "gettstl") {
        server.write(dtostrf(thresholdSensorTopLow, 0, 2, stringBuffer));
      } else if (str_data == "gettsch") {
        server.write(dtostrf(thresholdSensorCenterHigh, 0, 2, stringBuffer));
      } else if (str_data == "gettscl") {
        server.write(dtostrf(thresholdSensorCenterLow, 0, 2, stringBuffer));
      } else if (str_data == "gettsbh") {
        server.write(dtostrf(thresholdSensorBottomHigh, 0, 2, stringBuffer));
      } else if (str_data == "gettsbl") {
        server.write(dtostrf(thresholdSensorBottomLow, 0, 2, stringBuffer));
      } else if (str_data == "dc01ro01ra01") {
        server.write(dtostrf(((temperature1 + temperature2 + temperature3 + temperature4 + temperature5 + temperature6)/sensorCount), 0, 2, stringBuffer));
      } else if (str_data == "getdc") {
        server.write(datacenterNumber);
      } else if (str_data == "getrm") {
        server.write(roomNumber);
      } else if (str_data == "getrk") {
        server.write(rackNumber);
      } else if (str_data == "addF1") {
        isFan1Enabled = true;
      } else if (str_data == "addF2") {
        isFan2Enabled = true;
      } else if (str_data == "remF1") {
        isFan1Enabled = false;
      } else if (str_data == "remF2") {
        isFan2Enabled = false;
      } else if (str_data == "addS1") {
        isSensor1Enabled = true;
      } else if (str_data == "addS2") {
        isSensor2Enabled = true;
      } else if (str_data == "addS3") {
        isSensor3Enabled = true;
      } else if (str_data == "addS4") {
        isSensor4Enabled = true;
      } else if (str_data == "addS5") {
        isSensor5Enabled = true;
      } else if (str_data == "addS6") {
        isSensor6Enabled = true;
      } else if (str_data == "remS1") {
        isSensor1Enabled = false;
      } else if (str_data == "remS2") {
        isSensor2Enabled = false;
      } else if (str_data == "remS3") {
        isSensor3Enabled = false;
      } else if (str_data == "remS4") {
        isSensor4Enabled = false;
      } else if (str_data == "remS5") {
        isSensor5Enabled = false;
      } else if (str_data == "remS6") {
        isSensor6Enabled = false;
      }
      
      str_data = "";
    }
  }
}

// Read Temperature from temperatureSensor1820
float getTemp(int int_switch) {
  int16_t raw;
  if (int_switch == 1) {
    temperatureSensor1.reset();
    temperatureSensor1.select(addr1);
    temperatureSensor1.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor1.reset();
    temperatureSensor1.select(addr1);
    temperatureSensor1.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor1.read();
    }
    raw = (data1[1] << 8) | data1[0];
  } else if (int_switch == 2) {
    temperatureSensor2.reset();
    temperatureSensor2.select(addr2);
    temperatureSensor2.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor2.reset();
    temperatureSensor2.select(addr2);
    temperatureSensor2.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data2[i] = temperatureSensor2.read();
    }
    raw = (data2[1] << 8) | data2[0];
  }else if (int_switch == 3) {
    temperatureSensor3.reset();
    temperatureSensor3.select(addr3);
    temperatureSensor3.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor3.reset();
    temperatureSensor3.select(addr3);
    temperatureSensor3.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data3[i] = temperatureSensor3.read();
    }
    raw = (data3[1] << 8) | data3[0];
  }else if (int_switch == 4) {
    temperatureSensor4.reset();
    temperatureSensor4.select(addr4);
    temperatureSensor4.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor4.reset();
    temperatureSensor4.select(addr4);
    temperatureSensor4.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data4[i] = temperatureSensor4.read();
    }
    raw = (data4[1] << 8) | data4[0];
  }else if (int_switch == 5) {
    temperatureSensor5.reset();
    temperatureSensor5.select(addr5);
    temperatureSensor5.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor5.reset();
    temperatureSensor5.select(addr5);
    temperatureSensor5.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data5[i] = temperatureSensor5.read();
    }
    raw = (data5[1] << 8) | data5[0];
  }else if (int_switch == 6) {
    temperatureSensor6.reset();
    temperatureSensor6.select(addr6);
    temperatureSensor6.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor6.reset();
    temperatureSensor6.select(addr6);
    temperatureSensor6.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data6[i] = temperatureSensor6.read();
    }
    raw = (data6[1] << 8) | data6[0];
  }
  
  celsius = (float)raw / 16.0;
  
  return celsius;
}

void control() {

  if (temperature1 < thresholdLower1 && thresholdUpper1Fan2Control) {
    if(isFan1Enabled){
      //Fan 1 OFF
      digitalWrite(fan1Pin, LOW);
      fan1State = false;
    }
    if(isFan2Enabled){
      //Fan 2 OFF
      digitalWrite(fan2Pin, LOW);
      fan2State = false;
      thresholdUpper1Fan2Control = false;
    }
  }else if(temperature1 < thresholdLower1){
    if(isFan1Enabled){
      //Fan 1 OFF
      digitalWrite(fan1Pin, LOW);
      fan1State = false;
    }
    if(isFan2Enabled){
      //Fan 2 OFF
      digitalWrite(fan2Pin, LOW);
      fan2State = false;
    }
  }
  if(temperature2 < thresholdLower2 && thresholdUpper2Fan1Control) {
    if(isFan1Enabled){
      //Fan 1 OFF
      digitalWrite(fan1Pin, LOW);
      fan1State = false;
    }
    if(isFan2Enabled){
      //Fan 2 OFF
      digitalWrite(fan2Pin, LOW);
      fan2State = false;
      thresholdUpper2Fan1Control = false;
    }
  }else if(temperature2 < thresholdLower2){
    if(isFan1Enabled){
      //Fan 1 OFF
      digitalWrite(fan1Pin, LOW);
      fan1State = false;
    }
    if(isFan2Enabled){
      //Fan 2 OFF
      digitalWrite(fan2Pin, LOW);
      fan2State = false;
    }
  }

  
  if (temperature1 > thresholdUpper1Fan1) {
    if(isFan1Enabled){
      //Fan 1 ON
      digitalWrite(fan1Pin, HIGH);
      fan1State = true;
    }
  }
  if (temperature1 > thresholdUpper1Fan2) {
    if(isFan2Enabled){
      //Fan 2 ON
      digitalWrite(fan2Pin, HIGH);
      fan2State = true;
    }
  }
  if (temperature2 > thresholdUpper2Fan2) {
    if(isFan2Enabled){
      //Fan 2 ON
      digitalWrite(fan2Pin, HIGH);
      fan2State = true;
    }
  }
  if (temperature2 > thresholdUpper2Fan1) {
    if(isFan1Enabled){
      //Fan 1 ON
      digitalWrite(fan1Pin, HIGH);
      fan1State = true;
    }
  }
}

void setTemperatures(){
  if(isSensor1Enabled){
    temperature1 = getTemp(1);
  }else{
    temperature1 = -1;
  }
  if(isSensor2Enabled){
    temperature2 = getTemp(2);
  }else{
    temperature2 = -1;
  }
  if(isSensor3Enabled){
    temperature3 = getTemp(3);
  }else{
    temperature3 = -1;
  }
  if(isSensor4Enabled){
    temperature4 = getTemp(4);
  }else{
    temperature4 = -1;
  }
  if(isSensor5Enabled){
    temperature5 = getTemp(5);
  }else{
    temperature5 = -1;
  }
  if(isSensor6Enabled){
    temperature6 = getTemp(6);
  }else{
    temperature6 = -1;
  }
}

void printDataToLcd(){
  /*
   * get dc, room, rack number
   * 
   * 
   * 
   * 
   */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DC");
  lcd.print(" ");
  lcd.print(datacenterNumber);
  lcd.setCursor(6, 0);
  lcd.print("Room");
  lcd.print(" ");
  lcd.print(roomNumber);
  lcd.setCursor(14, 0);
  lcd.print("Rack");
  lcd.print(" ");
  lcd.print(rackNumber);

  //Temperature
  if(isSensor1Enabled){
    lcd.setCursor(0, 1);
    lcd.print("STL ");
    lcd.print(temperature1);
  }
  if(isSensor2Enabled){
    lcd.setCursor(0, 2);
    lcd.print("SCL ");
    lcd.print(temperature2);
  }
  if(isSensor3Enabled){
    lcd.setCursor(0, 3);
    lcd.print("SBL ");
    lcd.print(temperature3);
  }
  if(isSensor4Enabled){
    lcd.setCursor(11, 1);
    lcd.print("STR ");
    lcd.print(temperature4);
  }
  if(isSensor5Enabled){
    lcd.setCursor(11, 2);
    lcd.print("SCR ");
    lcd.print(temperature5);
  }
  if(isSensor6Enabled){
    lcd.setCursor(11, 3);
    lcd.print("SBR ");
    lcd.print(temperature6);
  }
}

