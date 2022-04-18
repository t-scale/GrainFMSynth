/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GrainFMSynthAudioProcessor::GrainFMSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts ( *this, nullptr, "PARAMETERS", createParameterLayout() )
#endif
{
    for (auto i = 0; i < MAX_VOICES; i++)
    {
        synth.addSound (new SynthSound());
        synth.addVoice (new SynthVoice());
    }
}

GrainFMSynthAudioProcessor::~GrainFMSynthAudioProcessor()
{

}

//==============================================================================
const juce::String GrainFMSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GrainFMSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GrainFMSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GrainFMSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GrainFMSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GrainFMSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GrainFMSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GrainFMSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GrainFMSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void GrainFMSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GrainFMSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);

    for (auto i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            voice->prepareToPlay ( sampleRate, samplesPerBlock, getTotalNumOutputChannels() );
        }
    }
}

void GrainFMSynthAudioProcessor::releaseResources()
{
    for (auto i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
            voice->releaseResources();
    }

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GrainFMSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GrainFMSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // juce::dsp::AudioBlock<float> main_audioblock { buffer };
    
    float fc_freq = apvts.getRawParameterValue ("FC_FUND")->load();
    float fc_f_spread = apvts.getRawParameterValue ("FC_F_SPREAD")->load();
    float fc_f_spread_shape = apvts.getRawParameterValue ("FC_F_SPREAD_SHAPE")->load();
    float fc_a_shape = apvts.getRawParameterValue ("FC_A_SHAPE")->load();
    // float fc_a_sweep = apvts.getRawParameterValue ("FC_A_SWEEP")->load();
    float fc_amplitude = apvts.getRawParameterValue ("FC_AMPLITUDE")->load();
    float fm_harm_ratio = apvts.getRawParameterValue ("FM_HARM_RATIO")->load();
    float fm_harm_ratio_spread = apvts.getRawParameterValue ("FM_HARM_RATIO_SPREAD")->load();
    float fm_harm_ratio_spread_shape = apvts.getRawParameterValue ("FM_HARM_RATIO_SPREAD_SHAPE")->load();
    float fm_mod_index = apvts.getRawParameterValue ("FM_MOD_INDEX")->load();
    float fm_mod_index_spread = apvts.getRawParameterValue ("FM_MOD_INDEX_SPREAD")->load();
    float amp_attack = apvts.getRawParameterValue ("AMP_ATTACK")->load();
    float amp_decay = apvts.getRawParameterValue ("AMP_DECAY")->load();
    float amp_sustain = apvts.getRawParameterValue ("AMP_SUSTAIN")->load();
    float amp_release = apvts.getRawParameterValue ("AMP_RELEASE")->load();
    // num_partials = apvts.getRawParameterValue ("NUM_PARTIALS")->load();

    for (auto i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            voice->setFrequencySpectrum ( fc_freq, fc_f_spread, fc_f_spread_shape, fc_amplitude, fc_a_shape );
            voice->setHarmonicRatios ( fm_harm_ratio, fm_harm_ratio_spread, fm_harm_ratio_spread_shape );
            voice->setModulationIndexes ( fm_mod_index, fm_mod_index_spread );
            voice->setAmplitudeADSR ( amp_attack, amp_decay, amp_sustain, amp_release );
        }
    }
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool GrainFMSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GrainFMSynthAudioProcessor::createEditor()
{
    // return new GrainFMSynthAudioProcessorEditor (*this);
        return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void GrainFMSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void GrainFMSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType() ))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout GrainFMSynthAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FC_FUND",
        "F(c) Fundamental Frequency",
        juce::NormalisableRange<float>
        (
            20.f, 2200.f, 0.01f
        ),
        220.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FC_F_SPREAD",
        "F(c) Frequency Spread",
        juce::NormalisableRange<float>
        (
            0.f, 16.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FC_F_SPREAD_SHAPE",
        "F(c) Frequency Spread Shape",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FC_A_SHAPE",
        "F(c) Amplitude Shape",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        0.f
    ));

    // layout.add (std::make_unique<juce::AudioParameterFloat>
    // (
    //     "FC_A_SWEEP",
    //     "F(c) Amplitude Sweep",
    //     juce::NormalisableRange<float>
    //     (
    //         0.f, 1.f, 0.001f
    //     ),
    //     0.f
    // ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FC_AMPLITUDE",
        "F(c) Amplitude",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        0.5f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FM_HARM_RATIO",
        "F(m) Harmonic Ratio",
        juce::NormalisableRange<float>
        (
            0.f, 16.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FM_HARM_RATIO_SPREAD",
        "F(m) Harmonic Ratio Spread",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FM_HARM_RATIO_SPREAD_SHAPE",
        "F(m) Harmonic Ratio Spread Shape",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FM_MOD_INDEX",
        "F(m) Modulation Index",
        juce::NormalisableRange<float>
        (
            0.f, 1100.f, 0.001f
        ),
        0.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "FM_MOD_INDEX_SPREAD",
        "F(m) Modulation Index Spread",
        juce::NormalisableRange<float>
        (
            0.f, 64.f, 0.001f
        ),
        0.f
    ));

    // layout.add (std::make_unique<juce::AudioParameterInt>
    // (
    //     "NUM_PARTIALS",
    //     "Number of Partials",
    //     1, 128, 16
    // ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "AMP_ATTACK",
        "Amplitude Attack",
        juce::NormalisableRange<float>
        (
            0.001f, 1.f, 0.001f
        ),
        0.05f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "AMP_DECAY",
        "Amplitude Decay",
        juce::NormalisableRange<float>
        (
            0.001f, 4.f, 0.001f
        ),
        0.4f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "AMP_SUSTAIN",
        "Amplitude Sustain",
        juce::NormalisableRange<float>
        (
            0.f, 1.f, 0.001f
        ),
        1.f
    ));

    layout.add (std::make_unique<juce::AudioParameterFloat>
    (
        "AMP_RELEASE",
        "Amplitude Release",
        juce::NormalisableRange<float>
        (
            0.001f, 4.f, 0.001f
        ),
        0.4f
    ));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrainFMSynthAudioProcessor();
}
