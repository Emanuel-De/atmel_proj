// ---------------------------------------------------------------------------
// Praktikum, Aufgabe B2:
// Leuchtdioden an PB5/PD6 werden durch Timer gesteuert.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void led_on(void)
{
    PORTB = 0b00100000;
    PORTD = 0b01000100;  // Aufpassen: Pullup an PD2 muss bleiben!
}

void led_off(void)
{
    PORTB = 0b00000000;
    PORTD = 0b00000100;  // Aufpassen: Pullup an PD2 muss bleiben!
}

void init_timer(void)
{
    TCCR0A = 0b00000010; // WGM01 gesetzt (siehe Tab. 19-9, S. 140)
    TCCR0B = 0b00000101; // CS00+CS02 gesetzt (Tab. 19-10, S. 142)
    TIMSK0 = 0b00000010; // OCIE0A gesetzt (siehe S. 143)
    OCR0A  = 255;        // Vergleichswert für Timer
    sei();               // Interrupt-System einschalten
}

int led = 0; // Aktueller LED-Zustand
ISR(TIMER0_COMPA_vect)
{
    switch(led)
    {
        case 0: led_on();  led = 1; break;
        case 1: led_off(); led = 0; break;
    }
}

int main(void)
{
    DDRB  = 0b00100000;  // LED an PB5
    DDRD  = 0b01000000;  // LED an PD6, Taster an PD2
    PORTD = 0b00000100;  // Pullup an PD2 einschalten!

    init_timer();
    while(1) { }  // Endlosschleife
}
