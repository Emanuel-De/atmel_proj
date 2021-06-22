// ---------------------------------------------------------------------------
// Praktikum, Aufgabe F3:
// Automatisches Treppenlicht (Relais an PB2) über Taster (PD2) einschalten.
// Lösung der Zusatzaufgabe (Treppenlicht mit Nachdrücken) --> Praktikum G
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define TASTER PD2         // Taster an PD2
#define RELAIS PB2         // Relais an PB2
#define INTLED PB5         // Interne LED an PB5
#define DAUER_EIN 10000UL  // Wie lange (in ms) leuchtet das Treppenlicht?

// ---------------------------------------------------------------------------
// Wird der Taster gerade gedrückt?
// ---------------------------------------------------------------------------
int taster_gedrueckt(void)
{
    if(PIND & _BV(TASTER)) return 0;
    return 1;
}

// ---------------------------------------------------------------------------
// Relais ein- (1) oder ausschalten (0)
// ---------------------------------------------------------------------------
void relais_schalten(int ein_aus)
{
    if(ein_aus == 1) PORTB |=  (_BV(RELAIS) + _BV(INTLED));
    if(ein_aus == 0) PORTB &= ~(_BV(RELAIS) + _BV(INTLED));
}

// ---------------------------------------------------------------------------
// Ein- und Ausgänge initialisieren
// ---------------------------------------------------------------------------
void init_ports(void)
{
    DDRB  = _BV(RELAIS) + _BV(INTLED);  // Relais und interne LED
    PORTD = _BV(TASTER);                // Pullup am Taster
}

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    init_ports();

    while(1)
    {
        if(taster_gedrueckt())
        {
            relais_schalten(1);
            _delay_ms(DAUER_EIN);
            relais_schalten(0);
        }
    }
}
