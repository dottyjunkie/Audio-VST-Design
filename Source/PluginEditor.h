/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Yc_dalayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Yc_dalayAudioProcessorEditor (Yc_dalayAudioProcessor&);
    ~Yc_dalayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Yc_dalayAudioProcessor& processor;
    
    Slider ycDryWetSlider;
    Slider ycFeedbackSlider;
    Slider ycDelayTimeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Yc_dalayAudioProcessorEditor)
};
