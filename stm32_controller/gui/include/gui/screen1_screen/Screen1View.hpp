#ifndef SCREEN1_VIEW_HPP
#define SCREEN1_VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/Texts.hpp>


class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void colorHandler(const ScalableImage& b, const ClickEvent& e);
    int *convertHSBtoRGB(float H, float S, float B);
    

protected:
  const int RADIUS = 135;
  Callback<Screen1View, const ScalableImage&, const ClickEvent&> colorCallback;
  

};

#endif // SCREEN1_VIEW_HPP
