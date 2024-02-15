#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace kmo
{ 

class KMOLookAndFeel : public juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider);
};

}
