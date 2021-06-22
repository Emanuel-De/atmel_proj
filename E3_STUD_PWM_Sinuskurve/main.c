// ---------------------------------------------------------------------------
// Praktikum, Aufgabe E3:
// Ausgabe eines Sinussignals mittels PWM am Anschluss OC0A/PD6;
// die vorab berechneten Sinuswerte sind im Programm-Flash abgelegt.
// ---------------------------------------------------------------------------

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// ---------------------------------------------------------------------------
// Sinuswerte von 0°...358° in Schritten von 2°
// ---------------------------------------------------------------------------
const uint8_t sin_tab[] PROGMEM =
{   
    128, 132, 137, 141, 146, 150, 154, 159, 163, 167, 171, 176,
    180, 184, 188, 192, 195, 199, 203, 206, 210, 213, 216, 219,
    222, 225, 228, 231, 233, 236, 238, 240, 242, 244, 246, 247,
    249, 250, 251, 252, 253, 254, 254, 255, 255, 255, 255, 255,
    254, 254, 253, 252, 251, 250, 249, 247, 246, 244, 242, 240,
    238, 236, 233, 231, 228, 225, 222, 219, 216, 213, 210, 206,
    203, 199, 195, 192, 188, 184, 180, 176, 171, 167, 163, 159,
    154, 150, 146, 141, 137, 132, 128, 124, 119, 115, 110, 106,
    102, 97 , 93 , 89 , 85 , 80 , 76 , 72 , 68 , 65 , 61 , 57 ,
    53 , 50 , 46 , 43 , 40 , 37 , 34 , 31 , 28 , 25 , 23 , 20 ,
    18 , 16 , 14 , 12 , 10 , 9  , 7  , 6  , 5  , 4  , 3  , 2  ,
    2  , 1  , 1  , 1  , 1  , 1  , 2  , 2  , 3  , 4  , 5  , 6  ,
    7  , 9  , 10 , 12 , 14 , 16 , 18 , 20 , 23 , 25 , 28 , 31 ,
    34 , 37 , 40 , 43 , 46 , 50 , 53 , 57 , 61 , 64 , 68 , 72 ,
    76 , 80 , 85 , 89 , 93 , 97 , 102, 106, 110, 115, 119, 124,
    0  // <--- Ende-Markierung
};

// ---------------------------------------------------------------------------
// Timer-Overflow-Interrupt: Nächsten Sinuswert aus Tabelle
// lesen und den PWM-Tastgrad entsprechend einstellen.
// ---------------------------------------------------------------------------
uint16_t sin_index = 0;  // Aktuelle Position in der Sinustabelle
ISR(TIMER0_OVF_vect)
{
    // TODO...!!!
}

// ---------------------------------------------------------------------------
// PWM-Ausgabe und Timer-Overflow-Interrupt aktivieren;
// beides wird mit demselben Timer Timer/Counter0 realisiert.
// ---------------------------------------------------------------------------
void init_pwm_and_interrupt(void)
{
    DDRD = _BV(PD6);  // PWM-Ausgabe am Anschluss PD6/OC0A
    TCCR0A = _BV(COM0A1) + _BV(WGM00) + _BV(WGM01);  // S. 139f., Tab. 19-5/-9
    TCCR0B = _BV(CS02);  // clk/256-Prescaler, Datenblatt S. 142, Tab. 19-10
    TIMSK0 |= _BV(TOIE0);  // Overflow-Interrupt aktivieren
    sei();
}

// ---------------------------------------------------------------------------
// Hauptprogramm
// ---------------------------------------------------------------------------
int main(void)
{
    init_pwm_and_interrupt();
    while(1) { }  // Interrupt-Funktion erledigt den Rest...
}





#if 0
// ---------------------------------------------------------------------------
// Sinustabelle von 0°...358° in Schritten von 2° ausgeben.
// Werte sind so skaliert, dass sie im Bereich 1...255 liegen.
// ---------------------------------------------------------------------------
#include <math.h>
#include <stdio.h>

int main()
{
    int count = 0;
    printf("    ");

    for(int x = 0; x < 360; x += 2)
    {
        double y = sin(double(x) * M_PI / 180.0);
        y = y * 127.0 + 128.0;
        printf("%.0f", y);

        if(y < 100) printf(" ");
        if(y <  10) printf(" ");
        if(++count % 12 == 0)
            printf(",\n    ");
        else
            printf(", ");
    }
}
#endif
