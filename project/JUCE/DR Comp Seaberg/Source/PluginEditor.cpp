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
    setSize (510, 340);
    
    myWaveform = new AudioVisualiserComponent(1);
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
    
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioProcessorParameterWithID* parameter = dynamic_cast<AudioProcessorParameterWithID*> (params[i]))
        {
            createControl(parameter,i);
            
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
//    for(int i=0;i < parameterSliders.size();i++){
//        parameterSliders[i]->setBounds(30+(i*90), 200, 90, 80);
//    }
}



void DrCompSeabergAudioProcessorEditor::createWaveform(){
    myWaveform->setBounds(30, 30, 450, 150);
    //myWaveform->setRepaintRate(40);
    myWaveform->setColours(Colour(200,200,200), Colour(0, 0, 255));
    addAndMakeVisible(myWaveform);
    
}

void DrCompSeabergAudioProcessorEditor::createControl(const AudioProcessorParameterWithID* parameter, int parameterNumber){
    
    if(parameter->paramID == "r"){
        //choice parameter-want buttons
        AudioParameterChoice* choiceParam = (AudioParameterChoice*) parameter;
        for(int i=0; i< choiceParam->choices.size(); i++){
            Button* newButton;
            ratioButtons.add(newButton = new TextButton (choiceParam->choices[i]));
            //newButton->setButtonText(choiceParam->name);
            newButton->setClickingTogglesState(true);
            newButton->setRadioGroupId(1);
            newButton->addListener(this);
            newButton->setBounds(30+(i*112.5), 281, 112.5, 40);
            addAndMakeVisible(newButton);
        }
        
        
        //newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        //newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
        //TODO: Create new Slider Label that gives Parameter: Value Unit
        //newSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 90,25);
        //newSlider->setTextValueSuffix(" :"+ floatParam->name);
        
    }
    else{
        //float parameter-want knob
        AudioParameterFloat* floatParam = (AudioParameterFloat*) parameter;
        Slider* newSlider;
        parameterSliders.add (newSlider = new Slider (floatParam->name));
        newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
        //TODO: Create new Slider Label that gives Parameter: Value Unit
        newSlider->setTextBoxStyle(Slider::TextBoxBelow, true, 90,25);
        newSlider->setTextValueSuffix(" :"+ floatParam->name);
        newSlider->setValue (*floatParam);
        newSlider->addListener (this);
        newSlider->setBounds(30+(parameterNumber*90), 200, 90, 80);
        addAndMakeVisible (newSlider);
    }
    
}
void DrCompSeabergAudioProcessorEditor::buttonClicked (Button* button){
    //button->setState(juce::ArrowButton::buttonDown);
    
    
}
void DrCompSeabergAudioProcessorEditor::buttonStateChanged (Button* button){
    //if button is clicked and was not previously selected then pass new ratio value-ratio parameter is added 6th, array index 5
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    
    if(AudioParameterChoice* param = dynamic_cast<AudioParameterChoice*> (params[5]))
        if(button->getState() == juce::TextButton::buttonDown)
            *param = ratioButtons.indexOf(button);
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
    if(processor.isAudioPlaying() && !processor.isProcessorBypassed())
        repaint();
}

