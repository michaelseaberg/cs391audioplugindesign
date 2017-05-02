/*
  ==============================================================================
 Michael Seaberg
 Dynamic Range Compressor Plugin
 Duke University-CS391
 Duvall,Pfister

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//Constructor + Destructor
//==============================================================================
DrCompSeabergAudioProcessor::DrCompSeabergAudioProcessor()
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
    StringArray ratioTextArray = StringArray(&ratios[0],4);
    //all parameters must be allocated and added here
    addParameter (inputGain = new AudioParameterFloat ("inG",
                                                       "Input Gain (dB)",
                                                       0,
                                                       15,
                                                       0));
    addParameter (threshold = new AudioParameterFloat ("t", // parameterID
                                                      "Threshold", // parameter name
                                                      -60,   // mininum value
                                                      0,   // maximum value
                                                      0)); // default value
    addParameter (attackTime = new AudioParameterFloat ("aT", // parameterID
                                                      "Attack Time (ms)", // parameter name
                                                      0,   // mininum value
                                                      100,   // maximum value
                                                      0)); // default value
    addParameter (releaseTime = new AudioParameterFloat ("rT", // parameterID
                                                      "Release Time (ms)", // parameter name
                                                      10,   // mininum value
                                                      1000,   // maximum value
                                                      10)); // default value
    addParameter (makeupGain = new AudioParameterFloat ("g", // parameterID
                                                        "Makeup Gain", // parameter name
                                                        0,   // mininum value
                                                        30,   // maximum value
                                                        0)); // default value
    addParameter (ratio = new AudioParameterChoice("r",
                                                   "Ratio",
                                                   ratioTextArray,
                                                   0));
}

DrCompSeabergAudioProcessor::~DrCompSeabergAudioProcessor()
{
}

//Helper Methods
//==============================================================================
float computeConstant(float time, double sampleRate){
    return exp(-1/((time*.001)*sampleRate));
}

void linTodB(float& sample){
    float temp = fabsf(sample);
    if(temp < 0.000001)
        sample = -120;
    else
        sample = 20*log10(fabsf(sample));
}

void dBToLin(float& sample){
    sample = powf(10,(sample/20));
}

//why can I not access threshold or values from inside helper methods-private objects
float computeGainCorrection(float sample, float thresholdValue, float ratioValue){
    if(sample <= thresholdValue)
        return sample;
    
    else
        return thresholdValue+((sample-thresholdValue)/ratioValue);
    
    
}

float computeLevelDetection(float sample, float previousSample, float attackConstant, float releaseConstant){
    if (sample>previousSample)
        return (attackConstant*previousSample)+((1-attackConstant)*sample);
    else
        return (releaseConstant*previousSample)+((1-releaseConstant)*sample);
    
}


//Input Gain Stage - Distortion
//==============================================================================
float DrCompSeabergAudioProcessor::gainStage(float inputSample){
    
    //process through distortion
    float outputSample;
    float threshold1 = 1.0f/3.0f;
    float threshold2 = 2.0f/3.0f;
    
        if(inputSample > threshold2)
            outputSample = 1;
        else if(inputSample > threshold1)
            outputSample = (3 - (2 - 3*inputSample) *
                   (2 - 3*inputSample))/3;
        else if(inputSample < -threshold2)
            outputSample = -1;
        else if(inputSample < -threshold1)
            outputSample = -(3 - (2 + 3*inputSample) *
                    (2 + 3*inputSample))/3;
        else
            outputSample = 2* inputSample;
    
    return outputSample;
}


//==============================================================================
void DrCompSeabergAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplesInBlock = samplesPerBlock;
    currentSample = 0;
    previousSample = 0;
    gainComputerOut = 0;
    levelDetectorOut = 0;
    controlSignal = 0;
    computedSample = 0;
    attackConstant = 0;
    releaseConstant = 0;
    gainStageFilter.setCoefficients(IIRCoefficients::makeLowPass(sampleRate*oversampling, 20000));
    gainStageFilter.reset();
    resampledBuffer.setSize (getTotalNumInputChannels(), samplesPerBlock * oversampling);
}

void DrCompSeabergAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DrCompSeabergAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DrCompSeabergAudioProcessor::processBlockBypassed (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    processorBypassed = true;
    for (int ch = getMainBusNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());
}

void DrCompSeabergAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if(buffer.hasBeenCleared())
        processorBypassed = true;
    else
        processorBypassed = false;
    
    juce::AudioPlayHead::CurrentPositionInfo tempPosition;
    getPlayHead()->getCurrentPosition(tempPosition);
    audioPlaying=tempPosition.isPlaying;
    
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    attackConstant = computeConstant(attackTime->get(),this->getSampleRate());
    releaseConstant = computeConstant(releaseTime->get(),this->getSampleRate());
    inputGainLinear = powf(10.0f, ((inputGain->get())/20.0f));

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        float* writer1 = resampledBuffer.getWritePointer (channel);
        float* channelData = buffer.getWritePointer (channel);
        
        for (int x=0; x<buffer.getNumSamples(); ++x) // note -1 otherwise you go out of bounds...
        {
            writer1[x*oversampling] = channelData[x] * inputGainLinear;
            writer1[x*oversampling+1] = 0;
            writer1[x*oversampling+2] = 0;
            writer1[x*oversampling+3] = 0;
            
        }
        gainStageFilter.processSamples(resampledBuffer.getWritePointer(channel), resampledBuffer.getNumSamples());
        
        for (int i=0; i<resampledBuffer.getNumSamples(); i++){
            writer1[i] = gainStage(writer1[i]);
        }
        
        
        for(int x=0; x<buffer.getNumSamples(); ++x){
            channelData[x] = writer1[x*oversampling];
        }
        
        
        
        //gainStageFilter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        currentSamples = buffer.getReadPointer(channel);
        for(int sample=0; sample < buffer.getNumSamples(); ++sample){
            currentSample = channelData[sample];
            
            //take channelData and treat as "data"-one sample. Increment the pointer when time to process next sample
            controlSignal = currentSample;
            //linear to DB conversion
            linTodB(controlSignal);
            //gain computing
            gainComputerOut = computeGainCorrection(controlSignal,threshold->get(),(ratio->getCurrentChoiceName()).getFloatValue());
            controlSignal = controlSignal-gainComputerOut;
            //dBToLin(controlSignal);
            
            //level detection
            levelDetectorOut = computeLevelDetection(controlSignal,previousSample,attackConstant,releaseConstant);
            previousSample = levelDetectorOut;
            controlSignal = (makeupGain->get())-levelDetectorOut;
            //db to linear conversion
            dBToLin(controlSignal);
            //set sample to equal new sample
            computedSample = currentSample*controlSignal;
            channelData[sample] = computedSample;
            
        }
        //detect how many channels there are, for each channel of data,
        //we must process the current buffer individually
        //currentSamples = *new AudioSampleBuffer(&channelData,totalNumInputChannels,buffer.getNumSamples());
    }
}

bool DrCompSeabergAudioProcessor::isAudioPlaying(){
    return audioPlaying;
}

bool DrCompSeabergAudioProcessor::isProcessorBypassed(){
    return processorBypassed;
}

//Editor interface
//==============================================================================
bool DrCompSeabergAudioProcessor::hasEditor() const
{
    return true; //Returns true to tell host that we want to use our editor
}

AudioProcessorEditor* DrCompSeabergAudioProcessor::createEditor()
{
    //uncomment below to use PluginEditor class
    return new DrCompSeabergAudioProcessorEditor(*this);

    
    //currently using self set up editor interface
    //return new GenericAudioProcessorEditor (this);
}

//State storage
//==============================================================================
void DrCompSeabergAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DrCompSeabergAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrCompSeabergAudioProcessor();
}
