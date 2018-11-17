#include <gui/screen1_screen/Screen1View.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265

Screen1View::Screen1View() :  colorCallback(this, &Screen1View::colorHandler)
{

}

void Screen1View::setupScreen()
{
 color_box.setClickAction(colorCallback);
}

void Screen1View::tearDownScreen()
{

}


void Screen1View::colorHandler(const ScalableImage& b, const ClickEvent& evt)
{
	if (&b == &color_box)
	{
                int16_t x = evt.getX() - 135;
                int16_t y = (evt.getY() - 135);
                uint16_t rad = sqrt((x*x) + (y*y));
                int16_t r,g,b;

                
                float theta = atan(y / float(x)) * 180.0f / PI;
                if ( x < 0 ) {
                  theta += 180;
                } else if ( y < 0 ) {
                  theta += 360;
                }
                if ( rad <= RADIUS) {
                    int* vals = convertHSBtoRGB(theta, rad/float(RADIUS * 1.0f), 1.0f);
                    r = vals[0];
                    g = vals[1];
                    b = vals[2];
                } else {
                  r = 0;
                  g = 0;
                  b = 0;
                }

                selector.setAlpha(0);
                selector.invalidate();
                selector.setXY(evt.getX(), evt.getY());
                selector.setAlpha(255);
                
                Unicode::snprintf(powerLabelBuffer, POWERLABEL_SIZE, "#%02x%02x%02x", r,g,b);
                powerLabel.invalidate();
                selector.invalidate();
	}
        
}

int *Screen1View::convertHSBtoRGB(float H, float S, float B){
  // theta, r/circleR, circleR/circleR
  // HSB to RGB Formula
  // https://www.rapidtables.com/convert/color/hsv-to-rgb.html
  float C = S * B;
  float t = H/60;
  float mod = fmod(t,2);
  float X = C * (1 - abs(mod - 1));
  float m = B - C;
  float rp, gp, bp;
  if(H < 60){
    rp = C;
    gp = X;
    bp = 0;
  }
  else if(H < 120){
    rp = X;
    gp = C;
    bp = 0;
  }
  else if(H < 180){
    rp = 0;
    gp = C;
    bp = X;
  }
  else if(H < 240){
    rp = 0;
    gp = X;
    bp = C;
  }
  else if(H < 300){
    rp = X;
    gp = 0;
    bp = C;
  }
  else{
    rp = C;
    gp = 0;
    bp = X;
  }
  // rgb format
  int *vals =  new int[3];
  vals[0] = int((rp+m)*255);
  vals[1] = int((gp+m)*255);
  vals[2] = int((bp+m)*255);
  
  return vals;
}




