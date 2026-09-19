#pragma once
#include <JuceHeader.h>
#include "SpeakerEmulatorProcessor.h"

class AdvancedSettingsComponent : public juce::Component
{
public:
    AdvancedSettingsComponent(SpeakerEmulatorProcessor& processor);
    ~AdvancedSettingsComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SpeakerEmulatorProcessor& dspProcessor;

    juce::Slider crossfeedSlider;
    juce::Label crossfeedLabel;

    juce::Slider delaySlider;
    juce::Label delayLabel;

    juce::Slider lpfSlider;
    juce::Label lpfLabel;

    juce::Slider roomSizeSlider;
    juce::Label roomSizeLabel;

    juce::Slider wetLevelSlider;
    juce::Label wetLevelLabel;

    juce::Slider widthSlider;
    juce::Label widthLabel;

    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name, double min, double max, double init, const juce::String& tooltip);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedSettingsComponent)
};