// ---------------------------------------------------------------------------
// Praktikum, Aufgabe C4:
// Leuchtdioden an PB5/PD6 über USART ein- und ausschalten.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define LED1 PB5
#define LED2 PD6

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
    DDRB = _BV(LED1);  // Interne LED an PB5
    DDRD = _BV(LED2);  // Externe LED an PD6
    USART_Init(MYUBRR);

    while(1)
    {
        uint8_t ch = USART_Receive();
        if(ch == '1')
        {
            PORTB |= _BV(LED1);
            PORTD |= _BV(LED2);
            USART_Transmit('E'); USART_Transmit('I'); USART_Transmit('N'); USART_Transmit(' ');
        }
        if(ch == '0')
        {
            PORTB &= ~_BV(LED1);
            PORTD &= ~_BV(LED2);
            USART_Transmit('A'); USART_Transmit('U'); USART_Transmit('S'); USART_Transmit(' ');
        }
    }
}
