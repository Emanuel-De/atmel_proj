// ---------------------------------------------------------------------------
// Praktikum, Aufgabe B1:
// Leuchtdioden an PB5/PD6 blinken, wenn Taster (PD2) gedrückt ist.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

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

int taster_gedrueckt(void)
{
    if((PIND & 0b00000100) == 0) return 1;
    return 0;
}

int main(void)
{
    DDRB  = 0b00100000;  // LED an PB5
    DDRD  = 0b01000000;  // LED an PD6, Taster an PD2
    PORTD = 0b00000100;  // Pullup an PD2 einschalten!

    while (1)
    {
        if(taster_gedrueckt())
        {
            led_on();  _delay_ms(100);
            led_off(); _delay_ms(100);
        }
    }
}
