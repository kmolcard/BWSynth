#include "PluginEditor.h"
#include "KMOLookAndFeel.h"


namespace kmo
{

KMOLookAndFeel myLF;

const auto knobW = 95;
const auto labelH = 20;

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), paramsRef (p.getParameters())
{
    juce::ignoreUnused (processorRef);
 
    setLookAndFeel (&myLF);
    
    for (auto& param : paramsRef)
    {
        param->addListener (this);

        auto s = std::make_unique<juce::Slider>();
        s->setRange (0.f, 1.f);
        s->setLookAndFeel (&myLF);
        s->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        s->setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        s->setPopupDisplayEnabled (true, false, this);
        s->setValue (param->getValue(), juce::NotificationType::dontSendNotification);
        s->addListener (this);
        addAndMakeVisible (s.get());
        sliders.push_back (std::move (s));

        auto l = std::make_unique<juce::Label>();
        l->setText (param->getName (32),
            juce::NotificationType::dontSendNotification);
        l->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (l.get());
        labels.push_back (std::move (l));
    }

    auto w{800};
    auto h{40 + knobW+labelH};
    auto nbParam{paramsRef.size()};
    auto nbParamPerLine{w / (knobW+5)};
    while(nbParam > nbParamPerLine)
    {
        h += knobW+labelH;
        nbParam -= nbParamPerLine;
    }
    setSize (w, h);

    startTimer (30);
    
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };
}

PluginEditor::~PluginEditor()
{
    const auto& params = processorRef.getParameters();
    for (auto& param : params)
    {
        param->removeListener (this);
    }
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll (
        getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom (50);
    inspectButton.setBounds (0, 0, 100, 30);

    auto w = knobW;
    auto h = knobW;
    auto x_start = std::max(0, getWidth() / 2 - (w + 5) * static_cast<int> (sliders.size()) / 2);
    auto x = x_start;
    auto y_start = 40;
    auto y = y_start;
    for (auto& s : sliders)
    {
        if(x + w >= getWidth())
        {
            x = x_start;
            y += h + labelH;
        }
        s->setBounds (x, y, w, h);
        x += w + 5;
    }
    
    x = x_start;
    y = y_start + h;
    h = labelH;
    for (auto& l : labels)
    {
        if(x + w >= getWidth())
        {
            x = x_start;
            y += knobW + labelH;
        }
        l->setBounds (x, y, w, h);
        x += w + 5;
    }
}

void PluginEditor::sliderValueChanged (juce::Slider* slider)
{
    for (size_t i = 0; i < std::size (sliders); ++i)
    {
        if (sliders[i].get() == slider)
        {
            auto& params = processorRef.getParameters();
            params[static_cast<int> (i)]->setValueNotifyingHost (static_cast<float> (slider->getValue()));
        }
    }
}

void PluginEditor::parameterValueChanged (int parameterIndex, float newValue)
{
    auto paramId = static_cast<size_t> (parameterIndex);
    assert (paramId < mParamChanged.size());

    if (!juce::MessageManager::getInstance()->isThisTheMessageThread())
    {
        mParamChanged[paramId] = true;
    }
    else
    {
        auto& slider = *sliders[paramId];
        auto value = static_cast<double> (newValue);
        slider.setValue (value, juce::NotificationType::dontSendNotification);
    }
}

void PluginEditor::timerCallback()
{
    for (size_t i = 0; i < NUM_PARAMETERS; ++i)
    {
        if (mParamChanged[i])
        {
            auto value = static_cast<double> (paramsRef[(int) (i)]->getValue());
            sliders[i]->setValue (value, juce::NotificationType::dontSendNotification);
            mParamChanged[i] = false;
        }
    }
}

}
