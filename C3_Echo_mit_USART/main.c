// ---------------------------------------------------------------------------
// Praktikum, Aufgabe C3:
// Echoprogramm mit eingebauter serieller Schnittstelle ("USART")
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// USART initialisieren
// Siehe Datenblatt, 24.6. USART Initialization (S. 230)
void USART_Init(uint16_t ubrr)
{
    UBRR0L = (uint8_t)(ubrr);
    UBRR0H = (uint8_t)(ubrr >> 8);       // Set baud rate
    UCSR0B = _BV( RXEN0) + _BV( TXEN0);  // Enable receiver & transmitter
    UCSR0C = _BV(UCSZ00) + _BV(UCSZ01);  // 8 data bits, 1 stop bit
}

// Ein einzelnes Zeichen über die Schnittstelle senden
// Siehe Datenblatt, 24.7. Data Transmission (S. 231)
void USART_Transmit(uint8_t data)
{
    while(!(UCSR0A & _BV(UDRE0))) { }    // Wait for empty transmit buffer
    UDR0 = data;                         // Put data into transmit buffer
}

// Ein einzelnes Zeichen über die Schnittstelle empfangen
// Siehe Datenblatt, 24.8. Data Reception (S. 233)
uint8_t USART_Receive(void)
{
    while (!(UCSR0A & _BV(RXC0))) { }    // Wait for data to be received
    return UDR0;                         // Get and return received data
}

// Hauptprogramm
int main(void)
{
    USART_Init(MYUBRR);
    while(1)
    {
        uint8_t ch = USART_Receive();
        USART_Transmit(ch);
    }
}

