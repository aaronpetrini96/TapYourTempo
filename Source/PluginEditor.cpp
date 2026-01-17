/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapYourTempoAudioProcessorEditor::TapYourTempoAudioProcessorEditor (TapYourTempoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setWantsKeyboardFocus(true);
    setAlwaysOnTop(true);
    
    for (auto& button : textButtons)
    {
        button->setRadioGroupId(123);
//        button->setClickingTogglesState(true);
        button->setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::white);
        button->setColour(juce::TextButton::ColourIds::buttonColourId, offColor);
        button->setColour(juce::TextButton::ColourIds::buttonOnColourId, onColor);
        button->setLookAndFeel(&textButtonLAF);
        addAndMakeVisible(*button);
    }
    
    halfTempoButton.onClick = [this]
    {
        updateMultiplier(1);
    };
    doubleTempoButton.onClick = [this] {updateMultiplier(2);};
    
    bpmLabel.setFont(juce::Font(juce::FontOptions("Barber Chop", 46.f, juce::Font::FontStyleFlags::plain)));
    bpmLabel.setText("TAP TEMPO PRESSING THE SPACE BAR", juce::dontSendNotification);
    bpmLabel.setJustificationType(juce::Justification::centred);
    bpmLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    bpmLabel.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible(bpmLabel);
    
    
    
    setSize (guiWidth, guiHeight);
    startTimerHz(30);
}

TapYourTempoAudioProcessorEditor::~TapYourTempoAudioProcessorEditor()
{
}

//==============================================================================
void TapYourTempoAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    const auto now = juce::Time::getMillisecondCounter();
    const bool flash = (now - lastFlashTime) < 120;
    
    
    g.setColour(flash ? onColor : offColor);
    auto bounds = bpmLabel.getBounds();
    g.fillRect(bounds);
}

void TapYourTempoAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    bpmLabel.setBounds(bounds.removeFromTop(250));
    
    halfTempoButton.setBounds(bounds.removeFromLeft(guiWidth / 2));
    doubleTempoButton.setBounds(bounds);
}

void TapYourTempoAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown())
    {
        grabKeyboardFocus();
        audioProcessor.tap();
        lastFlashTime = juce::Time::getMillisecondCounter();
    }
}

bool TapYourTempoAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        keyWasPressed = true;
        audioProcessor.tap();
        lastFlashTime = juce::Time::getMillisecondCounter();
        return true;
    }
    
    return false;
}

void TapYourTempoAudioProcessorEditor::timerCallback()
{
    if (keyWasPressed)
    {
        bpmLabel.setText(juce::String(audioProcessor.getBPM() * multiplier, 2) + " BPM", juce::dontSendNotification);
        repaint();
    }
}

void TapYourTempoAudioProcessorEditor::updateMultiplier(int btnIndex)
{
    if (btnIndex == 1)
    {
        isDoubleOn = false;
        doubleTempoButton.setToggleState(false, juce::dontSendNotification);
        halfTempoButton.setToggleState((isHalfOn) ? false : true, juce::dontSendNotification);
        multiplier = (isHalfOn) ? 1.0f : 0.5f;
        isHalfOn = !isHalfOn;
    }
    else if (btnIndex == 2)
    {
        isHalfOn = false;
        halfTempoButton.setToggleState(false, juce::dontSendNotification);
        doubleTempoButton.setToggleState((isDoubleOn) ? false : true, juce::dontSendNotification);
        multiplier = (isDoubleOn) ? 1.0f : 2.0f;
        isDoubleOn = !isDoubleOn;
    }
}
