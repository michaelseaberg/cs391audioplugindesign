/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "CompressorDisplay.h"


//==============================================================================
DrCompSeabergAudioProcessorEditor::DrCompSeabergAudioProcessorEditor (DrCompSeabergAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{    
    setSize (width, height);
    
    myCompressorDisplay = new CompressorDisplay(p,*this,width-10,200,5,5);
    const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
    
    for (int i = 0; i < params.size(); ++i)
    {
        if (const AudioProcessorParameterWithID* parameter = dynamic_cast<AudioProcessorParameterWithID*> (params[i]))
        {
            createControl(parameter,i);
            
        }
    }
    
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
    g.drawFittedText ("Seacomp",0,0, getWidth(),30, Justification::centred, 1);
    
}

void DrCompSeabergAudioProcessorEditor::resized()
{

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
            if(i==0){
                newButton->setConnectedEdges(2);
                Label* ratioLabel;
                ratioLabel = new Label(String("ratioLabel"), String("Ratio: "));
                //ratioLabel->setText(String("Ratio Setting"), dontSendNotification);
                ratioLabel->setBounds(0, 330, 50, 30);
                ratioLabel->attachToComponent(newButton, true);
                addAndMakeVisible(ratioLabel);
            }
            else if (i==3)
                newButton->setConnectedEdges(1);
            else
                newButton->setConnectedEdges(3);
            newButton->setBounds(70+(i*112.5), 331, 112.5, 40);
            addAndMakeVisible(newButton);
        }
        
        
    }
    else{
        //float parameter-want knob
        AudioParameterFloat* floatParam = (AudioParameterFloat*) parameter;
        Slider* newSlider;
        parameterSliders.add (newSlider = new Slider (floatParam->name));
        newSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        newSlider->setRange (floatParam->range.start, floatParam->range.end,1);
        //TODO: Create new Slider Label that gives Parameter: Value Unit
        newSlider->setTextBoxStyle(Slider::NoTextBox, true, 100,0);
        

        //newSlider->setTextValueSuffix(" :"+ floatParam->name);
        newSlider->setPopupDisplayEnabled (true, this);
        newSlider->setValue (*floatParam);
        newSlider->addListener (this);
        newSlider->setBounds(30+(parameterNumber*100), 220, 100, 80);
        addAndMakeVisible (newSlider);
        
        Label* sliderLabel;
        sliderLabel = new Label(floatParam->paramID, floatParam->name);
        sliderLabel->setBounds(30+(parameterNumber*100), 301, 100, 29);
        sliderLabel->setJustificationType(Justification::centred);
        addAndMakeVisible(sliderLabel);
        
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
