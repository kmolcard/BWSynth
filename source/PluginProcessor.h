#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#if (MSVC)
    #include "ipps.h"
#endif

#include "PluginConfig.h"

namespace kmo
{


using State = chowdsp::PluginStateImpl<PluginParameterState, PluginNonParameterState>;

class PluginProcessor : public juce::AudioProcessor,
public juce::AudioProcessorParameter::Listener
{
public:
    PluginProcessor();
    ~PluginProcessor() override;
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // AudioProcessorParameter::Listener
     void parameterValueChanged (int parameterIndex, float newValue) override;
     void parameterGestureChanged (int /*parameterIndex*/, bool /*gestureIsStarting*/) override {}


    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
    std::array<P_TYPE, 1> mEngine;
    
    State mState;

    double mSampleRate = 44100.;
    int32_t mNbSampleSinceLastUpdate = 0;
    int32_t mNbSamplePerUpdate = 0;
    static constexpr auto kFramerate = 60; //fps
    
    std::vector<int> mOutParams;
    
    juce::UndoManager undoManager { 3000 };
    std::unordered_map<std::string, int> mParamIdMappings;
    std::unordered_map<std::string, std::atomic_bool> mParamChanged;
};

}
