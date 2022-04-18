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

#define MAX_PARTIALS 32

class SynthVoice : public juce::SynthesiserVoice
{
  public:
    SynthVoice();
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int totalNumOutputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    int getNumPartials();
    void setNumPartials ( int numPartials );
    float getFundamentalFrequency ();
    void setFundamentalFrequency ( float f );
    void setFrequencySpectrum ( float f, float f_spread, float f_shape, float a, float a_shape );
    float getAmplitude();
    void setAmplitude ( float a );
    void setHarmonicRatios ( float ratio, float ratio_spread, float ratio_shape );
    void setModulationIndexes ( float index, float index_spread );
    void preparePartials ( int sampleRate, int samplesPerBlock );
    void processPartials ( juce::dsp::AudioBlock<float> &audioblock );
    void setAmplitudeADSR (float a, float d, float s, float r);
    void releaseResources();

  private:
    juce::AudioBuffer<float> synthBuffer;

    juce::ADSR ampADSR;
    // juce::ADSR modADSR;
    juce::ADSR::Parameters ampADSRParameters;
    // juce::ADSR::Parameters modADSRParameters;

    int num_partials = 16;
    float fundamental = 220.f;
    float amp = 1.f;
    float harmonic_ratio = 0.f;
    float index = 0.f;

    juce::OwnedArray<OneGrain> ownedarray_onegrain;

    bool isPrepared = false;
    bool partialsPrepared = false;
};
