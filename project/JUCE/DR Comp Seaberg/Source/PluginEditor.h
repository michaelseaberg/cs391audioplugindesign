/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class DrCompSeabergAudioProcessorEditor  : public AudioProcessorEditor,
                                            private Slider::Listener,
                                            private Timer  
{
public:
    DrCompSeabergAudioProcessorEditor (DrCompSeabergAudioProcessor&);
    ~DrCompSeabergAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    AudioParameterFloat* getParameterForSlider(Slider* slider);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DrCompSeabergAudioProcessor& processor;
    
    void createSlider(const AudioParameterFloat* parameter, int parameterNumber);
    void createWaveform();
    
    AudioVisualiserComponent* myWaveform;
    OwnedArray<Slider> parameterSliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrCompSeabergAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
