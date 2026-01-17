/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TextButtonLAF.h"

//==============================================================================
/**
*/
class TapYourTempoAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    TapYourTempoAudioProcessorEditor (TapYourTempoAudioProcessor&);
    ~TapYourTempoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const juce::MouseEvent&) override;
    bool keyPressed (const juce::KeyPress&) override;
    
    void updateMultiplier(int);
private:
    void timerCallback() override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TapYourTempoAudioProcessor& audioProcessor;
    TextButtonLAF textButtonLAF;
    
    const int guiWidth = 400;
    const int guiHeight = 300;
    
    const juce::Colour onColor = juce::Colour(85, 135, 235).darker(1.f);
    const juce::Colour offColor = juce::Colours::darkgrey.darker(2.f);
    
    float multiplier = 1.0f;
    bool isHalfOn = false;
    bool isDoubleOn = false;
    
    juce::Label bpmLabel;
    juce::uint32 lastFlashTime = 0;
    bool keyWasPressed {false};
    
    juce::TextButton halfTempoButton {"x0.5"}, doubleTempoButton {"x2"};
    std::vector<juce::TextButton*> textButtons {&halfTempoButton, &doubleTempoButton};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapYourTempoAudioProcessorEditor)
};
