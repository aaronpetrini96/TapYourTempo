/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapYourTempoAudioProcessor::TapYourTempoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TapYourTempoAudioProcessor::~TapYourTempoAudioProcessor()
{
}

//==============================================================================
const juce::String TapYourTempoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

double TapYourTempoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapYourTempoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TapYourTempoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapYourTempoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TapYourTempoAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapYourTempoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TapYourTempoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TapYourTempoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapYourTempoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TapYourTempoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    [[maybe_unused]] auto totalNumInputChannels  = getTotalNumInputChannels();
    [[maybe_unused]] auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    buffer.clear();
    
}

//==============================================================================
bool TapYourTempoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TapYourTempoAudioProcessor::createEditor()
{
    return new TapYourTempoAudioProcessorEditor (*this);
}

//==============================================================================
void TapYourTempoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void TapYourTempoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void TapYourTempoAudioProcessor::tap()
{
    const double now = juce::Time::getMillisecondCounterHiRes() * 0.001;
    
    if (lastTapTime > 0.0)
    {
        const double delta = now - lastTapTime;
        
        if (delta > 0.15 && delta < 2.5)
        {
            intervals[intervalIndex++ % intervals.size()] = delta;
            
            const int count = std::min(intervalIndex, (int)intervals.size());
            
            if (count >= 2)
            {
                double sum = 0.0;
                for (int i = 0; i < count; ++i)
                    sum += intervals[i];
                
                const double avg = sum / count;
                float bpmNumber = static_cast<float>(60.0 / avg);
                bpm.store(bpmNumber);
            }
        }
        else
        {
            intervalIndex = 0;
        }
    }
    
    lastTapTime = now;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapYourTempoAudioProcessor();
}
