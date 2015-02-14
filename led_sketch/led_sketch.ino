

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      180

const int IR = 0; //ANALOG

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel pixelss you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

boolean solid = 1; // If the lights show a solid color (false for animations)
boolean isDisabled = 1; //true when robot is disabled
boolean isAuto = 0;  //true when robot is in autonomus mode
boolean isTeleop = 0;  //true when robot is in teleoperated mode
boolean isSet = 0;
int incoming;        // handles incoming bytes
boolean isAscending = 1; // for autonomous processing
int i = 100; // for autonomous processing

int l = 0;  // for the theatrechase program
int t = 0; // another theatrechase variable
int q = 0; // yet another theatrechase variable

boolean averaging = 0;
const int interval = 2; // every (2) milliseconds, it will take a value
const int samples = 5; // takes (5) samples, averages them and uses that result.
int s = samples; // temp variable for averaging (with above two)
int d[] = {0,0,0,0,0}; // temp distance variable
int average = 0;

boolean allianceSet = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(11520000);
  pinMode(5,INPUT);
}
int blueAlliance = 1;
  

int increment = -1;
int pos = 0;
int rvalue = 255;

void loop() {
  
  if (Serial.available() > 0) {
    incoming = Serial.read();
    if (incoming == 'a') {
      isDisabled = 0;
      isAuto = 1;
      isTeleop = 0;
      //Serial.println ("AUTONOMOUS ENABLED!");
    }
    if (incoming == 't') {
      isDisabled = 0;
      isAuto = 0;
      isTeleop = 1;
      //Serial.println ("TELEOP ENABLED!");
    }
    if (incoming == 'd') {
      isDisabled = 1;
      isAuto = 0;
      isTeleop = 0;
      isSet = 0;
      //Serial.println ("ROBOT DISABLED!");
    }
    if (incoming == 'b') {
      blueAlliance = 1;
      allianceSet = true;
      isSet = 1;
      //Serial.println ("Welcome to the blue alliance!");
    }
    if (incoming == 'r') {
      blueAlliance = 0;
      allianceSet = true;
      isSet = 1;
      //Serial.println ("Welcome to the red alliance!");
    }
  }
  int r,g,b;
  /*
  int r = random(256);
  int g = random(256);
  int b = random(256);
  
  rvalue = rvalue + increment;
  
  if(rvalue > 255){
    rvalue = 255;
    increment = -1;
  }
  
  if(rvalue < 0) {
    rvalue = 0;
    increment = 1;
  }
  */
  
  int dis = readDistance();
  if (isDisabled == 1) {
    if(!isSet){        //calls if alliance color is not set
   //this is the original fade in/out animation code for disabled.
 /*  r = i;
   g = i;
   b = i;
      
   if (i > 100)
   isAscending = 0;   
   if (i < 10)
   isAscending = 1;
   
   if (isAscending)
     i = i + 4;
   if (isAscending == 0)
     i = i - 4;
  */
  
    r=0;
    g=0;                // this shows a pretty animation.
    b=0;
  rainbowCycleMod(5);
  solid = 0;
    } else {                   // if the alliance color is set during disabled, 
     if (blueAlliance) {
    fancyAnimation(pixels.Color(0, 0, 50), 5, 6); // this is the animation for when the alliance is set.
    solid = 0;
     } else {
    fancyAnimation(pixels.Color(50, 0, 0), 5, 6);
    solid = 0;
     }
    }
  }
  if (isAuto == 1) {
    r=30;
    g=250;
    b=20;
  }  
  
  if(isTeleop == 1) {           // calls during teleop period
  if(average >= 115) {
    r=50;
    g=50;
    b=50;
  } else if (average >= 50) {
    if (blueAlliance) {              // defaults to blue if not set.
    /*
    theaterChase(pixels.Color(0,50,50),50);      // omitted due to use of delays...
    solid = 0;
    */
    r=00;
    g=50;
    b=50;
  } else {
    /*
    theaterChase(pixels.Color(25,50,50),50);
    solid = 0;
    */
    g=25;
    b=50;
    r=50;
    }
  } else {
    g=0;
    if (blueAlliance) {
    b=50;
    r=0;
    } else {
      r=50;
      b=0;
      }
  }
  }
  if (allianceSet) {                 // calls when alliance color is first set. Then goes to isSet function, see above.
    if (blueAlliance) {
    colorWipe(pixels.Color(0, 0, 255), 5); // Blue
   } else {
    colorWipe(pixels.Color(255, 0, 0), 5); // Red
    }
  }
  
  if (averaging) {      // routine does not work. THIS LINE MUST STILL BE HERE!
  if (s > 0) {          // DO NOT DELETE!
    d[s-1] = dis;
    s--;
    delay(interval);
  } else {
    for (int a = 0; a < samples ; a++) {
      
      average = average + d[a];
    }
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  average = average / 5;
  if (solid) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //pixels.setPixelColor(i-1,pixels.Color(0,0,0));
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
  }
    
  }
  
  pixels.show();
  solid = 1;
  Serial.print (average);
  Serial.println (";");
  average = 0;
  delay(interval);
  }
} else {
  
  if (solid) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //pixels.setPixelColor(i-1,pixels.Color(0,0,0));
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
  }
    
  }
  
  pixels.show();
  solid = 1;
  Serial.print (dis);
  Serial.println (";");
  average = dis;
  delay(10);
}
}
int readDistance()
{
    int dist = analogRead(IR);         //Read IR Sensor
    int dist2 = analogRead(IR);
    dist = (dist+dist2)/2;
    dist = map(dist, 50, 500, 0, 255); //scale it to LED range 
    dist = constrain(dist, 0, 255);    //Constrain it
    return dist;                       //Return scaled distance
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
      allianceSet = 0;
      isDisabled = 1;
      // isSet = 1;
  }
}

/*
void theaterChasemodified() {
  // animation from http://www.tweaking4all.com/hardware/arduino/arduino-ws2812-led/#adafruit_neopixel
        if (q < 3) {
      while (l < pixels.numPixels()) {
        pixels.setPixelColor(l+q, 0);        //turn every third pixel off
      l=l+3;
      }
      q++;
      i=0;
        while (l < pixels.numPixels()) {
        pixels.setPixelColor(l+q, pixels.Color(0, 50, 50));    //turn every third pixel on
      l=l+3;
    }
    pixels.show();
    i=0;
    } else {
      q = 0;
    }
}
*/
void fancyAnimation(uint32_t c, uint8_t wait, byte groups) {
  int g = 14;
  int sum = 0;
  int v = pixels.numPixels() / groups;
  for(uint16_t q=0; q<pixels.numPixels(); q++) {
    for(uint16_t i=0; i<g; i++) {
      for(uint16_t n = 0; n < groups; n++) {
        sum = (i + q) + (n * v);
        while (sum > pixels.numPixels()) {
            sum = sum - pixels.numPixels();
          }
        pixels.setPixelColor(sum, c);
        
      }
    }
    if(!Serial.available()) {
        pixels.show();
        for (uint16_t n = 0; n < groups; n++) {
          sum = q + (n * v);
          
          while (sum > pixels.numPixels()) {
            sum = sum - pixels.numPixels();
          }
          pixels.setPixelColor(sum, pixels.Color(0,0,0));
                    
        }
        delay(wait);
    } else {
      isDisabled = 0;
    }
  }
}


void theaterChase(uint32_t c, uint8_t wait) {
    for (int q=0; q < 3; q++) {
      for (int l=0; l < pixels.numPixels(); l=l+3) {
        pixels.setPixelColor(l+q, c);    //turn every third pixel on
      }
      pixels.show();


      for (int l=0; l < pixels.numPixels(); l=l+3) {
        pixels.setPixelColor(l+q, 0);        //turn every third pixel off
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < pixels.numPixels(); i=i+3) {
          pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        pixels.show();

        delay(wait);

        for (int i=0; i < pixels.numPixels(); i=i+3) {
          pixels.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}



void rainbowCycleMod(uint8_t wait) {
  uint16_t i, j;
  
  for(j=0; j<256; j++) { 
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
   if(!Serial.available()) {
        pixels.show();
        delay(wait);
    } else {
      isDisabled = 0;
    }
  }
}

uint32_t WheelMod(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 100 - WheelPos * 3, 0);
  } else if(WheelPos < 66) {
   WheelPos -= 170;
   return pixels.Color(100 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 66;
   return pixels.Color(0, WheelPos * 3, 100 - WheelPos * 3);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

