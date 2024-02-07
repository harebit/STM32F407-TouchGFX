/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINVIEWBASE_HPP
#define MAINVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class MainViewBase : public touchgfx::View<MainPresenter>
{
public:
    MainViewBase();
    virtual ~MainViewBase();
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void buttonUpClicked()
    {
        // Override and implement this function in Main
    }
    virtual void buttonDownClicked()
    {
        // Override and implement this function in Main
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Image backgroundimage;
    touchgfx::Button buttonUp;
    touchgfx::Button buttonDown;
    touchgfx::Image counter_box;
    touchgfx::TextAreaWithOneWildcard textCounter;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTCOUNTER_SIZE = 10;
    touchgfx::Unicode::UnicodeChar textCounterBuffer[TEXTCOUNTER_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<MainViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // MAINVIEWBASE_HPP
