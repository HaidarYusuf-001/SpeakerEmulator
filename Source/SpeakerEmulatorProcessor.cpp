#include "SpeakerEmulatorProcessor.h"

SpeakerEmulatorProcessor::SpeakerEmulatorProcessor() : delayLineL(48000), delayLineR(48000)
{}

void SpeakerEmulatorProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = (float)spec.sampleRate;

    delayBuffer.setSize(2, (int)spec.maximumBlockSize);
    delayBuffer.clear();

    juce::dsp::ProcessSpec monoSpec;
    monoSpec.sampleRate = spec.sampleRate;
    monoSpec.maximumBlockSize = spec.maximumBlockSize;
    monoSpec.numChannels = 1;

    delayLineL.prepare(monoSpec);
    delayLineR.prepare(monoSpec);

    lowPassFilter.prepare(spec);
    reverb.prepare(spec);

    lastDelayTime = -1.0f;
    lastLpfFreq = -1.0f;
}

void SpeakerEmulatorProcessor::process(juce::AudioBuffer<float>& buffer)
{
    if (isBypassed.load()) return;

    auto totalNumInputChannels = buffer.getNumChannels();
    if (totalNumInputChannels < 2) return;

    float currentDelay = delayTimeMs.load();
    if (currentDelay != lastDelayTime)
    {
        delayLineL.setDelay(currentDelay * (sampleRate / 1000.0f));
        delayLineR.setDelay(currentDelay * (sampleRate / 1000.0f));
        lastDelayTime = currentDelay;
    }

    float currentLpf = lpfFreq.load();
    if (currentLpf != lastLpfFreq)
    {
        lowPassFilter.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, currentLpf);
        lastLpfFreq = currentLpf;
    }

    float currentRoom = revRoomSize.load();
    float currentWet = revWetLevel.load();
    float currentWidth = revWidth.load();

    if (currentRoom != lastRoomSize || currentWet != lastWetLevel || currentWidth != lastWidth)
    {
        reverbParameters.roomSize = currentRoom;
        reverbParameters.damping = 0.75f;
        reverbParameters.wetLevel = currentWet;
        reverbParameters.dryLevel = 1.0f;
        reverbParameters.width = currentWidth;
        reverbParameters.freezeMode = 0.0f;
        reverb.setParameters(reverbParameters);

        lastRoomSize = currentRoom;
        lastWetLevel = currentWet;
        lastWidth = currentWidth;
    }

    auto numSamples = buffer.getNumSamples();
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    auto* delayDataL = delayBuffer.getWritePointer(0);
    auto* delayDataR = delayBuffer.getWritePointer(1);

    for (int i = 0; i < numSamples; ++i)
    {
        delayLineL.pushSample(0, channelDataL[i]);
        delayLineR.pushSample(0, channelDataR[i]);

        delayDataL[i] = delayLineL.popSample(0);
        delayDataR[i] = delayLineR.popSample(0);
    }

    juce::dsp::AudioBlock<float> block(delayBuffer.getArrayOfWritePointers(), 2, (size_t)numSamples);
    juce::dsp::ProcessContextReplacing<float> context(block);
    lowPassFilter.process(context);

    float currentCrossfeed = crossfeedLevel.load();
    float normalizationFactor = 1.0f + currentCrossfeed;

    for (int i = 0; i < numSamples; ++i)
    {
        float originalL = channelDataL[i];
        float originalR = channelDataR[i];

        float crossfeedL = delayDataR[i] * currentCrossfeed;
        float crossfeedR = delayDataL[i] * currentCrossfeed;

        channelDataL[i] = (originalL + crossfeedL) / normalizationFactor;
        channelDataR[i] = (originalR + crossfeedR) / normalizationFactor;
    }

    juce::dsp::AudioBlock<float> mainBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> mainContext(mainBlock);
    reverb.process(mainContext);

    buffer.applyGain(0.75f);
}

void SpeakerEmulatorProcessor::reset()
{
    delayLineL.reset();
    delayLineR.reset();
    lowPassFilter.reset();
    reverb.reset();
}

void SpeakerEmulatorProcessor::setBypass(bool shouldBypass)
{
    isBypassed.store(shouldBypass);
}