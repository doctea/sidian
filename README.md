# sidian


## Breadboard pins

- D7  -> TFT_MOSI
- D5  -> TFT_CS
- D6  -> TFT_SCLK
- D4  -> TFT_MISO
- D24 -> TFT_DC

- D25 -> Select button
- D26 -> Back button
- D27 -> Encoder button
- D28 -> Encoder left?
- D29 -> Encoder right?+
 

## SID

DATA:-
- D0 -> GPIO23
- D1 -> GPIO22
- D2 -> GPIO21
- D3 -> GPIO20
- D4 -> GPIO19
- D5 -> GPIO18
- D6 -> GPIO17
- D7 -> GPIO16

- A0 -> GPIO11
- A1 -> GPIO12
- A2 -> GPIO13
- A3 -> GPIO14
- A4 -> GPIO15

- CS -> GPIO10
- RW -> GPIO9
- Phi -> GPIO3
- RES -> GPIO2  (pin 6 on sid)

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