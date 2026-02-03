/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// Your WiFi credentials. Set password to "" for open networks.
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

ESP8266 wifi(&EspSerial);


BLYNK_WRITE(V1) {
  TimeInputParam t(param);

  // Process start time

  if (t.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }
  else if (t.isStartSunrise())
  {
    Serial.println("Start at sunrise");
  }
  else if (t.isStartSunset())
  {
    Serial.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (t.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
  }
  else if (t.isStopSunrise())
  {
    Serial.println("Stop at sunrise");
  }
  else if (t.isStopSunset())
  {
    Serial.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone
  // Timezone is already added to start/stop time

  Serial.println(String("Time zone: ") + t.getTZ());

  // Get timezone offset (in seconds)
  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

BLYNK_WRITE(V2) // Start Button Widget writes to Virtual Pin V2
{
  if(param.asInt() == 1) {     // if Button sends 1
    Serial.println("Cleaner is turned ON");
  }
  else{
    Serial.println("Cleaner is turned OFF");
  }
}


//as soon as connected update TimeInput widget state
BLYNK_CONNECTED() {
  //seconds from the start of a day. 0 - min, 86399 - max
  int startAt = 5 * 60; //00:05

  //seconds from the start of a day. 0 - min, 86399 - max
  int stopAt = (60 + 5) * 60; //01:05

  //timezone
  //full list of supported timezones could be found here
  //https://www.mkyong.com/java/java-display-list-of-timezone-with-gmt/
  char tz[] = "Asia/Karachi";

  Blynk.virtualWrite(V1, startAt, stopAt, tz);

  //you may also pass day
  //char days[] = "1"; //Monday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);

  //or days
  //char days[] = "1,2,3"; //Monday, Tuesday, Wednesday
  //Blynk.virtualWrite(V1, startAt, stopAt, tz, days);
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  //Serial.setDebugOutput(true);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  /*
  WiFi.mode(WIFI_STA);
   int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    printf("."); // cnt<10 connects to previous wifi network if there is one otherwise cnt>10 enters Smart Config mode
    if (cnt++ >= 10) {
      WiFi.beginSmartConfig();
      Serial.println("Waiting for SmartConfig.");
      while (1) {
        delay(1000);
        printf(".");
        if (WiFi.smartConfigDone()) {
          printf("");
          printf("Smart Config Done.");
          break;
        }
      }
    }
  }*/
  Blynk.begin(auth, wifi, ssid, pass);
  //Blynk.begin(auth, ssid, pass);
  //Blynk.config(auth);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}
