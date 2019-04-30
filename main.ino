#include <Adafruit_ST7735.h>

#include <HttpClient.h>

#define cs   A2
#define dc   A1
#define rst  A0 

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst); // hardware spi

unsigned int nextTime = 0;
float difference;
String difference;
String pewdiepie;
String tseries;

HttpClient http;
http_header_t headers[] = {{"Accept" , "*/*"},{NULL, NULL}};

http_request_t request;
http_response_t response;

void setup() {
    Serial.begin(9600);
	tft.initR( INITR_BLACKTAB );
	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(12, 12);
    tft.setTextColor(ST7735_WHITE);
    tft.setRotation(1);
    tft.setTextSize(1);
}

void loop() {
    if (nextTime > millis()) {
        return;
    }

    request.hostname = "heyroot.com";
    request.port = 80;
    request.path = "/photon/pewdiepie.php";

    http.get(request, response, headers);
    //Particle.publish("STATUS", String(response.status));
    //Particle.publish("STATUS", response.body);
    
    float p1 = getValue(response.body, ',', 0).toFloat();
    float t1 = getValue(response.body, ',',  1).toFloat();
    difference = getValue(String(p1 - t1), '.', 0);
    pewdiepie = getValue(String(p1), '.', 0);
    tseries = getValue(String(t1), '.', 0);
    //Particle.publish("PEWDIEPIE", String(pewdiepie));
    //Particle.publish("TSERIES", String(tseries));
	tft.fillScreen(ST7735_BLACK);       
	tft.setCursor(12, 12);

    if(pewdiepie > tseries){
        Particle.publish("pewdiepie", difference);
        tft.print("PEWDIEPIE IS WINNING BY");
        tft.setCursor(12, 16+12);
        tft.setTextColor(ST7735_RED);
        tft.setTextSize(2);
        tft.setCursor(40, 16+12);
        tft.print(difference);
        tft.setCursor(46, 40+12);
        tft.setTextColor(ST7735_WHITE);
        tft.setTextSize(1);
        tft.print("SUBSCRIBERS!");
        //tft.setCursor(46, 50+12);
       // tft.print("TSERIES: 95000000");
       // tft.setCursor(46, 50+12);
        //tft.print("PEWDIEPIE: 95000000");

    } else {
        String difference = getValue(String(tseries - pewdiepie), '.', 0);
        Particle.publish("tseries", difference);
        tft.print("TSERIES IS WINNING BY " + difference + " SUBSCRIBERS :(");
    }
    nextTime = millis() + 5000;
}


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
