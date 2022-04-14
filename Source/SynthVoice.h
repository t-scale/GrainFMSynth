/*
  ==============================================================================

    SynthVoice.h
    Created: 14 Apr 2022 5:09:32pm
    Author:  jef

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "OneGrain.h"

class SynthVoice : public juce::SynthesiserVoice
{
  public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int totalNumOutputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;

  private:
    juce::AudioBuffer<float> synthBuffer;

    juce::ADSR ampADSR;
    // juce::ADSR modADSR;
    juce::ADSR::Parameters ampADSRParameters;
    // juce::ADSR::Parameters modADSRParameters;

    juce::OwnedArray<OneGrain> ownedarray_onegrain;
    int num_partials;

    bool isPrepared = false;
};