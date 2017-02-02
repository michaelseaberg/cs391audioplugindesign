/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
TestPlugAudioProcessorEditor::TestPlugAudioProcessorEditor (TestPlugAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);
    
    // these define the parameters of our slider object
    audioVolume.setSliderStyle (Slider::LinearBarVertical);
    audioVolume.setRange(0.0, 127.0, 1.0);
    audioVolume.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    audioVolume.setPopupDisplayEnabled (true, this);
    audioVolume.setTextValueSuffix (" Volume");
    audioVolume.setValue(1.0);
    
    // this function adds the slider to the editor
    addAndMakeVisible (&audioVolume);
    
    
    audioVolume.addListener(this);
}

TestPlugAudioProcessorEditor::~TestPlugAudioProcessorEditor()
{
}

//==============================================================================
void TestPlugAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (Colours::white);
    
    // set the current drawing colour to black
    g.setColour (Colours::black);
    
    // set the font size and draw text to the screen
    g.setFont (15.0f);
    
    g.drawFittedText ("Track Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void TestPlugAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    audioVolume.setBounds (40, 30, 20, getHeight() - 60);
}
void TestPlugAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    //processor.noteOnVel = audioVolume.getValue();
}
