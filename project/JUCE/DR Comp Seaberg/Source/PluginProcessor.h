/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>



//==============================================================================
/**
*/
class DrCompSeabergAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DrCompSeabergAudioProcessor();
    ~DrCompSeabergAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    void processBlockBypassed (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override               { return JucePlugin_Name; }
    bool acceptsMidi() const override                   { return false; }
    bool producesMidi() const override                  { return false; }
    double getTailLengthSeconds() const override        { return 0; }
    
    //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return String(); }
    void changeProgramName (int , const String& ) override { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    const float* currentSamples;
    const float* reductionSamples;
    bool isAudioPlaying();
    bool isProcessorBypassed();
    int samplesInBlock; //refactor
    

private:
    float gainStage(float inputSample);
    //==============================================================================
    //declare all parameters here
    AudioParameterFloat* makeupGain;
    AudioParameterFloat* threshold;
    AudioParameterFloat* attackTime;
    AudioParameterFloat* releaseTime;
    AudioParameterChoice* ratio;
    AudioParameterFloat* inputGain;
    float currentSample;
    float previousSample;
    float gainComputerOut;
    float levelDetectorOut;
    float controlSignal;
    float computedSample;
    float attackConstant;
    float releaseConstant;
    float inputGainLinear;
    const String ratios[4] = {String("2"),String("4"),String("6"),String("8")};
    
    bool processorBypassed;
    bool audioPlaying;
    
    
    const int oversampling = 4;
    AudioSampleBuffer resampledBuffer;
    AudioSampleBuffer reductionBuffer;
    IIRFilter gainStageFilter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrCompSeabergAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
