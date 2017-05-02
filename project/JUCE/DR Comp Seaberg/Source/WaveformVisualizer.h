/*
  ==============================================================================

    WaveformVisualizer.h
    Created: 2 May 2017 2:21:53pm
    Author:  Michael Seaberg

  ==============================================================================
*/

#ifndef WAVEFORMVISUALIZER_H_INCLUDED
#define WAVEFORMVISUALIZER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompressorDisplay.h"

//==============================================================================
/*
*/
class WaveformVisualizer    : public AudioVisualiserComponent
{
public:
    WaveformVisualizer(CompressorDisplay&);
    ~WaveformVisualizer();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformVisualizer)
    CompressorDisplay& myDisplay;
    
    void createWaveform();
};


#endif  // WAVEFORMVISUALIZER_H_INCLUDED
