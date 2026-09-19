#include "AdvancedSettingsComponent.h"

AdvancedSettingsComponent::AdvancedSettingsComponent(SpeakerEmulatorProcessor& processor)
    : dspProcessor(processor)
{
    setupSlider(crossfeedSlider, crossfeedLabel, "Crossfeed Level", 0.0, 1.0, 0.35,
        "Tingkat amplitudo sinyal yang bocor ke telinga berlawanan.");
    crossfeedSlider.onValueChange = [this] { dspProcessor.crossfeedLevel.store((float)crossfeedSlider.getValue()); };

    setupSlider(delaySlider, delayLabel, "Interaural Delay (ms)", 0.0, 2.0, 0.15,
        "Waktu tunda propagasi sinyal pada spektrum Crossfeed.");
    delaySlider.onValueChange = [this] { dspProcessor.delayTimeMs.store((float)delaySlider.getValue()); };

    setupSlider(lpfSlider, lpfLabel, "LPF Frequency (Hz)", 400.0, 10000.0, 4000.0,
        "Ambang batas frekuensi High-Pass untuk pelemahan gelombang akustik.");
    lpfSlider.onValueChange = [this] { dspProcessor.lpfFreq.store((float)lpfSlider.getValue()); };

    setupSlider(roomSizeSlider, roomSizeLabel, "Reverb Room Size", 0.0, 1.0, 0.0,
        "Dimensi spasial untuk kalkulasi Room Reflections.");
    roomSizeSlider.onValueChange = [this] { dspProcessor.revRoomSize.store((float)roomSizeSlider.getValue()); };

    setupSlider(wetLevelSlider, wetLevelLabel, "Reverb Wet Ratio", 0.0, 1.0, 0.0,
        "Rasio gabungan antara Direct Signal dan Reverberation.");
    wetLevelSlider.onValueChange = [this] { dspProcessor.revWetLevel.store((float)wetLevelSlider.getValue()); };

    setupSlider(widthSlider, widthLabel, "Reverb Stereo Width", 0.0, 1.0, 0.0,
        "Penyebaran korelasi fase pada spektrum pantulan ruangan.");
    widthSlider.onValueChange = [this] { dspProcessor.revWidth.store((float)widthSlider.getValue()); };
}

void AdvancedSettingsComponent::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name, double min, double max, double init, const juce::String& tooltip)
{
    addAndMakeVisible(slider);
    addAndMakeVisible(label);

    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    slider.setRange(min, max, 0.01);
    slider.setValue(init);
    slider.setTooltip(tooltip);

    label.setText(name, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::bottomLeft);
}

void AdvancedSettingsComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff333333));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("Spatial Acoustic Parameters", getLocalBounds().withHeight(40), juce::Justification::centred, true);
}

void AdvancedSettingsComponent::resized()
{
    auto area = getLocalBounds().reduced(15).withTrimmedTop(40);
    int itemHeight = 35;

    crossfeedSlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
    delaySlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
    lpfSlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
    roomSizeSlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
    wetLevelSlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
    widthSlider.setBounds(area.removeFromTop(itemHeight).withTrimmedTop(10));
}