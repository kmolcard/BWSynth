#pragma once

#include <chowdsp_plugin_state/chowdsp_plugin_state.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wfloat-conversion"

#include "examples/synthpp_poly/src/config.h"

#pragma GCC diagnostic pop


enum class Octave{
    kLow = 1,
    k32 = 2,
    k16 = 4,
    k8 = 8,
    k4 = 16,
    k2 = 32,
};

enum class Waveform{
    Triangle = 1,
    Saw = 2,
    Ramp = 4,
    Square = 8,
    NarrowRect = 16,
    WideRect = 32,
};

enum class NoiseColor{
    White = 1,
    Pink = 2,
};

enum class VCFKbdCtrl{
    None = 1,
    Normal = 2,
    Double = 2,
};

struct PluginParameterState : chowdsp::ParamHolder
{
    
    PluginParameterState()
    {
        add(volume,
            masterTune,
            portamento,
            modMix,
            vco1Mod,
            vco1Coarse,
            vco1Fine,
            vco1Wave,
            vco1PW,
            vco1Level,
            vco2Mod,
            vco2Coarse,
            vco2Fine,
            vco2Wave,
            vco2PW,
            vco2Level,
            vco3Mod,
            vco3Coarse,
            vco3Fine,
            vco3Wave,
            vco3PW,
            vco3Level,
            noiseColor,
            noiseLevel,
            vcfMod,
            vcfKbdCtrl,
            vcfCutoff,
            vcfQ,
            vcfContour,
            vcfAttack,
            vcfDecay,
            vcfSustain,
            vcfRelease,
            vcaAttack,
            vcaDecay,
            vcaSustain,
            vcaRelease);
    }
    
    
    chowdsp::GainDBParameter::Ptr volume {
        juce::ParameterID { "Volume", 100 },
        "Volume",
        juce::NormalisableRange { -45.0f, 12.0f },
        -24.0f
    };
    chowdsp::FloatParameter::Ptr masterTune {
        juce::ParameterID { "MasterTune", 100 },
        "Master Tune",
        juce::NormalisableRange (-1.f, 1.f),
        0.f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::TimeMsParameter::Ptr portamento {
        juce::ParameterID { "Portamento", 100 },
        "Portamento",
        chowdsp::ParamUtils::createNormalisableRange (0.f, 5000.0f, 1000.0f),
        0.f
    };
    chowdsp::PercentParameter::Ptr modMix {
        juce::ParameterID { "Mod mix", 100 },
        "Modulation mix",
        0.f
    };

    //VCO 1
    chowdsp::PercentParameter::Ptr vco1Mod {
        juce::ParameterID { "VCO1 mod", 100 },
        "VCO1 Modulation",
        0.f
    };
    chowdsp::EnumChoiceParameter<Octave>::Ptr vco1Coarse {
        juce::ParameterID { "VCO1 coarse", 100 },
        "VCO1 coarse",
        Octave::k8
    };
    chowdsp::FloatParameter::Ptr vco1Fine {
        juce::ParameterID { "VCO1 Fine", 100 },
        "VCO1 Fine",
        juce::NormalisableRange (-1.f, 1.f),
        0.f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::EnumChoiceParameter<Waveform>::Ptr vco1Wave {
        juce::ParameterID { "VCO1 Wave", 100 },
        "VCO1 Wave",
        Waveform::Saw
    };
    chowdsp::PercentParameter::Ptr vco1PW {
        juce::ParameterID { "VCO1 pw/slope", 100 },
        "VCO1 pulse width/slope",
        0.5f
    };
    chowdsp::GainDBParameter::Ptr vco1Level {
        juce::ParameterID { "VCO1 Level", 100 },
        "VCO1 Level",
        juce::NormalisableRange { -45.0f, 12.0f },
        0.f
    };

    //VCO 2
    chowdsp::PercentParameter::Ptr vco2Mod {
        juce::ParameterID { "VCO2 mod", 100 },
        "VCO2 Modulation",
        0.f
    };
    chowdsp::EnumChoiceParameter<Octave>::Ptr vco2Coarse {
        juce::ParameterID { "VCO2 coarse", 100 },
        "VCO2 coarse",
        Octave::k8
    };
    chowdsp::FloatParameter::Ptr vco2Fine {
        juce::ParameterID { "VCO2 Fine", 100 },
        "VCO2 Fine",
        juce::NormalisableRange (-1.f, 1.f),
        0.f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::EnumChoiceParameter<Waveform>::Ptr vco2Wave {
        juce::ParameterID { "VCO2 Wave", 100 },
        "VCO2 Wave",
        Waveform::Saw
    };
    chowdsp::PercentParameter::Ptr vco2PW {
        juce::ParameterID { "VCO2 pw/slope", 100 },
        "VCO2 pulse width/slope",
        0.5f
    };
    chowdsp::GainDBParameter::Ptr vco2Level {
        juce::ParameterID { "VCO2 Level", 100 },
        "VCO2 Level",
        juce::NormalisableRange { -45.0f, 12.0f },
        -45.0f
    };


    //VCO 3
    chowdsp::PercentParameter::Ptr vco3Mod {
        juce::ParameterID { "VCO3 mod", 100 },
        "VCO3 Modulation",
        0.f
    };
    chowdsp::EnumChoiceParameter<Octave>::Ptr vco3Coarse {
        juce::ParameterID { "VCO3 coarse", 100 },
        "VCO3 coarse",
        Octave::k8
    };
    chowdsp::FloatParameter::Ptr vco3Fine {
        juce::ParameterID { "VCO3 Fine", 100 },
        "VCO3 Fine",
        juce::NormalisableRange (-1.f, 1.f),
        0.f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::EnumChoiceParameter<Waveform>::Ptr vco3Wave {
        juce::ParameterID { "VCO3 Wave", 100 },
        "VCO3 Wave",
        Waveform::Saw
    };
    chowdsp::PercentParameter::Ptr vco3PW {
        juce::ParameterID { "VCO3 pw/slope", 100 },
        "VCO3 pulse width/slope",
        0.5f
    };
    chowdsp::GainDBParameter::Ptr vco3Level {
        juce::ParameterID { "VCO3 Level", 100 },
        "VCO3 Level",
        juce::NormalisableRange { -45.0f, 12.0f },
        -45.0f
    };

    //Noise
    chowdsp::EnumChoiceParameter<NoiseColor>::Ptr noiseColor {
        juce::ParameterID { "Noise color", 100 },
        "Noise color",
        NoiseColor::White
    };
    chowdsp::GainDBParameter::Ptr noiseLevel {
        juce::ParameterID { "Noise Level", 100 },
        "Noise Level",
        juce::NormalisableRange { -45.0f, 12.0f },
        -45.0f
    };

    //VCF
    chowdsp::PercentParameter::Ptr vcfMod {
        juce::ParameterID { "VCF mod", 100 },
        "VCF Modulation",
        0.f
    };
    chowdsp::EnumChoiceParameter<VCFKbdCtrl>::Ptr vcfKbdCtrl {
        juce::ParameterID { "VCF kbd ctrl", 100 },
        "VCF keyboard control",
        VCFKbdCtrl::None
    };
    chowdsp::FreqHzParameter::Ptr vcfCutoff {
        juce::ParameterID { "VCF Cutoff", 100 },
        "VCF Cutoff",
        chowdsp::ParamUtils::createNormalisableRange (20.0f, 20000.0f, 2000.0f),
        1000.0f
    };
    chowdsp::FloatParameter::Ptr vcfQ {
        juce::ParameterID { "VCF Q", 100 },
        "VCF Q",
        juce::NormalisableRange (0.f, 1.f),
        0.f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::PercentParameter::Ptr vcfContour {
        juce::ParameterID { "VCF Contour", 100 },
        "VCF Contour",
        0.f
    };
    chowdsp::TimeMsParameter::Ptr vcfAttack {
        juce::ParameterID { "VCF Attack", 100 },
        "VCF Attack",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        100.f
    };
    chowdsp::TimeMsParameter::Ptr vcfDecay {
        juce::ParameterID { "VCF Decay", 100 },
        "VCF Decay",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        200.f
    };
    chowdsp::PercentParameter::Ptr vcfSustain {
        juce::ParameterID { "VCF Sustain", 100 },
        "VCF Sustain",
        0.5f
    };
    chowdsp::TimeMsParameter::Ptr vcfRelease {
        juce::ParameterID { "VCF Release", 100 },
        "VCF Release",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        200.f
    };
    
    //VCA
    chowdsp::TimeMsParameter::Ptr vcaAttack {
        juce::ParameterID { "VCA Attack", 100 },
        "VCA Attack",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        100.f
    };
    chowdsp::TimeMsParameter::Ptr vcaDecay {
        juce::ParameterID { "VCA Decay", 100 },
        "VCA Decay",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        200.f
    };
    chowdsp::PercentParameter::Ptr vcaSustain {
        juce::ParameterID { "VCA Sustain", 100 },
        "VCA Sustain",
        0.5f
    };
    chowdsp::TimeMsParameter::Ptr vcaRelease {
        juce::ParameterID { "VCA Release", 100 },
        "VCA Release",
        chowdsp::ParamUtils::createNormalisableRange (100.0f, 5000.0f, 1000.0f),
        200.f
    };
    
//    { "A440", "A440", "", 0, 0, 1, 0.f },
//    { "Level", "Level", "", 1, 0, 0, 0.f }
    
};

struct PluginNonParameterState : chowdsp::NonParamState
{
    PluginNonParameterState()
    {
        addStateValues ({ &editorBounds });
    }

    chowdsp::StateValue<juce::Point<int>> editorBounds { "editor_bounds", { 300, 500 } };
};
