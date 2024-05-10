#include "lcdDisplay.hpp"
#include "adc.hpp"

#include <avr/interrupt.h>
#include <util/atomic.h>

#include <stdio.h>

constexpr uint8_t DISPLAY_LENGTH{16}; ///< Szerokość wyświetlacza.

volatile uint16_t measurement; ///< Wynik pomiaru ADC.
volatile double temperature; ///< Obliczona temperatura.

/**
 * Obsługa przerwania pomiaru ADC.
 *
 * Tu odczytywany jest pomiar, a na jego podstawie obliczana jest temperatura.
 */
ISR(ADC_vect)
{
	measurement = adc.measure();
	temperature = 1.1 * measurement / 1024 * 100;
}

/**
 * Wyświetla pomiar i liczbę błędów w odczycie danych z przerwania ADC.
 */
void printMeasurement()
{
	static uint16_t errors;
	static uint16_t slowDown;
	char buf[DISPLAY_LENGTH + 1];

	uint16_t measurementLocal = measurement;
	double temperatureLocal = temperature;

	if (temperatureLocal != 1.1 * measurementLocal / 1024 * 100) {
		errors++;
	}

	if (slowDown++ % 0x4000) {
		return;
	}

	snprintf(buf, sizeof(buf), "%4u", measurement);
	lcdDisplay.goTo(0, 4);
	lcdDisplay.write(buf);

	snprintf(buf, sizeof(buf), "%5.2f", temperature);
	lcdDisplay.goTo(1, 5);
	lcdDisplay.write(buf);

	snprintf(buf, sizeof(buf), "%5d", errors);
	lcdDisplay.goTo(0, 11);
	lcdDisplay.write(buf);
}

/**
 * Funkcja główna.
 */
int main()
{
	constexpr uint8_t DEGREE[] = {
	    0b00000010,
	    0b00000101,
	    0b00000010,
	    0b00000000,
	    0b00000000,
	    0b00000000,
	    0b00000000,
	    0b00000000,
	};

	lcdDisplay.initialize();
	lcdDisplay.addSymbol(DEGREE, 0);
	adc.initialize();
	sei();

	lcdDisplay.goTo(0, 0);
	lcdDisplay.write("ADC:");

	lcdDisplay.goTo(0, 9);
	lcdDisplay.write("E:");

	lcdDisplay.goTo(1, 0);
	lcdDisplay.write("TEMP:      C");
	lcdDisplay.goTo(1, 10);
	lcdDisplay.write('\x00');

	while (true) {
		printMeasurement();
	}
}
