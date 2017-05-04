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
WaveformVisualizer::WaveformVisualizer(CompressorDisplay& display, const float** dataPointer, const int numChannels, Colour startColor, Colour endColor , int w, int h) : AudioVisualiserComponent(1) , myDisplay(display), myDataLocation(dataPointer), myNumChannels(numChannels), myStartColor(startColor), myEndColor(endColor), width(w), height(h)
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
    g.setGradientFill(ColourGradient(myStartColor, width, height/2, myEndColor, width-100, height/2, false));
    AudioVisualiserComponent::paint(g);
    myDisplay.repaint();
}

void WaveformVisualizer::resized()
{

}

void WaveformVisualizer::createWaveform(){
    AudioVisualiserComponent::setBounds(0, 0, width, height);
    AudioVisualiserComponent::setRepaintRate(refreshRate);
    AudioVisualiserComponent::setColours(Colour(130,130,130), Colour());
    myDisplay.addAndMakeVisible(this);
}

