// ---------------------------------------------------------------------------
// Praktikum, Aufgabe G3:
// Einfaches Steuergerät für Kfz-Innenbeleuchtung (Fahrzeugtürstellung
// wird mit regelbarem Widerstand an PC0 erfasst, Taster zum manuellen
// Ein-/Ausschalten an PD2, Relais zum Anschluss der Lampe an PB2)
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Anschlüsse von Taster und Relais
#define TASTER PD2
#define RELAIS PB2

// ---------------------------------------------------------------------------
// Hat der Anwender den Taster betätigt (und wieder losgelassen)...?
// ---------------------------------------------------------------------------
int tastendruck(void)
{
    if((PIND & _BV(TASTER)) != 0) return 0;

    // Abwarten, bis der Taster wieder losgelassen wird;
    // kurze Delays dienen zum Entprellen des Tasters...
    _delay_ms(100);
    while((PIND & _BV(TASTER)) == 0) { }
    _delay_ms(100);
    return 1;
}

// ---------------------------------------------------------------------------
// Relais ein- (1) oder ausschalten (0)
// ---------------------------------------------------------------------------
void relais_schalten(int ein_aus)
{
    if(ein_aus == 1) PORTB |=  _BV(RELAIS);
    if(ein_aus == 0) PORTB &= ~_BV(RELAIS);
}

// ---------------------------------------------------------------------------
// Ein- und Ausgänge initialisieren
// ---------------------------------------------------------------------------
void init_ports(void)
{
    DDRB  = _BV(RELAIS);  // Ausgang mit Relais
    PORTD = _BV(TASTER);  // Pullup am Taster
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
    while(ADCSRA & _BV(ADSC)) { }  // Auf das Ende der Wandlung warten
    return ADC;
}

// ---------------------------------------------------------------------------
// Acht Analogwerte von ADC0 einlesen, Mittelwert berechnen und zurückgeben
// ---------------------------------------------------------------------------
uint16_t read_adc0_mean(void)
{
    uint8_t i;
    uint16_t sum = 0;
    for(i = 0; i < 8; ++i) sum += read_adc0();
    return sum / 8;
}

// Schwellwerte zur Erkennung der Türposition
#define SCHWELLE_AUF 600
#define SCHWELLE_ZU  400

// Mögliche Zustände der Kfz-Innenbeleuchtung
#define ZUSTAND_AUS 1
#define ZUSTAND_AN  2

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    init_ports();
    init_adc();

    uint8_t state = ZUSTAND_AUS;
    uint8_t manuell_ein = 0;

    while(1)
    {
        switch(state)
        {
        case ZUSTAND_AUS:
            if(manuell_ein == 1 || read_adc0_mean() > SCHWELLE_AUF) { relais_schalten(1); state = ZUSTAND_AN; }
            if(tastendruck() == 1) manuell_ein = 1;
            break;

        case ZUSTAND_AN:
            if(manuell_ein == 0 && read_adc0_mean() < SCHWELLE_ZU) { relais_schalten(0); state = ZUSTAND_AUS; }
            if(tastendruck() == 1) manuell_ein = 1 - manuell_ein;
            break;
        }
    }
}
