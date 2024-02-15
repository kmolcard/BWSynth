#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

namespace kmo
{
//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::AudioProcessorParameter::Listener,
                     private juce::Slider::Listener,
                     private juce::Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    virtual void sliderValueChanged (juce::Slider* slider) override;

    void timerCallback() override;

    // AudioProcessorParameter::Listener
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int /*parameterIndex*/,
        bool /*gestureIsStarting*/) override {}

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    const juce::Array<juce::AudioProcessorParameter*>& paramsRef;

    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect" }, rebuildButton { "Rebuild" };

    std::array<std::atomic_bool, NUM_PARAMETERS> mParamChanged { false };
    std::vector<std::unique_ptr<juce::Slider>> sliders;
    std::vector<std::unique_ptr<juce::Label>> labels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

}
