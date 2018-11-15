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

    void ColorClickHandler(const AbstractButton& b, const ClickEvent& e);

protected:
 
  Callback<Screen1View, const AbstractButton&, const ClickEvent&> ButtonClickedCallback;
  

};

#endif // SCREEN1_VIEW_HPP
