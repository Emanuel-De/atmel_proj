// ---------------------------------------------------------------------------
// Praktikum, Aufgabe E2:
// Der Tastgrad des PWM-Signals an OC0A/PD6 wird über das
// Potentiometer am AD-Wandlereingang ADC0/PC0 gesteuert.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

// ---------------------------------------------------------------------------
// Fast-PWM am Anschluss OC0A/PD6 initialisieren
// ---------------------------------------------------------------------------
void init_pwm(void)
{
    DDRD = _BV(PD6);  // PWM-Ausgabe erfolgt immer an OC0A/PD6
    TCCR0A = _BV(COM0A1) + _BV(WGM00) + _BV(WGM01);  // S. 139f., Tab. 19-5/-9
    TCCR0B = _BV(CS00) + _BV(CS01);  // Datenblatt S. 142, Tab. 19-10
    OCR0A  = 128;  // Tastgrad
}

// ---------------------------------------------------------------------------
// AD-Wandler initialisieren
// ---------------------------------------------------------------------------
void init_adc(void)
{
    // Referenz für AD-Wandler = VCC = 5V (Datenblatt S. 317)
    ADMUX = _BV(REFS0);
    
    // AD-Wandler einschalten, Prescaler = 128 (Datenblatt S. 320)
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// ---------------------------------------------------------------------------
// Analogwert von Eingang ADC0 (Anschluss PC0) einlesen:
// Rückgabewert 0...1023 entspricht Spannung von 0...5 Volt
// ---------------------------------------------------------------------------
uint16_t read_adc0(void)
{
    // ADSC-Bit setzen, um Wandlung zu starten, das ADSC-Bit wird nach
    // dem Ende der Wandlung automatisch wieder gelöscht. Der eingelesene
    // 10-Bit-Wert steht anschließend im Register ADC.
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC)) { }  // Auf das Ende der Wandlung warten...
    return ADC;
}

// ---------------------------------------------------------------------------
// 8 Analogwerte von Eingang ADC0 (Anschluss PC0) einlesen und mitteln:
// Rückgabewert 0...1023 entspricht Spannung von 0...5 Volt
// ---------------------------------------------------------------------------
uint16_t read_adc0_mean(void)
{
    uint16_t sum = 0;
    uint8_t i;
    for(i = 0; i < 8; i++) sum += read_adc0();
    return sum / 8;
}

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    init_adc();
    init_pwm();

    while(1)
    {
        uint16_t analog = read_adc0_mean();

        // AD-Wert linear in PWM-Tastgrad umrechnen:
        // OCR0A = (uint8_t)255 - (uint8_t)(analog / 4);
        // OCR0A = (uint8_t)(analog / 4);

        // AD-Wert exponentiell in PWM-Tastgrad umrechnen:
        double tmp = 30.9165 * (exp(0.0021744 * analog) - 1);
        if(tmp > 255) tmp = 255;
        if(tmp <   0) tmp =   0;
        OCR0A = (uint8_t)(tmp);

        // PWM = A * exp(B * ADC) - C
        // 
        // Vorgabe:
        //           ADC =    0 --> PWM =   0
        //           ADC =  511 --> PWM =  63
        //           ADC = 1023 --> PWM = 255
        //
        // Ergebnis:
        //           A = C = 30.9165 und B = 0.0021744
    }
}

