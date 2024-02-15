#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace kmo
{

inline juce::String getParamID (juce::AudioProcessorParameter* param)
{
    auto result = juce::String();
    if (auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*> (param))
        result = paramWithID->paramID;

    result = param->getName (50);
    return result.removeCharacters (" /");
}

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor (
        BusesProperties()
#if !JucePlugin_IsMidiEffect
    #if !JucePlugin_IsSynth
            .withInput ("Input", juce::AudioChannelSet::stereo(), true)
    #endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
, mState(*this, &undoManager)
{
    mParamIdMappings[mState.params.volume->paramID.toStdString()] = p_volume;
    mParamIdMappings[mState.params.masterTune->paramID.toStdString()] = p_master_tune;
    mParamIdMappings[mState.params.portamento->paramID.toStdString()] = p_portamento;
    mParamIdMappings[mState.params.modMix->paramID.toStdString()] = p_mod_mix;
    mParamIdMappings[mState.params.vco1Mod->paramID.toStdString()] = p_vco1_mod;
    mParamIdMappings[mState.params.vco1Coarse->paramID.toStdString()] = p_vco1_coarse;
    mParamIdMappings[mState.params.vco1Fine->paramID.toStdString()] = p_vco1_fine;
    mParamIdMappings[mState.params.vco1Wave->paramID.toStdString()] = p_vco1_waveform;
    mParamIdMappings[mState.params.vco1PW->paramID.toStdString()] = p_vco1_pw_slope;
    mParamIdMappings[mState.params.vco1Level->paramID.toStdString()] = p_vco1_level;
    mParamIdMappings[mState.params.vco2Mod->paramID.toStdString()] = p_vco2_mod;
    mParamIdMappings[mState.params.vco2Coarse->paramID.toStdString()] = p_vco2_coarse;
    mParamIdMappings[mState.params.vco2Fine->paramID.toStdString()] = p_vco2_fine;
    mParamIdMappings[mState.params.vco2Wave->paramID.toStdString()] = p_vco2_waveform;
    mParamIdMappings[mState.params.vco2PW->paramID.toStdString()] = p_vco2_pw_slope;
    mParamIdMappings[mState.params.vco2Level->paramID.toStdString()] = p_vco2_level;
    mParamIdMappings[mState.params.vco3Mod->paramID.toStdString()] = p_vco3_kbd;
    mParamIdMappings[mState.params.vco3Coarse->paramID.toStdString()] = p_vco3_coarse;
    mParamIdMappings[mState.params.vco3Fine->paramID.toStdString()] = p_vco3_fine;
    mParamIdMappings[mState.params.vco3Wave->paramID.toStdString()] = p_vco3_waveform;
    mParamIdMappings[mState.params.vco3PW->paramID.toStdString()] = p_vco3_pw_slope;
    mParamIdMappings[mState.params.vco3Level->paramID.toStdString()] = p_vco3_level;
    mParamIdMappings[mState.params.noiseColor->paramID.toStdString()] = p_noise_color;
    mParamIdMappings[mState.params.noiseLevel->paramID.toStdString()] = p_noise_level;
    mParamIdMappings[mState.params.vcfMod->paramID.toStdString()] = p_vcf_mod;
    mParamIdMappings[mState.params.vcfKbdCtrl->paramID.toStdString()] = p_vcf_kbd_ctrl;
    mParamIdMappings[mState.params.vcfCutoff->paramID.toStdString()] = p_vcf_cutoff;
    mParamIdMappings[mState.params.vcfQ->paramID.toStdString()] = p_vcf_Q;
    mParamIdMappings[mState.params.vcfContour->paramID.toStdString()] = p_vcf_contour;
    mParamIdMappings[mState.params.vcfAttack->paramID.toStdString()] = p_vcf_attack;
    mParamIdMappings[mState.params.vcfDecay->paramID.toStdString()] = p_vcf_decay;
    mParamIdMappings[mState.params.vcfSustain->paramID.toStdString()] = p_vcf_sustain;
    mParamIdMappings[mState.params.vcfRelease->paramID.toStdString()] = p_vcf_release;
    mParamIdMappings[mState.params.vcaAttack->paramID.toStdString()] = p_vca_attack;
    mParamIdMappings[mState.params.vcaDecay->paramID.toStdString()] = p_vca_decay;
    mParamIdMappings[mState.params.vcaSustain->paramID.toStdString()] = p_vca_sustain;
    mParamIdMappings[mState.params.vcaRelease->paramID.toStdString()] = p_vca_release;
    
    for(auto p : getParameters())
    {
        p->addListener(this);
        
        auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*> (p);
        auto paramID{paramWithID->paramID.toStdString()};
        mParamChanged[paramID] = false;
    }
}

PluginProcessor::~PluginProcessor()
{  
    for (auto& p : getParameters())
    {
        p->removeListener (this);
    }
}

//==============================================================================
const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const { return 0.0; }

int PluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
        // programs, so this should be at least 1, even if you're not really
        // implementing programs.
}

int PluginProcessor::getCurrentProgram() { return 0; }

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index,
    const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mSampleRate = sampleRate;
    mNbSamplePerUpdate = static_cast<int32_t>(mSampleRate / kFramerate);
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    for (auto& e : mEngine)
    {
        P_INIT (&e);
        P_SET_SAMPLE_RATE (&e, static_cast<float> (sampleRate));
        P_RESET (&e);
        
        for(auto p : getParameters())
        {
            if(auto paramWithID{dynamic_cast<juce::AudioProcessorParameterWithID*>(p)}){
                auto index = mParamIdMappings[paramWithID->paramID.toStdString()];
                auto value{p->getValue()};
                P_SET_PARAMETER (&e, index, value);
            }
        }
    }
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
    #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
#endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    
    for(auto p : getParameters())
    {
        if(auto fp{dynamic_cast<chowdsp::FloatParameter*>(p)}){
            auto paramID{fp->paramID.toStdString()};
            if(mParamChanged[paramID]){
                for (auto& e : mEngine){
                    auto index = mParamIdMappings[paramID];
                    auto value{p->getValue()};
                    P_SET_PARAMETER (&e, index, value);
                }
                mParamChanged[paramID] = false;
            }
        }
    }
    
#if JucePlugin_IsSynth
    for(auto midiMsg : midiMessages)
    {
        auto m {midiMsg.getMessage()};
        if(m.isNoteOn())
        {
            for (size_t c = 0; c < mEngine.size(); ++c)
            {
                P_NOTE_ON(&mEngine[c], (char)m.getNoteNumber(), (char)m.getVelocity());
            }
        }
        if(m.isNoteOff())
        {
            for (size_t c = 0; c < mEngine.size(); ++c)
            {
                P_NOTE_OFF(&mEngine[c], (char)m.getNoteNumber());
            }
        }
    }
#endif
    
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto nSamples = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto wChannels = buffer.getArrayOfWritePointers();
    auto rChannels = buffer.getArrayOfReadPointers();
    for (size_t c = 0; c < mEngine.size(); ++c)
    {
        P_PROCESS (&mEngine[c], const_cast<const float**> (&rChannels[c]), const_cast<float**> (&wChannels[c]), nSamples);
    }
    
    
    mNbSampleSinceLastUpdate += nSamples;
    if(mNbSampleSinceLastUpdate >= mNbSamplePerUpdate){
//        mNbSampleSinceLastUpdate -= mNbSamplePerUpdate;
//        for(const auto p : mOutParams){
//            auto index = (size_t)p;
//            assert(index < mState.params.count());
//            auto val = P_GET_PARAMETER (&mEngine[0], static_cast<int> (p));
//            mParams[index]->setValueNotifyingHost(val);
//        }
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Serializes your parameters, and any other potential data into an XML:

    juce::ValueTree params ("Params");

    for (auto& param : getParameters())
    {
        juce::ValueTree paramTree (getParamID (param));
        paramTree.setProperty ("Value", param->getValue(), nullptr);
        params.appendChild (paramTree, nullptr);
    }

    juce::ValueTree pluginPreset (getName().removeCharacters (" /"));
    pluginPreset.appendChild (params, nullptr);
    // This a good place to add any non-parameters to your preset

    copyXmlToBinary (*pluginPreset.createXml(), destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Loads your parameters, and any other potential data from an XML:

    auto xml = getXmlFromBinary (data, sizeInBytes);

    if (xml != nullptr)
    {
        auto preset = juce::ValueTree::fromXml (*xml);
        auto params = preset.getChildWithName ("Params");

        for (auto& param : getParameters())
        {
            auto paramTree = params.getChildWithName (getParamID (param));

            if (paramTree.isValid())
                param->setValueNotifyingHost (paramTree["Value"]);
        }

        // Load your non-parameter data now
    }
}


void PluginProcessor::parameterValueChanged (int parameterIndex, float)
{
    auto index = static_cast<size_t> (parameterIndex);
    auto p{getParameters()[parameterIndex]};
    auto paramWithID = dynamic_cast<juce::AudioProcessorParameterWithID*> (p);
    auto paramID{paramWithID->paramID.toStdString()};
    mParamChanged[paramID] = true;
}

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new kmo::PluginProcessor();
}
