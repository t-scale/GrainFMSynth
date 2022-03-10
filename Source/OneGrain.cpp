/*
  ==============================================================================

    OneGrain.cpp
    Created: 9 Mar 2022 5:12:44pm
    Author:  jef

  ==============================================================================
*/

#include "OneGrain.h"

void OneGrain::prepareToPlay (float sampleRate, float samplesPerBlock)
{
  fDSP = new mydsp();
  fDSP->init(sampleRate);
  fUI = new MapUI();
  fDSP->buildUserInterface(fUI);
  outputs = new float*[2];

  for (auto channel = 0; channel < 2; ++channel) 
      outputs[channel] = new float [samplesPerBlock];

  fUI->setParamValue("gate",1);
}

void OneGrain::releaseResources()
{
  delete fDSP;
  delete fUI;
  for (int channel = 0; channel < 2; ++channel)
      delete[] outputs[channel];
}
void OneGrain::process (juce::dsp::ProcessContextReplacing<float> context)
{ 
  if (context.isBypassed) context.getOutputBlock().clear();

  auto&& input_block  = context.getInputBlock();
  auto&& output_block = context.getOutputBlock();

  auto num_samples = output_block.getNumSamples();
  auto num_channels = output_block.getNumChannels();
  auto input_channels = input_block.getNumChannels();

  fDSP->compute (num_samples, NULL, outputs);

  for (auto channel = 0; channel < num_channels; channel++) {
    for (auto sample = 0; sample < num_samples; sample++) {
      output_block.addSample (channel, sample, outputs[0][sample]);
    }
  }
}

void OneGrain::frequency (float f) { fUI->setParamValue ("/addfm/FM/freq", f); }
void OneGrain::amplitude (float a) { fUI->setParamValue ("/addfm/envelope/gain", a); }
void OneGrain::harmonic_ratio (float r) { fUI->setParamValue ("/addfm/FM/harmonic_ratio", r); }
void OneGrain::mod_index (float i) { fUI->setParamValue ("/addfm/FM/mod_index", i); }
void OneGrain::gate (int g) { fUI->setParamValue ("/addfm/envelope/gate", g); }
void OneGrain::adsr (float a, float d, float s, float r)
{
  fUI->setParamValue ("/addfm/envelope/attack", a);
  fUI->setParamValue ("/addfm/envelope/decay", d);
  fUI->setParamValue ("/addfm/envelope/sustain", s);
  fUI->setParamValue ("/addfm/envelope/release", r);
}