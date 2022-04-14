/*
  ==============================================================================

    SynthSound.h
    Created: 14 Apr 2022 5:09:41pm
    Author:  jef

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct SynthSound : public juce::SynthesiserSound
{
  bool appliesToNote (int midiNoteNumber) { return true; }
  bool appliesToChannel (int midiChannel) { return true; }
};