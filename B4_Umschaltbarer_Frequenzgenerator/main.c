// ---------------------------------------------------------------------------
// Praktikum, Aufgabe B4:
// Frequenz der Leuchtdioden an PB5/PD6 kann per Taster (PD2)
// zwischen 1, 5, 10 und 50 Hz umgeschaltet werden.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int frequenz_weiterschalten(void)
{
    // Taster nicht gedrückt --> Rückgabe == 0.
    if((PIND & 0b00000100) != 0) return 0;

    // Kurz warten, Taster kann prellen...
    _delay_ms(100);

    // Abwarten, bis Taster wieder losgelassen wird...
    while((PIND & 0b00000100) == 0) { }

    // Nochmals kurz warten, Taster kann wieder prellen...
    _delay_ms(100);

    // Frequenz soll verändert werden --> Rückgabe == 1.
    return 1;
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

volatile char state = 1;    // Nummer der aktuellen Frequenz (1...4)
unsigned long millis = 0;   // Millisekunden seit Programmstart
int led = 0;                // Aktueller Zustand der LEDs

ISR(TIMER0_COMPA_vect)
{
    // Die aktuelle Frequenz (1...4) in der Variablen
    // "state" bestimmt den aktuellen Teiler...
    int div = 1000;
    switch(state)
    {
    case 1: div = 500; break;
    case 2: div = 100; break;
    case 3: div =  50; break;
    case 4: div =  10; break;
    }

    millis++;               // Millisekunden zählen
    if(millis % div == 0)   // LEDs blinken lassen
    {
        switch(led)
        {
            case 0: led_on();  led = 1; break;
            case 1: led_off(); led = 0; break;
        }
    }
}

void init_timer(void)
{
    TCCR0A = 0b00000010;    // WGM01 setzen (siehe Tab. 19-9, S. 140)
    TCCR0B = 0b00000011;    // CS00+CS01 setzen (Tab. 19-10, S. 142)
    TIMSK0 = 0b00000010;    // OCIE0A setzen (siehe S. 143)
    OCR0A  = 249;           // Vergleichswert für Timer
    sei();                  // Interrupt-System einschalten
}

int main(void)
{
    DDRB  = 0b00100000;     // LED an PB5
    DDRD  = 0b01000000;     // LED an PD6, Taster an PD2
    PORTD = 0b00000100;     // Pullup an PD2 einschalten!
    init_timer();

    while(1)                // Endlosschleife
    {
        // Wenn der Anwender die Frequenz verändern möchte,
        // wird die Variable "state" auf einen neuen Wert
        // gesetzt. Der Timer-Interrupt erledigt den Rest...
        if(frequenz_weiterschalten() == 1)
        {
            switch(state)
            {
                case 1: state = 2; break;
                case 2: state = 3; break;
                case 3: state = 4; break;
                case 4: state = 1; break;
            }
        }
    }
}
