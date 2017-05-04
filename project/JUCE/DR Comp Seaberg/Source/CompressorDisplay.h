/*
  ==============================================================================

    CompressorDisplay.h
    Created: 2 May 2017 2:22:13pm
    Author:  Michael Seaberg

  ==============================================================================
*/

#ifndef COMPRESSORDISPLAY_H_INCLUDED
#define COMPRESSORDISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
/*
*/
class CompressorDisplay    : public Component
{
public:
    CompressorDisplay(DrCompSeabergAudioProcessor&, DrCompSeabergAudioProcessorEditor&, int, int ,int,int);
    ~CompressorDisplay();

    void paint (Graphics&) override;
    void resized() override;
    DrCompSeabergAudioProcessor* getProcessor();

private:
    DrCompSeabergAudioProcessor* myProcessor;
    DrCompSeabergAudioProcessorEditor* myEditor;
    AudioVisualiserComponent* myResultWaveform;
    AudioVisualiserComponent* myReducedWaveform;
    int width;
    int height;
    int xpos;
    int ypos;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorDisplay)
};


#endif  // COMPRESSORDISPLAY_H_INCLUDED
