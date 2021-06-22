// ---------------------------------------------------------------------------
// Praktikum, Aufgabe A1:
// (Interne) Leuchtdiode am Anschluss PB5 blinken lassen.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0b00100000;
    while (1)
    {
        PORTB = 0b00100000;
        _delay_ms(500);

        PORTB = 0b00000000;
        _delay_ms(500);
    }
}
