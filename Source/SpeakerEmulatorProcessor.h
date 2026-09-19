#pragma once
#include <JuceHeader.h>
#include <atomic>

class SpeakerEmulatorProcessor
{
public:
    SpeakerEmulatorProcessor();
    ~SpeakerEmulatorProcessor() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();
    void setBypass(bool shouldBypass);

    std::atomic<float> crossfeedLevel{ 0.35f };
    std::atomic<float> delayTimeMs{ 0.15f };
    std::atomic<float> lpfFreq{ 4000.0f };
    std::atomic<float> revRoomSize{ 0.0f };
    std::atomic<float> revWetLevel{ 0.0f };
    std::atomic<float> revWidth{ 0.0f };

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLineL;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLineR;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParameters;
    juce::AudioBuffer<float> delayBuffer;

    float sampleRate = 48000.0f;
    std::atomic<bool> isBypassed{ false };

    float lastDelayTime = -1.0f;
    float lastLpfFreq = -1.0f;
    float lastRoomSize = -1.0f;
    float lastWetLevel = -1.0f;
    float lastWidth = -1.0f;
};