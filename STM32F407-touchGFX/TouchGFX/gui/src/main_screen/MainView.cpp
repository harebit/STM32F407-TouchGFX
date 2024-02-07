#include <gui/main_screen/MainView.hpp>
#include <touchgfx/utils.hpp>

MainView::MainView()
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::buttonUpClicked()
{  //harebit
    counter++;
    Unicode::snprintf(textCounterBuffer, TEXTCOUNTER_SIZE, "%d", counter);
    // Invalidate text area, which will result in it being redrawn in next tick.
    textCounter.invalidate();
}

void MainView::buttonDownClicked()
{   //harebit
    counter--;
    Unicode::snprintf(textCounterBuffer, TEXTCOUNTER_SIZE, "%d", counter);
    // Invalidate text area, which will result in it being redrawn in next tick.
    textCounter.invalidate();
}
