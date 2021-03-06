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
                                            private Button::Listener
{
public:
    DrCompSeabergAudioProcessorEditor (DrCompSeabergAudioProcessor&);
    ~DrCompSeabergAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider* slider) override;
    void sliderDragEnded (Slider* slider) override;
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button) override;
    AudioParameterFloat* getParameterForSlider(Slider* slider);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DrCompSeabergAudioProcessor& processor;
    
    void createControl(const AudioProcessorParameterWithID* parameter, int parameterNumber);
    void createWaveform();
    
    Component* myCompressorDisplay;
    OwnedArray<Slider> parameterSliders;
    OwnedArray<Button> ratioButtons;
    const int width = 560;
    const int height = 390;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrCompSeabergAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
