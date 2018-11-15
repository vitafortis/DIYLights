#include <gui/screen1_screen/Screen1View.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>


Screen1View::Screen1View() : ButtonClickedCallback(this, &Screen1View::ColorClickHandler)
{

}

void Screen1View::setupScreen()
{

}

void Screen1View::tearDownScreen()
{

}


void Screen1View::ColorClickHandler(const touchgfx::AbstractButton& b, const ClickEvent& evt)
{
	if (&b == &color_button)
	{
		//When HiddenBox is clicked move CoverBox
		int16_t x = evt.getX();
                Unicode::snprintf(powerLabelBuffer, POWERLABEL_SIZE, "%d", x);
                touchgfx_printf("X: %d",x);
                powerLabel.invalidate();
		// Check if CoverBox is already moving
		/*if (!CoverBox.isRunning())
		{
			//Move CoverBox to x:550, y:210 with BounceOut easing in 90 Ticks
			CoverBox.startMoveAnimation(HiddenBox.getX(), HiddenBox.getY(), 90, EasingEquations::bounceEaseOut, EasingEquations::bounceEaseOut);
		}*/
	}
}




