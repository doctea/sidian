#pragma once

// #ifdef USE_ATOMIC
//     #include "SimplyAtomic.h"
// #endif

// code to actually talk to the SID chip using the MCU's ports etc
// in theory this is where we'd replace it with shift registers et al

#include <RingBuf.h>

#include "RP2040_PWM.h"
extern RP2040_PWM* PWM_Instance;

#include <Arduino.h>

#include "sidspec.h"

#define CHECK_BIT(var,pos) ((var) & (pos))

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

extern bool debug_sid;

// implement stuff for actually reading/writing to the chip - so can "easily" rewire it
class Socket {
    public:

    // for tracking what has previously been written to this chip so we don't waste time duplicating data
    byte last_written[sizeof(sidmap)];

    struct event {
        uint8_t addr;
        uint8_t data;
    };
    RingBuf<event, 30> comBuffer;

    bool debug = false;

    // use PORTA for data bus
    #define SID6581_PIN_D0  D23
    #define SID6581_PIN_D1  D22
    #define SID6581_PIN_D2  D21
    #define SID6581_PIN_D3  D20
    #define SID6581_PIN_D4  D19
    #define SID6581_PIN_D5  D18
    #define SID6581_PIN_D6  D17
    #define SID6581_PIN_D7  D16

    // use sort of PORTD and one pin of PORTC for address bus
    // skip PD4 + PD5 so that they can be use as oscillator pinouts
    #define SID6581_PIN_A0  D11
    #define SID6581_PIN_A1  D12
    #define SID6581_PIN_A2  D13
    #define SID6581_PIN_A3  D14
    #define SID6581_PIN_A4  D15

    // SID control pins
    #define SID6581_PIN_RESET   D2
    #define SID6581_PIN_RW      D9
    #define SID6581_PIN_SEL     D10  // aka CS i believe

    #define SID6581_PIN_CLOCK   D3   // aka Phi


    void setup() {
        pinMode( SID6581_PIN_D0, OUTPUT );
        pinMode( SID6581_PIN_D1, OUTPUT );
        pinMode( SID6581_PIN_D2, OUTPUT );
        pinMode( SID6581_PIN_D3, OUTPUT );
        pinMode( SID6581_PIN_D4, OUTPUT );
        pinMode( SID6581_PIN_D5, OUTPUT );
        pinMode( SID6581_PIN_D6, OUTPUT );
        pinMode( SID6581_PIN_D7, OUTPUT );
        
        pinMode( SID6581_PIN_A0, OUTPUT );
        pinMode( SID6581_PIN_A1, OUTPUT );
        pinMode( SID6581_PIN_A2, OUTPUT );
        pinMode( SID6581_PIN_A3, OUTPUT );
        pinMode( SID6581_PIN_A4, OUTPUT );
        
        pinMode( SID6581_PIN_RESET, OUTPUT );
        pinMode( SID6581_PIN_RW,    OUTPUT );
        pinMode( SID6581_PIN_SEL,   OUTPUT );

        //pinMode( SID6581_PIN_CLOCK, OUTPUT );

        setupClock();

        resetChip();
    }


    void setupClock() {

        Serial.println("Socket#setupClock()"); Serial.flush();

        PWM_Instance = new RP2040_PWM(SID6581_PIN_CLOCK, 500000, 50);
        PWM_Instance->setPWM(SID6581_PIN_CLOCK, 1000000, 50);

        // while(1) {
        //     Serial.printf("trapped in setupClock()");
        //     Serial.flush();
        // }

        delayMicroseconds(1);
    }

    void waitCycle(void) {
        /*uint8_t foo = digitalRead( SID6581_PIN_TIMER );

        while( foo != HIGH ) {
            Serial.println("Delay1");
            foo = digitalRead( SID6581_PIN_TIMER );
        }
        
        foo = digitalRead( SID6581_PIN_TIMER );
        while( foo == HIGH ) {
            Serial.println("Delay2");    
            foo = digitalRead( SID6581_PIN_TIMER );
        }*/
        delayMicroseconds(2);
    }

    // set the SID address bus pins
    void setAddress(byte what) {
        digitalWrite( SID6581_PIN_A0, CHECK_BIT(what,1)?HIGH:LOW );
        digitalWrite( SID6581_PIN_A1, CHECK_BIT(what,2)?HIGH:LOW );
        digitalWrite( SID6581_PIN_A2, CHECK_BIT(what,4)?HIGH:LOW );
        digitalWrite( SID6581_PIN_A3, CHECK_BIT(what,8)?HIGH:LOW );
        digitalWrite( SID6581_PIN_A4, CHECK_BIT(what,16)?HIGH:LOW );
    }

    // set the SID data bus pins
    void setData(byte what) {
        digitalWrite( SID6581_PIN_D0, CHECK_BIT(what,1)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D1, CHECK_BIT(what,2)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D2, CHECK_BIT(what,4)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D3, CHECK_BIT(what,8)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D4, CHECK_BIT(what,16)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D5, CHECK_BIT(what,32)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D6, CHECK_BIT(what,64)?HIGH:LOW );
        digitalWrite( SID6581_PIN_D7, CHECK_BIT(what,128)?HIGH:LOW );
    }

    // write a register to the SID
    void write(byte addr, byte data, const char *msg = "") {
        //if (debug) 
        //if ((byte)last_written[addr]!=data) {
            //if (debug) 
            #ifdef USE_ATOMIC
            ATOMIC_BLOCK()
            #endif
            {
                Serial.printf("Writing to\t%i\t[%02x]: " BYTE_TO_BINARY_PATTERN "\t[%02X] :: %s\n", addr, addr, BYTE_TO_BINARY(data), data, msg);
            }
            /*setAddress(addr);
            setData(data);
            send();*/
            comBuffer.push({addr,data});
        // } else {
        //     // value isn't different from what was previously sent
        //     //Serial.printf("Not writing to\t%i\t[%02x]: %02x matches %02x\n", addr, last_written[addr], data);
        // }
        last_written[addr] = data;
        //delayMicroseconds(2);
    }

    void process() {
        while (!comBuffer.isEmpty()) {
            event e;
            comBuffer.pop(e);

            //Serial.printf("Popped and sending addr=%u, data=%u\n", e.addr, e.data);

            setAddress(e.addr);
            setData(e.data);
            this->send();
        }
    }

    private:

        void send() {
            waitCycle();
            
            // Ensure chip is in WRITE mode
            digitalWrite( SID6581_PIN_RW, LOW );
            
            // Select chip
            digitalWrite( SID6581_PIN_SEL, LOW );
            
            // Wait for a high to come and go
            waitCycle();
            
            // Ensure chip is in READ mode to avoid accidental writes ? // doctea
            digitalWrite( SID6581_PIN_RW, HIGH );
            
            // Deselect chip
            digitalWrite( SID6581_PIN_SEL, HIGH );
        }

        void resetChip(void) {
            int x = 10;
            
            memset(&last_written, 0, sizeof(last_written)/sizeof(byte));

            // Bring reset high
            digitalWrite( SID6581_PIN_RESET, HIGH );
            
            // HIGH for 2 cycles
            x = 2;
            while( x > 0 ) {
                waitCycle();
                x--;
            }
            
            digitalWrite( SID6581_PIN_RESET, LOW );
            
            // Bring reset LOW for 10 clock cycles
            x = 10;
            while( x > 0 ) {
                waitCycle();
                x--;
            }
            
            digitalWrite( SID6581_PIN_RESET, HIGH );
        }

};

