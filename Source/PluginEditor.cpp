/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Yc_dalayAudioProcessorEditor::Yc_dalayAudioProcessorEditor (Yc_dalayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    auto& params=processor.getParameters();
    //@
    AudioParameterFloat* drywetParameter=(AudioParameterFloat*) params.getUnchecked(0);
    
    ycDryWetSlider.setBounds(0, 0, 100, 100);
    ycDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ycDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    ycDryWetSlider.setRange(drywetParameter->range.start, drywetParameter->range.end);
    ycDryWetSlider.setValue(*drywetParameter);
    addAndMakeVisible(ycDryWetSlider);
    
    ycDryWetSlider.onValueChange = [this,drywetParameter]{*drywetParameter = ycDryWetSlider.getValue();};
    ycDryWetSlider.onDragStart = [drywetParameter]{drywetParameter->beginChangeGesture();};
    ycDryWetSlider.onDragEnd = [drywetParameter]{drywetParameter->endChangeGesture();};
    
    //@
    AudioParameterFloat* feedbackParameter =(AudioParameterFloat*) params.getUnchecked(1);
    
    ycFeedbackSlider.setBounds(100, 0, 100, 100);
    ycFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ycFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    ycFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    ycFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(ycFeedbackSlider);
    
    ycFeedbackSlider.onValueChange = [this,feedbackParameter]{*feedbackParameter = ycFeedbackSlider.getValue();};
    ycFeedbackSlider.onDragStart = [feedbackParameter]{feedbackParameter->beginChangeGesture();};
    ycFeedbackSlider.onDragEnd = [feedbackParameter]{feedbackParameter->endChangeGesture();};
    //@
    AudioParameterFloat* delaytimeParameter=(AudioParameterFloat*) params.getUnchecked(2);
    
    ycDelayTimeSlider.setBounds(200, 0, 100, 100);
    ycDelayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ycDelayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    ycDelayTimeSlider.setRange(delaytimeParameter->range.start, delaytimeParameter->range.end);
    ycDelayTimeSlider.setValue(*delaytimeParameter);
    addAndMakeVisible(ycDelayTimeSlider);
    
    ycDelayTimeSlider.onValueChange = [this,delaytimeParameter]{*delaytimeParameter = ycDryWetSlider.getValue();};
    ycDelayTimeSlider.onDragStart = [delaytimeParameter]{delaytimeParameter->beginChangeGesture();};
    ycDelayTimeSlider.onDragEnd = [delaytimeParameter]{delaytimeParameter->endChangeGesture();};
    
}

Yc_dalayAudioProcessorEditor::~Yc_dalayAudioProcessorEditor()
{
}

//==============================================================================
void Yc_dalayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Dotty Junkie Delay", getLocalBounds(), Justification::centred, 1);
}

void Yc_dalayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
