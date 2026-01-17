/*
  ==============================================================================

    TextButtonLAF.h
    Created: 16 Jan 2026 10:15:02pm
    Author:  Aaron Petrini

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TextButtonLAF : public juce::LookAndFeel_V4
{
    public:
    
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override
    {
        return juce::Font (juce::FontOptions("Barber Chop", buttonHeight * 0.3f, juce::Font::FontStyleFlags::plain));
    }
};
