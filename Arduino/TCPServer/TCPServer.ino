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
OneWire temperatureSensor3(4);
OneWire temperatureSensor4(5);
OneWire temperatureSensor5(6);
OneWire temperatureSensor6(7);
byte sensorCount = 6;
bool isSensor1Enabled, isSensor2Enabled, isSensor3Enabled, isSensor4Enabled, isSensor5Enabled, isSensor6Enabled = true;

/* 
 * temperatureSensor1820 Variables
 */
byte i;
byte present = 0;
byte data1[12];
/*byte data1[12];
byte data1[12];
byte data1[12];
byte data1[12];
byte data1[12];*/
byte addr1[8];
/*byte addr1[8];
byte addr1[8];
byte addr1[8];
byte addr1[8];
byte addr1[8];*/

/*
 * Temperature variables
 */
float celsius = 0;
float temperature1, temperature2, temperature3, temperature4, temperature5, temperature6 = 0;

/*
 * Variables for Datacenter, Room & Rack number
 */
byte datacenterNumber, roomNumber, rackNumber = 1;

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

bool ventilator1State = false;
bool ventilator2State = false;

/*
 * Pins for the Fans
 */
byte fan1Pin = 8;
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
        thresholdLower1 = sliderVal;
      }else if(str_split1 == "slider3"){
        thresholdUpper2Fan2 = sliderVal;
      }else if(str_split1 == "slider4"){
        thresholdLower2 = sliderVal;
      }else if(str_split1 == "slider5"){
        thresholdUpper1Fan2 = sliderVal;
        thresholdUpper1Fan2Control == true;
      }else if(str_split1 == "slider6"){
        thresholdUpper2Fan1 = sliderVal;
        thresholdUpper2Fan1Control == true;
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
      } else if (str_data == "getVent1") {
        if (ventilator1State == true) {
          server.write("v1on");
        } else {
          server.write("v1off");
        }
      } else if (str_data == "getVent2") {
        if (ventilator2State == true) {
          server.write("v2on");
        } else {
          server.write("v2off");
        }
      } else if (str_data == "setVent1On") {
        //Fan 1 ON
        digitalWrite(fan1Pin, HIGH);
        ventilator1State = true;
      } else if (str_data == "setVent1Off") {
        //Fan 1 OFF
        digitalWrite(fan1Pin, LOW);
        ventilator1State = false;
      } else if (str_data == "setVent2On") {
        //Fan 2 ON
        digitalWrite(fan2Pin, HIGH);
        ventilator2State = true;
      } else if (str_data == "setVent2Off") {
        //Fan 2 OFF
        digitalWrite(fan2Pin, LOW);
        ventilator2State = false;
      } else if (str_data == "dc01ro01ra01") {
        server.write(dtostrf(((temperature1 + temperature2 + temperature3 + temperature4 + temperature5 + temperature6)/sensorCount), 0, 2, stringBuffer));
      }
      
      str_data = "";
    }
  }
}

// Read Temperature from temperatureSensor1820
float getTemp(int int_switch) {
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
  } else if (int_switch == 2) {
    temperatureSensor2.reset();
    temperatureSensor2.select(addr1);
    temperatureSensor2.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor2.reset();
    temperatureSensor2.select(addr1);
    temperatureSensor2.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor2.read();
    }
  }else if (int_switch == 3) {
    temperatureSensor3.reset();
    temperatureSensor3.select(addr1);
    temperatureSensor3.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor3.reset();
    temperatureSensor3.select(addr1);
    temperatureSensor3.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor3.read();
    }
  }else if (int_switch == 4) {
    temperatureSensor4.reset();
    temperatureSensor4.select(addr1);
    temperatureSensor4.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor4.reset();
    temperatureSensor4.select(addr1);
    temperatureSensor4.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor4.read();
    }
  }else if (int_switch == 5) {
    temperatureSensor5.reset();
    temperatureSensor5.select(addr1);
    temperatureSensor5.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor5.reset();
    temperatureSensor5.select(addr1);
    temperatureSensor5.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor5.read();
    }
  }else if (int_switch == 6) {
    temperatureSensor6.reset();
    temperatureSensor6.select(addr1);
    temperatureSensor6.write(0x44);

    delay(sensorDelay);

    present = temperatureSensor6.reset();
    temperatureSensor6.select(addr1);
    temperatureSensor6.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data1[i] = temperatureSensor6.read();
    }
  }

  int16_t raw = (data1[1] << 8) | data1[0];
  celsius = (float)raw / 16.0;
  
  return celsius;
}

void control() {
  if (temperature1 > thresholdUpper1Fan1) {
    //Fan 1 ON
    digitalWrite(fan1Pin, HIGH);
    ventilator1State = true;
  }
  if (temperature1 > thresholdUpper1Fan2) {
    //Fan 2 ON
    digitalWrite(fan2Pin, HIGH);
    ventilator2State = true;
  }
  if (temperature2 > thresholdUpper2Fan2) {
    //Fan 2 ON
    digitalWrite(fan2Pin, HIGH);
    ventilator2State = true;
  }
  if (temperature2 > thresholdUpper2Fan1) {
    //Fan 1 ON
    digitalWrite(fan1Pin, HIGH);
    ventilator1State = true;
  }

  if (temperature1 < thresholdLower1 && thresholdUpper1Fan2Control) {
    //Fan 1 OFF
    digitalWrite(fan1Pin, LOW);
    ventilator1State = false;
    //Fan 2 OFF
    digitalWrite(fan2Pin, LOW);
    ventilator2State = false;
    thresholdUpper1Fan2Control = false;
  }else if(temperature1 < thresholdLower1){
    //Fan 1 OFF
    digitalWrite(fan1Pin, LOW);
    ventilator1State = false;
    //Fan 2 OFF
    digitalWrite(fan2Pin, LOW);
    ventilator2State = false;
  }
  if(temperature2 < thresholdLower2 && thresholdUpper2Fan1Control) {
    //Fan 1 OFF
    digitalWrite(fan1Pin, LOW);
    ventilator1State = false;
    //Fan 2 OFF
    digitalWrite(fan2Pin, LOW);
    ventilator2State = false;
    thresholdUpper2Fan1Control = false;
  }else if(temperature2 < thresholdLower2){
    //Fan 1 OFF
    digitalWrite(fan1Pin, LOW);
    ventilator1State = false;
    //Fan 2 OFF
    digitalWrite(fan2Pin, LOW);
    ventilator2State = false;
  }
}

void setTemperatures(){
  if(isSensor1Enabled){
    temperature1 = getTemp(1);
  }
  if(isSensor2Enabled){
    temperature2 = getTemp(2);
  }
  if(isSensor3Enabled){
    temperature3 = getTemp(3);
  }
  if(isSensor4Enabled){
    temperature4 = getTemp(4);
  }
  if(isSensor5Enabled){
    temperature5 = getTemp(5);
  }
  if(isSensor6Enabled){
    temperature6 = getTemp(6);
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

