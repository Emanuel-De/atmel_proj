// ---------------------------------------------------------------------------
// Praktikum, Aufgabe A3:
// Leuchtdioden an PB5/PD6 einschalten, wenn Taster an PD2 gerückt ist.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB  = 0b00100000;
    DDRD  = 0b01000000;
    PORTD = 0b00000100;  // Pullup-Widerstand an PD2 einschalten!

    while (1)
    {
        if((PIND & 0b00000100) == 0)
        {
            // Taster gedrückt --> Eingang PD2 ist null
            PORTB = 0b00100000;
            PORTD = 0b01000100;  // Aufpassen: Pullup an PD2 muss bleiben!
        }
        else
        {
            PORTB = 0b00000000;
            PORTD = 0b00000100;  // Aufpassen: Pullup an PD2 muss bleiben!
        }
    }
}
