#include <gui/screen1_screen/Screen1View.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <math.h>

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
                int16_t y = -1 * (evt.getY() - 135);
                uint16_t rad = sqrt((x*x) + (y*y));
                int16_t r,g,b;
		if ( rad <= RADIUS) {
                    r = x + 135;
                    g = y + 135;
                   b = sqrt(x * y);
                } else {
                  r = 0;
                  g = 0;
                  b = 0;
                }
                
                float theta = atan(y / float(x)) * 180.0f / PI;
                if ( x < 0 ) {
                  theta += 180;
                } else if ( y < 0 ) {
                  theta += 360;
                }
                selector.setAlpha(0);
                selector.invalidate();
                selector.setXY(evt.getX(), evt.getY());
                selector.setAlpha(255);
                
                Unicode::snprintf(powerLabelBuffer, POWERLABEL_SIZE, "#%02x%02x%02x", r,g,b);
                touchgfx_printf("R: %d Theta: %f\n",rad, theta);
                powerLabel.invalidate();
                selector.invalidate();
	}
        
}




