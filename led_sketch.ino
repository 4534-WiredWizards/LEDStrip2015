

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      120

const int IR = 0; //ANALOG

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 10; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  
  pinMode(5,INPUT);
}

int increment = -1;
int pos = 0;
int rvalue = 255;

void loop() {
  int blueAlliance = digitalRead(5);
  
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
  
  if(dis >= 115) {
    r=50;
    g=50;
    b=50;
  } else if(dis >= 50) {
    g=25;
    b=50;
    r=50;
  } else {
    g=0;
    if(blueAlliance) {
    b=50;
    r=0;
    } else {
    b=0;
    r=50;
    }
  }
  
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    //pixels.setPixelColor(i-1,pixels.Color(0,0,0));
    pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
    
    
  }
  
  
  
  pixels.show();
  delay(delayval);
  
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
