/*
  ==============================================================================

    SynthVoice.cpp
    Created: 14 Apr 2022 5:09:32pm
    Author:  jef

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice() : num_partials(MAX_PARTIALS), fundamental(220.f), amp(1.f), harmonic_ratio(0.f), index(0.f) {}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{ return dynamic_cast<juce::SynthesiserSound*> (sound); }

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
  setFundamentalFrequency (juce::MidiMessage::getMidiNoteInHertz( midiNoteNumber ));
  setAmplitude ( velocity );
  ampADSR.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
  ampADSR.noteOff();
  
  if (! allowTailOff || ! ampADSR.isActive())
  {
    clearCurrentNote();
  }
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue){}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue){}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int totalNumOutputChannels)
{
  ampADSR.reset();
  ampADSR.setSampleRate (sampleRate);
  ampADSRParameters.attack = 0.1f;
  ampADSRParameters.decay = 0.7f;
  ampADSRParameters.sustain = 0.8f;
  ampADSRParameters.release = 1.5f;
  ampADSR.setParameters (ampADSRParameters);
  preparePartials( sampleRate, samplesPerBlock );

  isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
  jassert (isPrepared);
    jassert(partialsPrepared);

  if (! isVoiceActive())
    return;

  synthBuffer.setSize ( outputBuffer.getNumChannels(), numSamples, false, false, true );
  synthBuffer.clear();

  juce::dsp::AudioBlock<float> synthBlock { synthBuffer };
  processPartials ( synthBlock );
  ampADSR.applyEnvelopeToBuffer ( synthBuffer, 0, synthBuffer.getNumSamples() );

  for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
  {
    outputBuffer.addFrom ( channel, startSample, synthBuffer, channel, 0, numSamples);
    
    if (! ampADSR.isActive())
      clearCurrentNote();
  } 
}

int SynthVoice::getNumPartials() { return num_partials; }

void SynthVoice::setNumPartials ( int numPartials ) { num_partials = numPartials; }

void SynthVoice::setFundamentalFrequency ( float f ) { fundamental = f; }

void SynthVoice::setAmplitude ( float a ) { amp = a; }

void SynthVoice::setFrequencySpectrum ( float f, float f_spread, float f_shape, float a, float a_shape )
{
  for (auto partial = 0; partial < getNumPartials(); partial++)
  {
    ownedarray_onegrain.getUnchecked (partial)->frequency (
      fundamental + f + (partial * f_spread) * cos ( juce::MathConstants<float>::twoPi * f_shape * partial / getNumPartials() )
    );
    ownedarray_onegrain.getUnchecked (partial)->amplitude (
      a * amp * cos ( juce::MathConstants<float>::twoPi * a_shape * partial / getNumPartials() )
    );
  }
}

void SynthVoice::setHarmonicRatios ( float ratio, float ratio_spread, float ratio_shape ) 
{
  for (auto partial = 0; partial < getNumPartials(); partial++)
  {
    ownedarray_onegrain.getUnchecked(partial)->harmonic_ratio ( 
        ratio + ( partial * ratio_spread ) * cos (
            juce::MathConstants<float>::twoPi * ratio_shape * partial / getNumPartials()
        )
    );
  }
}

void SynthVoice::setModulationIndexes ( float index, float index_spread )
{
  for (auto partial = 0; partial < getNumPartials(); partial++)
  {
    ownedarray_onegrain.getUnchecked(partial)->mod_index ( index + (partial * index_spread) );
  }
}

void SynthVoice::processPartials ( juce::dsp::AudioBlock<float> &audioblock )
{
  for (auto partial = 0; partial < getNumPartials(); partial++)
  {
    ownedarray_onegrain.getUnchecked(partial)->process ( juce::dsp::ProcessContextReplacing<float> (audioblock) );
  }
}

void SynthVoice::preparePartials ( int sampleRate, int samplesPerBlock )
{
  for (auto i = 0; i < getNumPartials(); ++i)
  {
    ownedarray_onegrain.add (new OneGrain());
    ownedarray_onegrain.getUnchecked(i)->prepareToPlay (sampleRate, samplesPerBlock);
  }
  partialsPrepared = true;
}

void SynthVoice::setAmplitudeADSR (float a, float d, float s, float r)
{
  ampADSRParameters.attack = a;
  ampADSRParameters.decay = d;
  ampADSRParameters.sustain = s;
  ampADSRParameters.release = r;
  ampADSR.setParameters (ampADSRParameters);
}

void SynthVoice::releaseResources()
{
  for (auto partial = 0; partial < num_partials; partial++)
  {
    ownedarray_onegrain.getUnchecked(partial)->releaseResources();
  } 
}
