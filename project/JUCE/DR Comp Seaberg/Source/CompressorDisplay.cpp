/*
  ==============================================================================

    CompressorDisplay.cpp
    Created: 2 May 2017 2:22:13pm
    Author:  Michael Seaberg

  ==============================================================================
*/

#include "CompressorDisplay.h"
#include "WaveformVisualizer.h"


//==============================================================================
CompressorDisplay::CompressorDisplay(DrCompSeabergAudioProcessor& processor, DrCompSeabergAudioProcessorEditor& editor) : Component()
{
    setBounds(30, 30, 450, 150);
    myProcessor = &processor;
    myEditor = &editor;
    myResultWaveform = new WaveformVisualizer(*this,&myProcessor->currentSamples,myProcessor->getTotalNumOutputChannels(),Colour(0,0,255));
    myReducedWaveform = new WaveformVisualizer(*this,&myProcessor->reductionSamples,myProcessor->getTotalNumOutputChannels(),Colour(220,220,220));
    myReducedWaveform->setBounds(0, -30, 450, 60);
    myEditor->addAndMakeVisible(*this);

}

CompressorDisplay::~CompressorDisplay()
{
}

void CompressorDisplay::paint (Graphics& g)
{
    myEditor->repaint();
}

void CompressorDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

DrCompSeabergAudioProcessor* CompressorDisplay::getProcessor(){
    return myProcessor;
}
