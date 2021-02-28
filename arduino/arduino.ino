#include <Wire.h>
#include <LiquidCrystal.h>   // include LCD library
#include <DS1302RTC.h>
#include <Time.h>
#include <TimeLib.h>

// Init the DS1302
// Set pins:  CE, IO, CLK
DS1302RTC RTC(13, 11, 12);

// Optional connection for RTC module
#define DS1302_GND_PIN 33
#define DS1302_VCC_PIN 35

// Init the LCD
//   initialize the library with the numbers of the interface pins
//            lcd(RS,  E, d4, d5, d6, d7)
LiquidCrystal lcd(8,   9,  4,  5,  6,  7);

void setup()
{
  Serial.begin(9600);
  
  // Setup LCD to 16x2 characters
  lcd.begin(16, 2);

  // Activate RTC module
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);

  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);

  lcd.print("RTC activated");
  Serial.println("RTC activated");

  delay(500);

  // Check clock oscillation  
  lcd.clear();
  if (RTC.haltRTC())
  {
    lcd.print("Clock stopped!");
    Serial.println("The DS1302 is stopped. Set time to initialize the time and begin running.");
    
    //setTime(1,49,0,1,02,2021); // setTime(hr,min,sec,day,month,yr)
    //time_t t = now();
    //RTC.set(t);
    
  }
  else
  {
    //setTime(13,52,0,1,02,2021); // setTime(hr,min,sec,day,month,yr)
    //time_t t = now();
    //RTC.set(t);
    lcd.print("Clock working.");
    Serial.println("Clock working.");
  }

  // Check write-protection
  lcd.setCursor(0,1);
  if (RTC.writeEN())
  {
    lcd.print("Write allowed.");
    Serial.println("Write allowed.");
  }
  else
  {
    lcd.print("Write blocked.");
    Serial.println("Write blocked.");
  }

  delay ( 2000 );

  // Setup Time library  
  lcd.clear();
  lcd.print("RTC Sync");
  Serial.print("RTC Sync");
  
  setSyncProvider(RTC.get); // the function to get the time from the RTC
  if(timeStatus() == timeSet)
  {
    lcd.print(" OK!");
    Serial.println(" OK!");
  }
  else
  {
    lcd.print(" FAIL!");
    Serial.println(" FAIL!");
  }

  delay ( 2000 );

  lcd.clear();
}

void loop()
{

  // Display time centered on the upper line
  lcd.setCursor(3, 0);
  print2digits(hour());
  lcd.print("  ");
  print2digits(minute());
  lcd.print("  ");
  print2digits(second());

  // Display abbreviated Day-of-Week in the lower left corner
  lcd.setCursor(0, 1);
  lcd.print(dayShortStr(weekday()));

  // Display date in the lower right corner
  lcd.setCursor(5, 1);
  lcd.print(" ");
  print2digits(day());
  lcd.print("/");
  print2digits(month());
  lcd.print("/");
  lcd.print(year());

  Serial.println(String(hour()) + ":"+ String(minute()) + ":" + String(second()) + ":" + String(day()) + ":"+ String(month()) + ":" + String(year()));

  // Warning!
  if(timeStatus() != timeSet) {
    lcd.setCursor(0, 1);
    lcd.print(F("RTC ERROR: SYNC!"));
  }

  delay ( 1000 ); // Wait approx 1 sec
}

void print2digits(int number) {
  // Output leading zero
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
