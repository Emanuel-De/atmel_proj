// ---------------------------------------------------------------------------
// Praktikum, Aufgabe F2:
// Relais an PB2 (und LED an PB5) über USART ein- und ausschalten.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define INTLED PB5  // Interne LED an PB5
#define RELAIS PB2  // Relais an PB2

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
// Komplette Textzeile inkl. Zeilenumbruch senden.
// ---------------------------------------------------------------------------
void USART_TransmitLine(const char *text)
{
    uint16_t idx;
    for(idx = 0; text[idx] != 0; idx++)
    USART_Transmit(text[idx]);

    // Zeilenumbruch
    USART_Transmit(13); USART_Transmit(10);
}

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    DDRB = _BV(INTLED) + _BV(RELAIS);  // Ausgabeports initialisieren
    USART_Init(MYUBRR);

    USART_TransmitLine("Relais einschalten (1) und ausschalten (0)");
    USART_TransmitLine("==========================================");
    while(1)
    {
        uint8_t ch = USART_Receive();
        if(ch == '1')
        {
            PORTB |= _BV(INTLED);
            PORTB |= _BV(RELAIS);
            USART_TransmitLine("EIN");
        }
        if(ch == '0')
        {
            PORTB &= ~_BV(INTLED);
            PORTB &= ~_BV(RELAIS);
            USART_TransmitLine("AUS");
        }
    }
}


#if 0
% ---------------------------------------------------------------------------
% relais_test.m
% Relais von MATLAB aus 10 mal ein- und wieder ausschalten.
% ---------------------------------------------------------------------------

% Alle noch offenen seriellen Ports schließen
delete(instrfindall);

% Serielle Schnittstelle zum Controller öffnen
s = serial('COM6');
fopen(s);

% Etwas Zeit zum Initialisieren des Controllers
pause(5);

% Kommandos über Schnittstelle senden
for i = 1:10
    fwrite(s, '1');
    pause(1);
    fwrite(s, '0');
    pause(1);
end

% Schnittstelle nach Gebrauch wieder schließen
fclose(s);
#endif
