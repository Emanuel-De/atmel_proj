// ---------------------------------------------------------------------------
// Praktikum, Aufgabe C1:
// Einfaches Testprogramm für die serielle Schnittstelle.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>

#define SERIAL_OUT PD1
#define SERIAL_IN  PD0

int main(void)
{
    UCSR0B = 0;         // Arduino Nano: Disable Serial Port!!!
    DDRD = 0b00000010;  // Senden/TXD --> PD1, Empfangen/RXD --> PD0

    while(1)
    {
        if(PIND & _BV(SERIAL_IN))
            PORTD |= _BV(SERIAL_OUT);
        else
            PORTD &= ~_BV(SERIAL_OUT);
    }
}
