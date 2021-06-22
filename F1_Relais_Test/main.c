// ---------------------------------------------------------------------------
// Praktikum, Aufgabe F1:
// Testprogramm für Relais am Anschluss PB2.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#define RELAIS PB2

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = _BV(RELAIS);
    while (1)
    {
        PORTB |= _BV(RELAIS);
        _delay_ms(500);

        PORTB &= ~_BV(RELAIS);
        _delay_ms(500);
    }
}
