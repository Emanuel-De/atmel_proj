// ---------------------------------------------------------------------------
// Praktikum, Aufgabe G1:
// Automatisches Treppenlicht mit Möglichkeit zum "Nachdrücken" und
// Blinkfunktion kurz vor dem Abschalten (Relais an PB2, Taster an PD2)
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Anschlüsse von Taster und Relais
#define TASTER PD2
#define RELAIS PB2

// Wie lange ist die Lampe an, wie lange blinkt sie (in Millisekunden)?
#define DAUER_EIN   15000UL
#define DAUER_BLINKT 5000UL

// Globale Variable: Millisekunden seit Programmstart
volatile uint32_t _millis_intern = 0;

// ---------------------------------------------------------------------------
// Timer-Interrupt, Frequenz = 1000 Hz
// ---------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
    _millis_intern++;
}

// ---------------------------------------------------------------------------
// Millisekunden seit Programmstart abfragen
// ---------------------------------------------------------------------------
uint32_t Millis(void)
{
    uint32_t result = 0;

    // Aufpassen, dass wir während der folgenden Zuweisung
    // nicht von einem Interrupt unterbrochen werden...!!!
    cli(); result = _millis_intern; sei();
    return result;
}

// ---------------------------------------------------------------------------
// Timer initialisieren: Interrupt-Frequenz = 1000 Hz
// ---------------------------------------------------------------------------
void init_timer(void)
{
    TCCR0A = _BV(WGM01);             // CTC-Modus (siehe Tab. 19-9, S. 140)
    TCCR0B = _BV(CS01) + _BV(CS00);  // Prescaler = 64 (Tab. 19-10, S. 142)
    OCR0A  = 249;                    // Vergleichswert für Timer

    TIMSK0 = _BV(OCIE0A);            // Compare Match Interrupt (siehe S. 143)
    sei();                           // Interruptsystem des Controllers ein
}

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
    if(ein_aus == 1) PORTB |=  _BV(RELAIS);
    if(ein_aus == 0) PORTB &= ~_BV(RELAIS);
}

// Zustände, die das Treppenlicht annehmen kann
#define ZUSTAND_AUS    1
#define ZUSTAND_EIN    2
#define ZUSTAND_BLINKT 3

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    uint8_t zustand = ZUSTAND_AUS;  // Zunächst ist das Licht aus!
    uint32_t start_time = 0;

    DDRB  = _BV(RELAIS);  // Ausgang mit Relais
    PORTD = _BV(TASTER);  // Pullup am Taster
    init_timer();

    // TODO: Endlicher Automat muss noch programmiert werden...!!!
}
