#include "KMOLookAndFeel.h"

#include <melatonin_blur/melatonin_blur.h>

namespace kmo
{
    void KMOLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
    {
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);

        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);

        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour (outline);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                bounds.getCentreY(),
                arcRadius,
                arcRadius,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setColour (fill);
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        auto thumbWidth = lineW * 2.0f;
        juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
            bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

        const auto& c { slider.findColour (juce::Slider::thumbColourId) };
        g.setColour (c);

        juce::Path myValueTrack;
        auto e = juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint);
        myValueTrack.addEllipse (e);
        melatonin::DropShadow shadow = { { c, 8, { 0, 0 } } };
        shadow.render (g, myValueTrack);
        g.fillEllipse (e);
        g.setColour (juce::Colours::black);
        g.drawEllipse (e, 1.f);
    }

    void KMOLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
        if (slider.isBar())
        {
            g.setColour (slider.findColour (juce::Slider::trackColourId));
            g.fillRect (slider.isHorizontal() ? juce::Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                              : juce::Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));

            drawLinearSliderOutline (g, x, y, width, height, style, slider);
        }
        else
        {
            auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

            auto trackWidth = juce::jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

            juce::Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

            juce::Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                slider.isHorizontal() ? startPoint.y : (float) y);

            juce::Path backgroundTrack;
            backgroundTrack.startNewSubPath (startPoint);
            backgroundTrack.lineTo (endPoint);
            g.setColour (slider.findColour (juce::Slider::backgroundColourId));
            g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

            juce::Path valueTrack;
            juce::Point<float> minPoint, maxPoint, thumbPoint;

            if (isTwoVal || isThreeVal)
            {
                minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

                if (isThreeVal)
                    thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                        slider.isHorizontal() ? (float) height * 0.5f : sliderPos };

                maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
                auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

                minPoint = startPoint;
                maxPoint = { kx, ky };
            }

            auto thumbWidth = getSliderThumbRadius (slider);

            valueTrack.startNewSubPath (minPoint);
            valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
            g.setColour (slider.findColour (juce::Slider::trackColourId));
            g.strokePath (valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

            if (!isTwoVal)
            {
                juce::Path myValueTrack;
                auto e = juce::Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint);
                auto c { slider.findColour (juce::Slider::thumbColourId) };
                myValueTrack.addEllipse (e);
                melatonin::DropShadow shadow = { { c, 8, { 0, 0 } } };
                shadow.render (g, myValueTrack);

                g.setColour (c);
                g.fillEllipse (e);
            }

            if (isTwoVal || isThreeVal)
            {
                auto sr = juce::jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
                auto pointerColour = slider.findColour (juce::Slider::thumbColourId);

                if (slider.isHorizontal())
                {
                    drawPointer (g, minSliderPos - sr, juce::jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f), trackWidth * 2.0f, pointerColour, 2);

                    drawPointer (g, maxSliderPos - trackWidth, juce::jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f), trackWidth * 2.0f, pointerColour, 4);
                }
                else
                {
                    drawPointer (g, juce::jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f), minSliderPos - trackWidth, trackWidth * 2.0f, pointerColour, 1);

                    drawPointer (g, juce::jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr, trackWidth * 2.0f, pointerColour, 3);
                }
            }

            if (slider.isBar())
                drawLinearSliderOutline (g, x, y, width, height, style, slider);
        }
    }
} 
