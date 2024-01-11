/*
  ==============================================================================

    SynthVoice.h
    Created: 14 Dec 2023 11:52:22pm
    Author:  Dele

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
// Include Maximilian library
#include "maximilian.h"

// Class represents a voice that a Synthesiser can use to play a SynthesiserSound.

class SynthVoice : public juce::SynthesiserVoice
{
public:
    // Pure virtual functions must be initialised
    // Must return true if this voice object is capable of playing the given sound.
    bool canPlaySound (juce::SynthesiserSound* sound)
    {
        // Use dynamic_cast to attempt to cast sound back to SynthSound
        // If that cast is successful, canPlaySound will return true, otherwise it will return false.
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    //============================================
    // Called to start a new note.
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
    {
        // Use trigger method to start the envelope. Set to 1. We have 0 or 1 value to see whether envelope is triggered
        env1.trigger = 1;
        // Set the variable "level" to being velocity argument. Velocity is a floating point value between 0 and 1
        // that is instatiated when key is pressed
        level = velocity;
        // Set frequency. This function will take as input the midi note pressed on keyboard and convert it to the
        // frequency the oscillator has to play
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
    }
    //============================================
    // Called to stop a note.
    void stopNote (float velocity, bool allowTailOff)
    {
        // Trigger envelope again when note is released
        env1.trigger = 0;
        // Allow the tailoff
        allowTailOff = true;
        // This will clear the note so another can be played
        if (velocity == 0)
            clearCurrentNote();
    }
    //============================================
    // Called to let the voice know that the pitch wheel has been moved.
    void pitchWheelMoved (int newPitchWheelValue)
    {
        
    }
    //============================================
    // Called to let the voice know that a midi controller has been moved.
    void controllerMoved (int controllerNumber, int newControllerValue)
    {
        
    }
    //============================================
    // Renders the next block of data for this voice.
    // This is called in PluginProcessor.cpp
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
    {
        // Set ADSR for env1. Times are in milliseconds
        env1.setAttack(2000);
        env1.setDecay(500);
        env1.setSustain(0.8);
        env1.setRelease(2000);
        
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // This is what defines the pitch of note played
            double theWave = osc1.saw(frequency);
            
            // Here we can define the pitch while also including envelope information
            double theSound = env1.adsr(theWave, env1.trigger) * level;
            
            // Create a filtered sound using filter1
            // filter1.lores(double input, double cutoff1, double resonance)
            // cutoff is in Hz
            double filteredSound = filter1.lores(theSound, 40, 0.1);
            
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                // outputBuffer.addSample(int destChannel, int destSample, float valueToAdd)
                outputBuffer.addSample(channel, startSample, filteredSound);
            }
            ++startSample;
        }
    }
    //============================================
private:
    // Create "level" to be used in startNote method
    double level;
    // Create global variable "frequency" to be used in startNote method
    double frequency;
    // Create Oscilator from maximilian libray
    maxiOsc osc1;
    // Create an envelope filter so that notes don't click
    maxiEnv env1;
    //Create filter
    maxiFilter filter1;

    
};
