/*
  ==============================================================================

    WaveformVisualizer.cpp
    Created: 2 May 2017 2:21:53pm
    Author:  Michael Seaberg

  ==============================================================================
*/

#include "WaveformVisualizer.h"
#include "CompressorDisplay.h"


//==============================================================================
WaveformVisualizer::WaveformVisualizer(CompressorDisplay& display) : AudioVisualiserComponent(1) , myDisplay(display)
{
    createWaveform();

}

WaveformVisualizer::~WaveformVisualizer()
{
}

void WaveformVisualizer::paint (Graphics& g)
{
    DrCompSeabergAudioProcessor* currentProcessor = myDisplay.getProcessor();
    if(currentProcessor->isAudioPlaying())
        pushBuffer(&currentProcessor->currentSamples, currentProcessor->getTotalNumInputChannels(), currentProcessor->samplesInBlock);
    AudioVisualiserComponent::paint(g);
    myDisplay.repaint();
}

void WaveformVisualizer::resized()
{

}

void WaveformVisualizer::createWaveform(){
    AudioVisualiserComponent::setBounds(0, 0, 450, 150);
    AudioVisualiserComponent::setRepaintRate(30);
    AudioVisualiserComponent::setColours(Colour(200,200,200), Colour(0, 0, 255));
    //addAndMakeVisible(this);
    myDisplay.addAndMakeVisible(this);
}
