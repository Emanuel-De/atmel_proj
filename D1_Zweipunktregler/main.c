// ---------------------------------------------------------------------------
// Praktikum, Aufgabe D1:
// Zweipunktregler: LEDs an PB5/PD6 ein, wenn Spannung an PC0 über 2 Volt
// steigt, bzw. wieder aus, wenn Spannung an PC0 unter 1,5 Volt abfällt.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

#define INTERNAL_LED PB5  // LED an PB5 (auf der Arduino-Platine)
#define EXTERNAL_LED PD6  // LED an PC6 (extern angeschlossen)
#define LIMIT_LO (uint16_t)(1.5 * 1023.0 / 5.0)  // Untere Schwelle
#define LIMIT_HI (uint16_t)(2.0 * 1023.0 / 5.0)  // Obere Schwelle

// ---------------------------------------------------------------------------
// Digitalausgang für LED initialisieren, AD-Wandler initialisieren
// ---------------------------------------------------------------------------
void init_ports_and_adc(void)
{
    // Ausgänge mit Leuchtdioden
    DDRB = _BV(INTERNAL_LED);
    DDRD = _BV(EXTERNAL_LED);

    // Referenz für AD-Wandler = VCC = 5V (Datenblatt S. 317)
    ADMUX = _BV(REFS0);
    
    // AD-Wandler einschalten, Prescaler = 128 (Datenblatt S. 320)
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// ---------------------------------------------------------------------------
// Analogwert von Eingang ADC0 (Anschluss PC0) einlesen:
// Rückgabewert 0...1023 entspricht Spannung von 0...5 Volt
// ---------------------------------------------------------------------------
uint16_t read_adc0(void)
{
    // ADSC-Bit setzen, um Wandlung zu starten, das ADSC-Bit wird nach
    // dem Ende der Wandlung automatisch wieder gelöscht. Der eingelesene
    // 10-Bit-Wert steht anschließend im Register ADC.
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC)) { }  // Auf das Ende der Wandlung warten...
    return ADC;
}

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    init_ports_and_adc();
    while(1)
    {
        uint16_t adc = read_adc0();
        if(adc > LIMIT_HI)
        {
            PORTB |= _BV(INTERNAL_LED);
            PORTD |= _BV(EXTERNAL_LED);
        } 
        if(adc < LIMIT_LO)
        {
            PORTB &= ~_BV(INTERNAL_LED);
            PORTD &= ~_BV(EXTERNAL_LED);
        }
    }
}
