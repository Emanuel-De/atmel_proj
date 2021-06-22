// ---------------------------------------------------------------------------
// Praktikum, Aufgabe A2:
// (Interne) LED an PB5 und externe LED an PD6 abwechselnd blinken lassen.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0b00100000;
    DDRD = 0b01000000;

    while (1)
    {
        PORTB = 0b00100000;
        PORTD = 0b01000000;
        _delay_ms(500);

        PORTB = 0b00000000;
        PORTD = 0b00000000;
        _delay_ms(500);
    }
}
