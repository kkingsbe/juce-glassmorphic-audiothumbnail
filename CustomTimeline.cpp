/*
  ==============================================================================

    CustomTimeline.cpp
    Created: 29 Jan 2022 10:08:14am
    Author:  kking

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomTimeline.h"

//==============================================================================
CustomTimeline::CustomTimeline(juce::Colour& _backgroundColor, 
                               juce::Colour& _timelineColor,
                               juce::Colour& _playheadColor,
                               juce::AudioTransportSource& _transportSource, 
                               int _x, 
                               int _y, 
                               int _width, 
                               int _height)
                               : 
                               backgroundColor(_backgroundColor),
                               timelineColor(_timelineColor),
                               playheadColor(_playheadColor),
                               thumbnailCache(5), 
                               thumbnail(512,formatManager,thumbnailCache), 
                               transportSource(_transportSource)
{
    thumbnail.addChangeListener(this);
    formatManager.registerBasicFormats();
    thumbnailBounds.setPosition(_x, _y);
    thumbnailBounds.setSize(_width, _height);
    changeState(Unloaded);
}

CustomTimeline::~CustomTimeline()
{

}

void CustomTimeline::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    if (thumbnail.getNumChannels() == 0)
    {
        paintIfNoFileLoaded(g, thumbnailBounds);
    }
    else
    {
        paintIfFileLoaded(g, thumbnailBounds);
    }
}

//Display "No File Loaded"
void CustomTimeline::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(backgroundColor);
    g.setOpacity(0.25);
    g.fillRoundedRectangle(thumbnailBounds.getX(), thumbnailBounds.getY(), thumbnailBounds.getWidth(), thumbnailBounds.getHeight(), 20);
    g.setOpacity(1.0);
    g.setColour(timelineColor);
    g.setFont(25.0f);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

//Display waveform
void CustomTimeline::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(backgroundColor); //Waveform display background
    g.setOpacity(0.25);
    g.fillRoundedRectangle(thumbnailBounds.getX(), thumbnailBounds.getY(), thumbnailBounds.getWidth(), thumbnailBounds.getHeight(), 20);
    g.setColour(timelineColor); //Waveform color
    g.setOpacity(1.0);

    //Modify to not run this each loop
    thumbnail.drawChannels(g,
        thumbnailBounds,
        0.0,                         //Start Time
        thumbnail.getTotalLength(),  //End Time
        1.0f);                       //Vertical Zoom

    g.setColour(playheadColor);
    auto audioPosition = (float)transportSource.getCurrentPosition();
    auto drawPosition = (audioPosition / transportSource.getLengthInSeconds()) * (float)thumbnailBounds.getWidth() + (float)thumbnailBounds.getX();
    g.drawLine(drawPosition, (float)thumbnailBounds.getY(), drawPosition, (float)thumbnailBounds.getBottom(), 2.0f);
}

void CustomTimeline::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
}

void CustomTimeline::setFileInputSource(juce::File file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void CustomTimeline::setWidth(int _width)
{
    thumbnailBounds.setWidth(_width);
}

void CustomTimeline::setHeight(int _height)
{
    thumbnailBounds.setHeight(_height);
}

void CustomTimeline::setX(int _X)
{
    thumbnailBounds.setX(_X);
}

void CustomTimeline::setY(int _y)
{
    thumbnailBounds.setY(_y);
}

void CustomTimeline::changeState(TimelineState newState)
{
    if (tlState != newState)
    {
        tlState = newState;
        switch (newState)
        {
        case Unloaded:
            stopTimer();
            break;
        case Loaded:
            startTimerHz(60);
            break;
        }
    }
}

void CustomTimeline::timerCallback()
{
    repaint();
}