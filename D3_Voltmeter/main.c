// ---------------------------------------------------------------------------
// Praktikum, Aufgabe D3:
// Voltmeter mit 10 Messungen pro Sekunde;
// Messwerte werden auf serieller Schnittstelle ausgegeben.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// ---------------------------------------------------------------------------
// Timer initialisieren: 1000 Timer-Interrupts pro Sekunde
// ---------------------------------------------------------------------------
void init_timer(void)
{
    TCCR0A = 0b00000010;  // WGM01 setzen (siehe Tab. 19-9, S. 140)
    TCCR0B = 0b00000011;  // CS00+CS01 setzen (Tab. 19-10, S. 142)
    TIMSK0 = 0b00000010;  // OCIE0A setzen (siehe S. 143)
    OCR0A  = 249;         // Vergleichswert für Timer
    sei();                // Interrupt-System einschalten
}

// ---------------------------------------------------------------------------
// Timer-Interrupt: Alle 100 ms wird die globale Variable "want_adc" auf 1
// gesetzt. Dies führt im Hauptprogramm (!) dazu, dass eine neue AD-Wandlung
// erfolgt und das Ergebnis auf die serielle Schnittstelle geschrieben wird.
// ---------------------------------------------------------------------------
uint32_t millis = 0;            // Millisekunden seit Programmstart
volatile uint8_t want_adc = 0;  // want_adc = 1, um AD-Wandlung anzufordern
ISR(TIMER0_COMPA_vect)
{
    millis++;
    if(millis % 100 == 0) want_adc = 1;
}

// ---------------------------------------------------------------------------
// AD-Wandler initialisieren
// ---------------------------------------------------------------------------
void init_adc(void)
{
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
// 8 Analogwerte von Eingang ADC0 (Anschluss PC0) einlesen und mitteln:
// Rückgabewert 0...1023 entspricht Spannung von 0...5 Volt
// ---------------------------------------------------------------------------
uint16_t read_adc0_mean(void)
{
    uint16_t sum = 0;
    uint8_t i;
    for(i = 0; i < 8; i++) sum += read_adc0();
    return sum / 8;
}

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
    char msg[] = "0.00 Volt";

    USART_Init(MYUBRR);
    init_adc();
    init_timer();

    while(1)
    {
        if(want_adc == 1)
        {
            uint32_t analog = read_adc0_mean();
            analog *=  500;  // Wert zwischen 0...1023 umrechnen in 
            analog /= 1023;  // 0...500 (also hundertstel Volt)...

            msg[3] = '0' + analog % 10; analog /= 10;
            msg[2] = '0' + analog % 10; analog /= 10;
            msg[0] = '0' + analog % 10;

            USART_TransmitLine(msg);
            want_adc = 0;
        }
    }
}
