// ---------------------------------------------------------------------------
// Praktikum, Aufgabe B3 (Zusatzaufgabe):
// Leuchtdioden an PB5/PD6 blinken mit 0,5 Hz, Timer-Interrupt mit 1 kHz,
// 10-fache LED-Frequenz solange der Taster gedrückt wird.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int taster_gedrueckt(void)
{
    if((PIND & 0b00000100) == 0) return 1;
    return 0;
}

void led_on(void)
{
    PORTB = 0b00100000;
    PORTD = 0b01000100;     // Aufpassen: Pullup an PD2 muss bleiben!
}

void led_off(void)
{
    PORTB = 0b00000000;
    PORTD = 0b00000100;     // Aufpassen: Pullup an PD2 muss bleiben!
}

void init_timer(void)
{
    TCCR0A = 0b00000010;    // WGM01 setzen (siehe Tab. 19-9, S. 140)
    TCCR0B = 0b00000011;    // CS00+CS01 setzen (Tab. 19-10, S. 142)
    TIMSK0 = 0b00000010;    // OCIE0A setzen (siehe S. 143)
    OCR0A  = 249;           // Vergleichswert für Timer
    sei();                  // Interrupt-System einschalten
}

unsigned long millis = 0;   // Millisekunden seit Programmstart
int led = 0;                // Aktueller Zustand der LEDs

ISR(TIMER0_COMPA_vect)
{
    // Wenn der Taster gedrückt wird: 10-fache LED-Frequenz!
    int div = 1000;
    if(taster_gedrueckt() == 1) div = 100;

    millis++;               // Millisekunden zählen
    if(millis % div == 0)   // LEDs 1x pro Sekunde umschalten
    {
        switch(led)
        {
            case 0: led_on();  led = 1; break;
            case 1: led_off(); led = 0; break;
        }
    }
}

int main(void)
{
    DDRB  = 0b00100000;     // LED an PB5
    DDRD  = 0b01000000;     // LED an PD6, Taster an PD2
    PORTD = 0b00000100;     // Pullup an PD2 einschalten!

    init_timer();
    while(1) { }            // Endlosschleife
}
