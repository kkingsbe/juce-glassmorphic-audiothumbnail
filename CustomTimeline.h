/*
  ==============================================================================

    CustomTimeline.h
    Created: 29 Jan 2022 10:08:14am
    Author:  kking

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomTimeline  : public juce::Component, public juce::ChangeListener, public juce::Timer
{
public:
    CustomTimeline(juce::Colour& _backgroundColor, juce::Colour& _timelineColor, juce::Colour& _playheadColor, juce::AudioTransportSource& _transportSource, int _x, int _y, int _width, int _height);
    ~CustomTimeline() override;

    enum TimelineState
    {
        Unloaded,
        Loading,
        Loaded
    };

    void paint (juce::Graphics&) override;
    void resized() override;
    void setTime(int time);
    void setFileInputSource(juce::File file);
    void setThumbnailBounds(juce::Rectangle<int> thumbnailBounds);
    void setWidth(int _width);
    void setHeight(int _height);
    void setX(int _X);
    void setY(int _y);
    void changeState(TimelineState newState);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override //Virtual function that was required to be overridden
    {
        if (source == &thumbnail)
        {
            thumbnailChanged();
        }
    }

    void thumbnailChanged()
    {
        repaint();
    }

private:

    void timerCallback() override;
    void CustomTimeline::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void CustomTimeline::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);

    TimelineState tlState;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource& transportSource;

    juce::Colour backgroundColor;
    juce::Colour timelineColor;
    juce::Colour playheadColor;

    juce::Rectangle<int> thumbnailBounds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomTimeline)
};