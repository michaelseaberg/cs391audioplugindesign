/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DrCompSeabergAudioProcessorEditor::DrCompSeabergAudioProcessorEditor (DrCompSeabergAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);
    
    myWaveform = new AudioVisualiserComponent(1);
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
    
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioParameterFloat* parameter = dynamic_cast<AudioParameterFloat*> (params[i]))
        {
            createSlider(parameter,i);
            
        }
    }
    startTimer (20);
    createWaveform();
    
}

DrCompSeabergAudioProcessorEditor::~DrCompSeabergAudioProcessorEditor()
{
}

//==============================================================================
void DrCompSeabergAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setFont (18.0f);
    g.drawFittedText ("Compressor",0,0, getWidth(),30, Justification::centred, 1);
    if(processor.isAudioPlaying())
        myWaveform->pushBuffer(&processor.currentSamples, processor.getTotalNumInputChannels(), processor.samplesInBlock);
    
}

void DrCompSeabergAudioProcessorEditor::resized()
{
    //TODO:Implement this method OR Leave alone for fixed size plugin-parameter
    //sliders is empty when this method is first called so this code does not get read on first pass
    for(int i=0;i < parameterSliders.size();i++){
        parameterSliders[i]->setBounds(30+(i*90), 200, 90, 80);
    }
}



void DrCompSeabergAudioProcessorEditor::createWaveform(){
    myWaveform->setBounds(30, 30, 540, 150);
    //myWaveform->setRepaintRate(40);
    myWaveform->setColours(Colour(200,200,200), Colour(0, 0, 255));
    addAndMakeVisible(myWaveform);
    
}

void DrCompSeabergAudioProcessorEditor::createSlider(const AudioParameterFloat* parameter, int parameterNumber){
    
    Slider* newSlider;
    parameterSliders.add (newSlider = new Slider (parameter->name));
    newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    newSlider->setRange (parameter->range.start, parameter->range.end,1);
    //TODO: Create new Slider Label that gives Parameter: Value Unit
    newSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 90,25);
    newSlider->setTextValueSuffix(" :"+ parameter->name);
    newSlider->setValue (*parameter);
    newSlider->addListener (this);
    newSlider->setBounds(30+(parameterNumber*90), 200, 90, 80);
    addAndMakeVisible (newSlider);
}

void DrCompSeabergAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        *param = (float) slider->getValue();
}

void DrCompSeabergAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->beginChangeGesture();
}

void DrCompSeabergAudioProcessorEditor::sliderDragEnded (Slider* slider)
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->endChangeGesture();
}

AudioParameterFloat* DrCompSeabergAudioProcessorEditor::getParameterForSlider (Slider* slider)
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    return dynamic_cast<AudioParameterFloat*> (params[parameterSliders.indexOf (slider)]);
}

void DrCompSeabergAudioProcessorEditor::timerCallback()
{
    if(processor.isAudioPlaying())
        repaint();
}

