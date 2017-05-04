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
CompressorDisplay::CompressorDisplay(DrCompSeabergAudioProcessor& processor, DrCompSeabergAudioProcessorEditor& editor, int w, int h, int x, int y) : Component(), width(w), height(h), xpos(x), ypos(y)
{
    setBounds(xpos, ypos, width, height);
    myProcessor = &processor;
    myEditor = &editor;
    myResultWaveform = new WaveformVisualizer(*this,&myProcessor->currentSamples,myProcessor->getTotalNumOutputChannels(),Colour(0,0,255),Colour(180,180,255),width,height);
    myReducedWaveform = new WaveformVisualizer(*this,&myProcessor->reductionSamples,myProcessor->getTotalNumOutputChannels(),Colour(180,180,255),Colour(255,255,255),width,height);
    myReducedWaveform->setBounds(0, -50, width, 100);
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

}

DrCompSeabergAudioProcessor* CompressorDisplay::getProcessor(){
    return myProcessor;
}
