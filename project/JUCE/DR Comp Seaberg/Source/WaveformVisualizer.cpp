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
WaveformVisualizer::WaveformVisualizer(CompressorDisplay& display, const float** dataPointer, const int numChannels, Colour color) : AudioVisualiserComponent(1) , myDisplay(display), myDataLocation(dataPointer), myNumChannels(numChannels), myWaveformColor(color)
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
        pushBuffer(myDataLocation, myNumChannels, currentProcessor->samplesInBlock);
    AudioVisualiserComponent::paint(g);
    myDisplay.repaint();
}

void WaveformVisualizer::resized()
{

}

void WaveformVisualizer::createWaveform(){
    AudioVisualiserComponent::setBounds(0, 0, 450, 150);
    AudioVisualiserComponent::setRepaintRate(30);
    AudioVisualiserComponent::setColours(Colour(180,180,180), myWaveformColor);
    myDisplay.addAndMakeVisible(this);
}

