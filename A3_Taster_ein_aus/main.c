// ---------------------------------------------------------------------------
// Praktikum, Aufgabe A3 (Zusatzaufgabe):
// Leuchtdioden an PB5/PD6 per Taster (PD2) ein- und wieder ausschalten.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Welchen Zustand sollen die LEDs beim n�chsten Tastendruck annehmen?
    int led_on = 1;

    DDRB  = 0b00100000;  // LED an PB5
    DDRD  = 0b01000000;  // LED an PD6, Taster an PD2
    PORTD = 0b00000100;  // Pullup-Widerstand an PD2 einschalten!

    while (1)
    {
        // Abwarten, solange Taster nicht gedr�ckt ist...
        while((PIND & 0b00000100) != 0) { }

        // Taster wurde gerade bet�tigt und k�nnte noch prellen...
        _delay_ms(100);

        if(led_on == 1)
        {
            // Taster gedr�ckt --> Eingang PD2 ist null
            PORTB = 0b00100000;
            PORTD = 0b01000100;  // Aufpassen: Pullup an PD2 muss bleiben!
            led_on = 0;          // Beim n�chsten Tastendruck: LEDs aus
        }
        else
        {
            PORTB = 0b00000000;
            PORTD = 0b00000100;  // Aufpassen: Pullup an PD2 muss bleiben!
            led_on = 1;          // Beim n�chsten Tastendruck: LEDs ein
        }

        // Abwarten, solange Taster gedr�ckt ist...
        while((PIND & 0b00000100) == 0) { }

        // Taster wurde gerade losgelassen und k�nnte noch prellen...
        _delay_ms(100);
    }
}
