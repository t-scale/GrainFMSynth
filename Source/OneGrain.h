/*
  ==============================================================================

    OneGrain.h
    Created: 9 Mar 2022 5:12:44pm
    Author:  jef

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FmVoice.h"

class MapUI;
class dsp;

class OneGrain
{
  public:

  /* utils */
  void prepareToPlay (float sampleRate, float samplesPerBlock);
  void releaseResources();
  void process (juce::dsp::ProcessContextReplacing<float> context);

  /* params */
  void frequency (float f);
  void amplitude( float a);
  void harmonic_ratio (float r);
  void mod_index (float i);
  void adsr (float a, float d, float s, float r);
  void gate (int g);

  private:
  MapUI* fUI;
  dsp* fDSP;
  float** outputs;
};