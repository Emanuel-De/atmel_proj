// ---------------------------------------------------------------------------
// Praktikum, Aufgabe C2:
// Serielle Schnittstelle, Bitmuster für Datenübertragung "manuell" erzeugen
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERIAL_OUT PD1  // Anschluss zum Senden
#define SERIAL_IN  PD0  // Anschluss zum Empfangen, wird hier nicht benutzt

//                           +-------------------------------- Startbit
//                           |  +----------------------------- Datenbit 0
//                           |  |  +--+--+--+--+--+----------- Datenbit 1..6
//                           |  |  |  |  |  |  |  |  +-------- Datenbit 7
//                           |  |  |  |  |  |  |  |  |  +----- Stoppbit
//                           |  |  |  |  |  |  |  |  |  |
int8_t bits_zum_senden[] = { 0, 0, 0, 0, 1, 0, 0, 1, 0, 1,
                             0, 1, 0, 1, 1, 0, 0, 1, 0, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // kleine Pause...
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // kleine Pause...
                            -1 };  // Ende der Übertragung - zurück zum Anfang

// Index des nächsten zu sendenden Bits
int8_t idx = 0;

// Timer Compare Match Interrupt
ISR(TIMER0_COMPA_vect)
{
    if(bits_zum_senden[idx] == 1) PORTD |=  _BV(SERIAL_OUT);
    if(bits_zum_senden[idx] == 0) PORTD &= ~_BV(SERIAL_OUT);
    
    ++idx;
    if(bits_zum_senden[idx] == -1) idx = 0;
}

// PORTD initialisieren, Timer auf 9.6kHz einstellen
void init_port_and_timer(void)
{
    UCSR0B = 0;                // Arduino Nano: USART deaktivieren!!!
    DDRD   = _BV(SERIAL_OUT);  // TXD-Anschluss als Ausgang aktivieren

    TCCR0A = _BV(WGM01);       // CTC Modus ("Clear Timer on Compare Match")
    TCCR0B = _BV(CS01);        // Prescaler: 8
    OCR0A  = 207;              // Vergleichswert für Timer --> 9.6 kHz
    
    TIMSK0 = _BV(OCIE0A);      // Compare Interrupt erlauben
    sei();                     // Interruptsystem aktivieren
}

// Hauptprogramm
int main(void)
{
    init_port_and_timer();
    while(1) { /* Endlosschleife */ }
}

