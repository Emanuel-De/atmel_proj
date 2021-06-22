// ---------------------------------------------------------------------------
// Praktikum, Aufgabe E1:
// PWM-Testprogramm --> Ausgabe des PWM-Signals am Anschluss OC0A/PD6.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void init_pwm(void)
{
    DDRD = _BV(PD6);  // PWM-Ausgabe erfolgt immer an PD6/OC0A
    TCCR0A = _BV(COM0A1) + _BV(WGM00) + _BV(WGM01);  // S. 139f., Tab. 19-5/-9
    TCCR0B = _BV(CS00) + _BV(CS01);  // Datenblatt S. 142, Tab. 19-10
    OCR0A  = 128;  // Tastgrad
}

int main(void)
{
    init_pwm();
    while(1)
    {
        OCR0A =   0; _delay_ms(1000);    // Frequenz, Tastgrad = ...?!
        OCR0A =  64; _delay_ms(1000);    OCR0A = 128; _delay_ms(1000);
        OCR0A = 192; _delay_ms(1000);    OCR0A = 255; _delay_ms(1000);
    }
}
