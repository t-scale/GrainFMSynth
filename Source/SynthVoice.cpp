/*
  ==============================================================================

    SynthVoice.cpp
    Created: 14 Apr 2022 5:09:32pm
    Author:  jef

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{ return dynamic_cast<juce::SynthesiserSound*> (sound); }

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
  for (auto i = 0; i < num_partials; ++i) 
  {
    ownedarray_onegrain.getUnchecked(i)->frequency (juce::MidiMessage::getMidiNoteInHertz( midiNoteNumber ));
    ownedarray_onegrain.getUnchecked(i)->amplitude ((float)velocity / 127.f);
  }
  ampADSR.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
  ampADSR.noteOff();
  if (! allowTailOff || ! ampADSR.isActive())
    clearCurrentNote();
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int totalNumOutputChannels)
{
  ampADSR.reset();

  ampADSR.setSampleRate (sampleRate);
  ampADSRParameters.attack = 0.1f;
  ampADSRParameters.decay = 0.7f;
  ampADSRParameters.sustain = 0.8f;
  ampADSRParameters.release = 1.5f;
  ampADSR.setParameters (ampADSRParameters);

  for (auto i = 0; i < num_partials; ++i) 
  {
    ownedarray_onegrain.add (new OneGrain());
    ownedarray_onegrain.getUnchecked(i)->prepareToPlay (sampleRate, samplesPerBlock);
  }

  isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
  jassert (isPrepared);

  if (! isVoiceActive())
    return;

  synthBuffer.setSize ( outputBuffer.getNumChannels(), numSamples, false, false, true );
  synthBuffer.clear();

  juce::dsp::AudioBlock<float> synthBlock { synthBuffer };

  for (auto i = 0; i < num_partials; i++) 
  {
    ownedarray_onegrain.getUnchecked(i)->process ( juce::dsp::ProcessContextReplacing<float> ( synthBlock ) );
  }
  ampADSR.applyEnvelopeToBuffer ( synthBuffer, 0, synthBuffer.getNumSamples() );

  for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
  {
  outputBuffer.addFrom ( channel, startSample, synthBuffer, channel, 0, numSamples);
  
  if (! ampADSR.isActive()) 
    clearCurrentNote();
  } 
}