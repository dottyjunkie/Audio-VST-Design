/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Yc_dalayAudioProcessor::Yc_dalayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(DryWetParameter = new AudioParameterFloat("drywet",
                                                           "DryWet",
                                                           0.0,
                                                           1.0,
                                                           0.5
                                                           ));
    addParameter(FeedbackParameter = new AudioParameterFloat("feedback",
                                                           "Feedback",
                                                           0,
                                                           0.9,
                                                           0.5
                                                           ));
    addParameter(DelayTimeParameter = new AudioParameterFloat("delaytime",
                                                           "DelayTime",
                                                           0.01,
                                                           MAX_DELAY_TIME,
                                                           0.5
                                                           ));
    ycCircularBufferLeft=nullptr;
    ycCircularBufferRight=nullptr;
    
    ycCircularBufferWriteHead=0;
    ycCircularBufferLength=0;
    ycDelayTimeInSample=0;
    ycDelayReadHead=0;
    feedbackLeft=0;
    feedbackRight=0;
    ycDryWet=0.6;
}

Yc_dalayAudioProcessor::~Yc_dalayAudioProcessor()
{
    if(ycCircularBufferLeft!=nullptr)
    {
        delete []ycCircularBufferLeft;
    }
    
    if(ycCircularBufferRight!=nullptr)
    {
        delete []ycCircularBufferRight;
    }
}

//==============================================================================
const String Yc_dalayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Yc_dalayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Yc_dalayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Yc_dalayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Yc_dalayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Yc_dalayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Yc_dalayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Yc_dalayAudioProcessor::setCurrentProgram (int index)
{
}

const String Yc_dalayAudioProcessor::getProgramName (int index)
{
    return {};
}

void Yc_dalayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Yc_dalayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ycCircularBufferLength=sampleRate* *DelayTimeParameter;
    ycDelayTimeInSample=sampleRate*0.5;
    
    if(ycCircularBufferLeft==nullptr)
    {
        ycCircularBufferLeft = new float[ycCircularBufferLength];
    }
    
    if(ycCircularBufferRight==nullptr)
    {
        ycCircularBufferRight = new float[ycCircularBufferLength];
    }
    
    ycCircularBufferWriteHead=0;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Yc_dalayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Yc_dalayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void Yc_dalayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float* inputLeft=buffer.getWritePointer(0);
    float* inputRight=buffer.getWritePointer(1);
    
    for(int i=0;i<buffer.getNumSamples();i++)
    {
        ycCircularBufferLength=getSampleRate()* *DelayTimeParameter;
        ycCircularBufferLeft[ycCircularBufferWriteHead]=inputLeft[i]+feedbackLeft;
        ycCircularBufferRight[ycCircularBufferWriteHead]=inputRight[i]+feedbackRight;
        
        ycDelayReadHead=ycCircularBufferWriteHead - ycDelayTimeInSample;
        if(ycDelayReadHead<0)
        {
            ycDelayReadHead+=ycCircularBufferLength;
        }
        
        float delay_left= ycCircularBufferLeft[(int)ycDelayReadHead];
        float delay_right=ycCircularBufferRight[(int)ycDelayReadHead];
        
        feedbackLeft = delay_left* *FeedbackParameter;
        feedbackRight = delay_right* *FeedbackParameter;
        
        buffer.setSample(0, i, buffer.getSample(0, i)* *DryWetParameter+delay_left*(1- *DryWetParameter));
        buffer.addSample(1, i, buffer.getSample(1, i)* *DryWetParameter+delay_left*(1- *DryWetParameter));
        
        ycCircularBufferWriteHead++;
        if(ycCircularBufferWriteHead>=ycCircularBufferLength)
        {
            ycCircularBufferWriteHead=0;
        }
    }
    
    
}

//==============================================================================
bool Yc_dalayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Yc_dalayAudioProcessor::createEditor()
{
    return new Yc_dalayAudioProcessorEditor (*this);
}

//==============================================================================
void Yc_dalayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Yc_dalayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Yc_dalayAudioProcessor();
}
