#include <SPI.h> //SD card module
#include <SD.h> //SD card
#include <DHT.h> //DHT
#include <RTClib.h> //RTC

//DHT pin
#define DHTPIN 2

#define DHTTYPE DHT11   //DHT 11
//#define DHTTYPE DHT21   //DHT 21 (AM2301)
//#define DHTTYPE DHT22   //DHT 22 (AM2302)

//initialize DHT
DHT dht(DHTPIN, DHTTYPE);

const int chipSelect = 4;

//make file
File myFile;

//RTC
RTC_DS1307 rtc;

void setup() {
  //initializing DHT
  dht.begin();

  //initializing Serial monitor
  Serial.begin(9600);

  //setup RTC
  while(!Serial);
    if(! rtc.begin()) {
      Serial.println("RTC not found");
      while (1);
    }
    else {
      //set RTC
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    if(! rtc.isrunning()) {
      Serial.println("RTC not running");
    }

  //setup SD
  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("SD initialization failed");
    return;
  }
  Serial.println("SD initialization successful.");

  //open file
  myFile=SD.open("Marte.txt", FILE_WRITE);

  if (myFile) {
    Serial.println("file open");
    //print headings
    myFile.println("Date,Time,Temperature ºC, Humidity %");
    Serial.println("file heading written");
  }
  myFile.close();
}

void logTime() {
  DateTime now = rtc.now();
  myFile = SD.open("Marte.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");
  }
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  myFile.close();
  delay(500);
}

void logTemp_Hum() {
  //temperature as C
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  //if read failed repeat
  if  (isnan(t)) {
    Serial.println("DHT read failed");
    return;
  }
//serial output
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  myFile = SD.open("Marte.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(t);
    myFile.print(",");
    myFile.print(h);
    myFile.println(",");
    Serial.println("written to file");
  }
  myFile.close();
}

void loop() {
  logTime();
  logTemp_Hum();
  delay(5000);
}
