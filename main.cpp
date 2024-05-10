#include "adc.hpp"
#include "keypad.hpp"
#include "lcdDisplay.hpp"

#include <util/atomic.h>

#include <stdio.h>

constexpr uint8_t DISPLAY_LENGTH{16}; ///< Szerokość wyświetlacza.

/**
 * Wyświetla pomiar klawiatury i temperatury.
 */
void mainLoop()
{
	char buf[DISPLAY_LENGTH + 1];

	double temperature;
	uint16_t keyboardAdcLocal;
	uint16_t temperatureAdcLocal;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		keyboardAdcLocal = keyboardAdc;
		temperatureAdcLocal = temperatureAdc;
		temperature = 1.1 * temperatureAdc / 1024 * 100;
	}

	snprintf(buf, sizeof(buf), "%4u", keyboardAdcLocal);
	lcdDisplay.goTo(0, 3);
	lcdDisplay.write(buf);

	lcdDisplay.goTo(0, 10);
	keypad.measure(keyboardAdcLocal);
	switch (keypad.currentKey()) {
	case KEY_SELECT:
		lcdDisplay.write("SELECT");
		break;
	case KEY_LEFT:
		lcdDisplay.write("LEFT");
		break;
	case KEY_RIGHT:
		lcdDisplay.write("RIGHT");
		break;
	case KEY_UP:
		lcdDisplay.write("UP");
		break;
	case KEY_DOWN:
		lcdDisplay.write("DOWN");
		break;
	case KEY_NONE:
	default:
		lcdDisplay.write("      ");
		break;
	}

	snprintf(buf, sizeof(buf), "%4u", temperatureAdcLocal);
	lcdDisplay.goTo(1, 3);
	lcdDisplay.write(buf);

	snprintf(buf, sizeof(buf), "%4.1f", temperature);
	lcdDisplay.goTo(1, 10);
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

	lcdDisplay.goTo(0, 0);
	lcdDisplay.write("A0:");
	lcdDisplay.goTo(0, 8);
	lcdDisplay.write("K:");

	lcdDisplay.goTo(1, 0);
	lcdDisplay.write("A5:");
	lcdDisplay.goTo(1, 8);
	lcdDisplay.write("T:     C");
	lcdDisplay.goTo(1, 14);
	lcdDisplay.write('\x00');

	adc.initialize();
	sei();

	while (true) {
		mainLoop();
	}
}
