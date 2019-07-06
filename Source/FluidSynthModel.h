//
// Created by Alex Birch on 10/09/2017.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SharesParams.h"
#include <fluidsynth.h>
#include <memory>
#include "BankAndPreset.h"
#include "PresetsToBanks.h"


// https://stackoverflow.com/a/13446565/5257399
//using std::shared_ptr;

using namespace std;

class FluidSynthModel {
public:
    FluidSynthModel(
        AudioProcessorValueTreeState& valueTreeState,
        SharesParams& sharedParams
        );
    // ~FluidSynthModel();

    shared_ptr<fluid_synth_t> getSynth();
    void initialise();

    BanksToPresets getBanks();

    void changePreset(int bank, int preset);
    int getChannel();

    void onFileNameChanged(const String &absPath, int bank, int preset);
    void setControllerValue(int controller, int value);

    //==============================================================================
    /**
        Used to receive callbacks when a button is clicked.

        @see Button::addListener, Button::removeListener
    */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener();

        /** Called when the button is clicked. */
        virtual void fontChanged (FluidSynthModel*, const String &absPath);
    };

    /** Registers a listener to receive events when this button's state changes.
        If the listener is already registered, this will not register it again.
        @see removeListener
    */
    void addListener (Listener* newListener);

    /** Removes a previously-registered button listener
        @see addListener
    */
    void removeListener (Listener* listener);

    void setSampleRate(float sampleRate);

    const String& getCurrentSoundFontAbsPath();

private:
    int handleMidiEvent(void* data, fluid_midi_event_t* event);

    AudioProcessorValueTreeState& valueTreeState;
    SharesParams& sharedParams;

    shared_ptr<fluid_synth_t> synth;
    unique_ptr<fluid_settings_t, decltype(&delete_fluid_settings)> settings;
    // unique_ptr<fluid_midi_driver_t, decltype(&delete_fluid_midi_driver)> midiDriver;

    String currentSoundFontAbsPath;

    float currentSampleRate;

    fluid_preset_t* getFirstPreset();
    void selectFirstPreset();
    unique_ptr<BankAndPreset> getFirstBankAndPreset();

    void unloadAndLoadFont(const String &absPath);
    void loadFont(const String &absPath);
    bool shouldLoadFont(const String &absPath);

    void changePresetImpl(int bank, int preset);

    bool initialised;
    unsigned int sfont_id;
    unsigned int channel;

//    fluid_mod_t* mod;

    ListenerList<Listener> eventListeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FluidSynthModel)
};
