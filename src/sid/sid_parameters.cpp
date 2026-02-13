#include "sid/sid6581.h"

#include "Config.h"

#include "parameters/Parameter.h"

#include "parameters/FrequencyParameter.h"
#include "parameters/ToggleParameter.h"

#include "mymenu.h"
#include "mymenu_items/ParameterMenuItems.h"
#include "mymenu_items/ToggleMenuItems.h"
#include "menuitems_widget.h"

#include "LinkedList.h"

#include "sid/sid_parameters.h"

#include "ParameterManager.h"

//LinkedList<FloatParameter*> available_parameters;

// Parameters, ie wrappers around destination object

//DataParameter       param_none                              = DataParameter((char*)"None");
FrequencyParameter  param_overall_pitch = FrequencyParameter<SID6581,float> (
    (char*)"Overall pitch", 
    &sid, 
    0.0, 
    &SID6581::setAllFrequency
);
DataParameter       param_overall_pulsewidth_modulation     = DataParameter<SID6581,float> ((char*)"Overall PW Mod", &sid, 0.0, &SID6581::modulateAllPulseWidths);
DataParameter       param_overall_pitch_modulation          = DataParameter<SID6581,float> ((char*)"Overall Pitch Mod", &sid, 0.0, &SID6581::modulateAllPitches);

DataParameter param_filter_cutoff = DataParameter<SID6581,uint16_t> (
    (char*)"Filter cutoff", 
    &sid, 
    &SID6581::setCutoff,
    &SID6581::getCutoff,
    0,
    MAX_CUTOFF
);
DataParameter param_filter_resonance = DataParameter<SID6581,uint8_t> (
    (char*)"Filter res", 
    &sid, 
    &SID6581::setResonance,
    &SID6581::getResonance,
    0, MAX_RESONANCE
);

ToggleParameter     param_filter_lowpass   = ToggleParameter<SID6581,bool> ((char*)"Filter LP", &sid, sid.isFilterTypeLP(), &SID6581::setFilterTypeLP);
ToggleParameter     param_filter_bandpass  = ToggleParameter<SID6581,bool> ((char*)"Filter BP", &sid, sid.isFilterTypeBP(), &SID6581::setFilterTypeBP);
ToggleParameter     param_filter_highpass  = ToggleParameter<SID6581,bool> ((char*)"Filter HP", &sid, sid.isFilterTypeHP(), &SID6581::setFilterTypeHP);
ToggleParameter     param_filter_off3      = ToggleParameter<SID6581,bool> ((char*)"Filter Off3", &sid, sid.isFilterTypeOff3(), &SID6581::setFilterTypeOff3);

void add_voice_parameters(int voice_number) {
    char label[40] = "            ";
    Voice *voice = &sid.voice[voice_number];
    ToggleParameter<Voice,bool> *param;

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Triangle");
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::triMask), &Voice::toggleTri); //triOn, &Voice::triOff);
    parameter_manager->addParameter(param);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Pulse");
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::pulseMask), &Voice::togglePulse); //&Voice::pulseOff);
    parameter_manager->addParameter(param);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Saw");    
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::sawMask), &Voice::toggleSaw); //&Voice::sawOff);
    parameter_manager->addParameter(param);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Noise");
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::noiseMask), &Voice::toggleNoise);    
    parameter_manager->addParameter(param);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Ring");
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::ringMask), &Voice::toggleRing);    
    parameter_manager->addParameter(param);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Sync");
    param = new ToggleParameter<Voice,bool>(label, voice, voice->isWaveform(ENVOSC::ringMask), &Voice::toggleRing);    
    parameter_manager->addParameter(param);

    /*sprintf(label, "Osc %i %s", voice_number+1, "Pitch");
    FrequencyParameter<Voice,float> *param_freq = new FrequencyParameter<Voice,float> (label, voice, &Voice::setFrequency);
    available_parameters->add(param_freq);*/

    /*sprintf(label, "Osc %i %s", voice_number+1, (char*)"Pitch Mod");
    DataParameter<Voice,float> *pitch_freq = new DataParameter<Voice,float> (label, voice, &Voice::modulatePitch, &Voice::getPitchMod);
    parameter_manager->addParameter(pitch_freq);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"PW Mod");
    DataParameter<Voice,float> *param3 = new DataParameter<Voice,float> (label, voice, &Voice::modulatePulseWidth, &Voice::getModulatedPulseWidth);
    param3->initialise_values(0,4095);
    parameter_manager->addParameter(param3);*/

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Freq mult");
    DataParameter<Voice,float> *param2 = new DataParameter<Voice,float> (label, voice, &Voice::setFrequencyMultiplier, &Voice::getFrequencyMultiplier);
    param2->initialise_values(0,400);
    parameter_manager->addParameter(param2);
    
    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Attack");
    DataParameter<Voice,uint8_t> *param4 = new DataParameter<Voice,uint8_t> (label, voice, &Voice::setAttack, &Voice::getAttack, 0, MAX_ATTACK);
    parameter_manager->addParameter(param4);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Decay");
    param4 = new DataParameter<Voice,uint8_t> (label, voice, &Voice::setDecay, &Voice::getDecay, 0, MAX_DECAY);
    parameter_manager->addParameter(param4);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Sustain");
    param4 = new DataParameter<Voice,uint8_t> (label, voice, &Voice::setSustain, &Voice::getSustain, 0, MAX_SUSTAIN);
    parameter_manager->addParameter(param4);

    sprintf(label, "Osc %i %s", voice_number+1, (char*)"Release");
    param4 = new DataParameter<Voice,uint8_t> (label, voice, &Voice::setRelease, &Voice::getRelease, 0, MAX_RELEASE);
    parameter_manager->addParameter(param4);

}

void setup_sid_parameters() {
    // add the available parameters to a list used globally and later passed to each selector menuitem
    Serial.println(F("==== begin setup_sid_parameters ===="));

    sid.updateAll();

    param_filter_cutoff.initialise_values(0, MAX_CUTOFF);
    param_filter_cutoff.connect_input(
        parameter_manager->getInputForName("LFO Sync"),
        1.0
    );
    param_filter_resonance.initialise_values(0, MAX_RESONANCE);

    // global sid parameters
    parameter_manager->addParameter(&param_overall_pitch);
    parameter_manager->addParameter(&param_overall_pitch_modulation);
    parameter_manager->addParameter(&param_overall_pulsewidth_modulation);
    parameter_manager->addParameter(&param_filter_cutoff);
    parameter_manager->addParameter(&param_filter_resonance);
  
    parameter_manager->addParameter(&param_filter_lowpass);
    parameter_manager->addParameter(&param_filter_bandpass);
    parameter_manager->addParameter(&param_filter_highpass);
    parameter_manager->addParameter(&param_filter_off3);

    // add the parameters for each voice
    parameter_manager->addParameter(
        new ToggleParameter<SID6581,bool>("Osc 1 Filt", &sid, sid.isFilterVoice0On(), &SID6581::changeFilterVoice0)
    );
    add_voice_parameters(0);

    parameter_manager->addParameter(
        new ToggleParameter<SID6581,bool>("Osc 2 Filt", &sid, sid.isFilterVoice1On(), &SID6581::changeFilterVoice1)
    );    
    add_voice_parameters(1);

    parameter_manager->addParameter(
        new ToggleParameter<SID6581,bool>("Osc 3 Filt", &sid, sid.isFilterVoice2On(), &SID6581::changeFilterVoice2)
    );
    add_voice_parameters(2);    

    Serial.println("==== end setup_sid_parameters ====");
}

// initialise the menus for the ParameterInputs
void setup_sid_parameter_menu() {
    Serial.println("==== setup_parameter_menu starting ====");
    //menu->add(&PulseWidthModulationPanel);
    //menu->add(&testitem);

    menu->add_page("SID parameters");

    menu->add(new MenuItem("testing..."));

    SubMenuItemColumns *test_menu = new SubMenuItemColumns("Test menu", 6);
    test_menu->add(new MenuItem("Test 1"));
    test_menu->add(new MenuItems_Widget("Widget 1", new Icon( 
        (const uint8_t[]){
            0b11111111, 
            0b10000001, 
            0b10111101, 
            0b10011001, 
            0b10011001, 
            0b10011001, 
            0b11011011, 
            0b11111111
        }, 8, 8), 32, 32));
    test_menu->add(new MenuItems_Widget("Widget 2", new Icon( 
        (const uint8_t[]){
            0b11111111, 
            0b10000001, 
            0b10111101, 
            0b10100101, 
            0b10100101, 
            0b10111101, 
            0b10000001, 
            0b11111111
        }, 8, 8), 32, 32));
    // Waveform / function icons
    test_menu->add(new MenuItems_Widget("Sawtooth", new Icon(
        (const uint8_t[]){
            0b00000001,
            0b00000011,
            0b00000111,
            0b00001111,
            0b00011111,
            0b00111111,
            0b01111111,
            0b11111111
        }, 8, 8), 32, 32));

    test_menu->add(new MenuItems_Widget("Triangle", new Icon(
        (const uint8_t[]){
            0b00010000,
            0b00111000,
            0b01111100,
            0b01111111,
            0b01111111,
            0b00111110,
            0b00111000,
            0b00010000
        }, 8, 8), 32, 32));

    test_menu->add(new MenuItems_Widget("Pulse/Square", new Icon(
        (const uint8_t[]){
            0b00110011,
            0b00110011,
            0b00110011,
            0b00110011,
            0b00110011,
            0b00110011,
            0b00110011,
            0b00110011
        }, 8, 8), 32, 32));

    test_menu->add(new MenuItems_Widget("White Noise", new Icon(
        (const uint8_t[]){
            0b01010011,
            0b01011010,
            0b00111001,
            0b10000100,
            0b11100001,
            0b10011110,
            0b00101001,
            0b01110110
        }, 8, 8), 32, 32));

    menu->add(test_menu);

    test_menu = new SubMenuItemColumns("More icons", 6);

    test_menu->add(new MenuItems_Widget("Sync", new Icon(
        (const uint8_t[]){
            0b00111000,
            0b00101110,
            0b01000011,
            0b00000011,
            0b00000011,
            0b01000011,
            0b00101110,
            0b00111000
        }, 8, 8), 32, 32));

    test_menu->add(new MenuItems_Widget("Ring", new Icon(
        (const uint8_t[]){
            0b00111000,
            0b01101110,
            0b10000011,
            0b10000001,
            0b10000001,
            0b10000011,
            0b01101110,
            0b00111000
        }, 8, 8), 32, 32));

    test_menu->add(new MenuItems_Widget("Filter", new Icon(
        (const uint8_t[]){
            0b11111111,
            0b11000001,
            0b11110001,
            0b11110001,
            0b11110001,
            0b11100001,
            0b11000001,
            0b11000001
        }, 8, 8), 32, 32));


    menu->add(test_menu);

    menu->remember_opened_page();

    /*Serial.println("Starting add available_parameters loop..."); Serial.flush();
    Serial.printf("Got %i parameters..\n", available_parameters.size());
    for (int i = 0 ; i < available_parameters.size() ; i++) {
        FloatParameter *p = available_parameters.get(i);
        Serial.printf("\tinstantiating ParameterMenuItem for item %i aka %s\n", i, p->label); Serial.flush();
        MenuItem *ctrl = p->makeControl();
        Serial.printf("\tadding it to menu...\n", i, p->label); Serial.flush();
        menu->add(ctrl);
    }
    Serial.println("Finished adding available_parameters loop"); Serial.flush();*/

    for (int i = 0 ; i < parameter_manager->available_parameters->size() ; i++) {
        FloatParameter *p = parameter_manager->available_parameters->get(i);
        if (p) {
            menu->add_page(p->label);
            menu->add(
                p->makeControls()
            );
        }
    }

    Serial.println("setup_parameter_menu done ==================");
}
