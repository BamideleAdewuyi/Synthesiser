/*
  ==============================================================================

    SynthSound.h
    Created: 14 Dec 2023 11:52:09pm
    Author:  Dele

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Class for storing sounds for the synthesiser

class SynthSound : public juce::SynthesiserSound
{
public:
    // Pure virtual functions must be initialised
    // If a note is pressed, this should return True
    bool appliesToNote (int /*midiNoteNumber*/)
    {
        return true;
    }
    
    bool appliesToChannel (int /*midiChannel*/)
    {
        return true;
    }
    
    
};
