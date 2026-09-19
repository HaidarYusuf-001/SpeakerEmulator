#pragma once
#include <JuceHeader.h>
#include "SpeakerEmulatorProcessor.h"
#include "AdvancedSettingsComponent.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SpeakerEmulatorProcessor dspProcessor;
    juce::AudioDeviceSelectorComponent audioSetupComp;
    juce::ToggleButton bypassToggle;
    AdvancedSettingsComponent advancedSettings;
    juce::TooltipWindow tooltipWindow{ this, 500 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};