// ---------------------------------------------------------------------------
// Praktikum, Aufgabe C3:
// Echoprogramm mit eingebauter serieller Schnittstelle ("USART")
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <inttypes.h>
#include <avr/io.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(uint16_t ubrr)
{
    // USART initialisieren
    // Siehe Datenblatt, 24.6. USART Initialization (S. 230)
}

void USART_Transmit(uint8_t data)
{
    // Ein einzelnes Zeichen über die Schnittstelle senden
    // Siehe Datenblatt, 24.7. Data Transmission (S. 231)
}

uint8_t USART_Receive(void)
{
    // Ein einzelnes Zeichen über die Schnittstelle empfangen
    // Siehe Datenblatt, 24.8. Data Reception (S. 233)
}

int main(void)
{
    USART_Init(MYUBRR);
    while(1)
    {
        uint8_t ch = USART_Receive();
        USART_Transmit(ch);
    }
}
