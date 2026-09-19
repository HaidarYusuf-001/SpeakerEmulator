#include "MainComponent.h"
#include <Windows.h>

MainComponent::MainComponent()
    : audioSetupComp(deviceManager, 0, 2, 0, 2, false, false, true, false),
    advancedSettings(dspProcessor)
{
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        RegDeleteValueA(hKey, "SpeakerEmulatorApp");
        RegCloseKey(hKey);
    }

    juce::AudioDeviceManager::AudioDeviceSetup setupConfig;
    deviceManager.getAudioDeviceSetup(setupConfig);

    setupConfig.inputDeviceName = "CABLE Output (VB-Audio Virtual Cable)";
    setupConfig.outputDeviceName = "Headphones (Realtek(R) Audio)";
    setupConfig.sampleRate = 48000.0;
    setupConfig.bufferSize = 960;

    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

    deviceManager.initialise(2, 2, nullptr, true, "", &setupConfig);

    addAndMakeVisible(audioSetupComp);

    bypassToggle.setButtonText("Bypass DSP (Headphone Direct Mode)");
    addAndMakeVisible(bypassToggle);

    bypassToggle.onClick = [this] { dspProcessor.setBypass(bypassToggle.getToggleState()); };

    addAndMakeVisible(advancedSettings);

    setSize(650, 700);
    setAudioChannels(2, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlockExpected;
    spec.numChannels = 2;

    dspProcessor.prepare(spec);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::AudioBuffer<float> buffer(bufferToFill.buffer->getArrayOfWritePointers(),
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.startSample,
        bufferToFill.numSamples);

    dspProcessor.process(buffer);
}

void MainComponent::releaseResources()
{
    dspProcessor.reset();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff222222));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    bypassToggle.setBounds(area.removeFromTop(40).reduced(10, 5));
    audioSetupComp.setBounds(area.removeFromTop(320));
    advancedSettings.setBounds(area.reduced(15));
}