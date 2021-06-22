// ---------------------------------------------------------------------------
// Praktikum, Aufgabe G2:
// Elektronisches Codeschloss ("ABCBA") mit serieller Schnittstelle
// und Relais (am Anschluss PB2)
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define RELAIS PB2

// ---------------------------------------------------------------------------
// USART initialisieren
// Siehe Datenblatt, 24.6. USART Initialization (S. 230)
// ---------------------------------------------------------------------------
void USART_Init(uint16_t ubrr)
{
    UBRR0L = (uint8_t)(ubrr);
    UBRR0H = (uint8_t)(ubrr >> 8);       // Set baud rate
    UCSR0B = _BV( RXEN0) + _BV( TXEN0);  // Enable receiver & transmitter
    UCSR0C = _BV(UCSZ00) + _BV(UCSZ01);  // 8 data bits, 1 stop bit
}

// ---------------------------------------------------------------------------
// Ein einzelnes Zeichen über die Schnittstelle senden
// Siehe Datenblatt, 24.7. Data Transmission (S. 231)
// ---------------------------------------------------------------------------
void USART_Transmit(uint8_t data)
{
    while(!(UCSR0A & _BV(UDRE0))) { }    // Wait for empty transmit buffer
    UDR0 = data;                         // Put data into transmit buffer
}

// ---------------------------------------------------------------------------
// Ein einzelnes Zeichen über die Schnittstelle empfangen
// Siehe Datenblatt, 24.8. Data Reception (S. 233)
// ---------------------------------------------------------------------------
uint8_t USART_Receive(void)
{
    while (!(UCSR0A & _BV(RXC0))) { }    // Wait for data to be received
    return UDR0;                         // Get and return received data
}

// ---------------------------------------------------------------------------
// Eine komplette Textzeile über die Schnittstelle senden,
// es wird automatisch ein Zeilenumbruch mitgesendet.
// ---------------------------------------------------------------------------
void USART_TransmitLine(const char *text)
{
    int i;
    for(i = 0; text[i]; ++i) USART_Transmit((uint8_t)text[i]);
    USART_Transmit(13); USART_Transmit(10);  // Zeilenumbruch!
}

// ---------------------------------------------------------------------------
// Relais ein- (1) oder ausschalten (0)
// ---------------------------------------------------------------------------
void relais_schalten(int ein_aus)
{
    if(ein_aus == 1) PORTB |=  _BV(RELAIS);
    if(ein_aus == 0) PORTB &= ~_BV(RELAIS);
}

// Liste der möglichen Zustände
#define ZUSTAND_0 0
#define ZUSTAND_1 1
#define ZUSTAND_2 2
#define ZUSTAND_3 3
#define ZUSTAND_4 4
#define ZUSTAND_5 5

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    DDRB = _BV(RELAIS);         // Ausgang mit Relais aktivieren
    uint8_t relais_on = 0;      // Relais ist zunächst ausgeschaltet
    uint8_t state = ZUSTAND_0;  // Startzustand

    USART_Init(MYUBRR);
    USART_TransmitLine("Elektronisches Codeschloss; bitte Code eingeben:");
    USART_TransmitLine("================================================");
    USART_TransmitLine("");

    while(1)
    {
        uint8_t ch = USART_Receive();
        switch(state)
        {
        case ZUSTAND_0:
            if(ch == 'A') state = ZUSTAND_1;
            break;

        case ZUSTAND_1:
            state = ZUSTAND_0;
            if(ch == 'B') state = ZUSTAND_2;
            if(ch == 'A') state = ZUSTAND_1;
            break;

        case ZUSTAND_2:
            state = ZUSTAND_0;
            if(ch == 'C') state = ZUSTAND_3;
            if(ch == 'A') state = ZUSTAND_1;
            break;

        case ZUSTAND_3:
            state = ZUSTAND_0;
            if(ch == 'B') state = ZUSTAND_4;
            if(ch == 'A') state = ZUSTAND_1;
            break;

        case ZUSTAND_4:
            state = ZUSTAND_0;
            if(ch == 'A') state = ZUSTAND_5;
        }

        // Relais umschalten, falls der korrekte Code eingegeben wurde...
        if(state == ZUSTAND_5)
        {
            relais_on = 1 - relais_on;
            relais_schalten(relais_on);
            state = ZUSTAND_0;  // Weiter mit Start-Zustand...
        }
    }
}

