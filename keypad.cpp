#include "keypad.hpp"

#include "adc.hpp"

Keypad keypad;

void Keypad::measure(uint16_t measurement)
{
	this->measurement = measurement;
}

KEY Keypad::currentKey() const
{
	if (measurement < 1.0 / 10 * 1024) {
		return KEY_RIGHT;
	} else if (measurement < 3.0 / 10 * 1024) {
		return KEY_UP;
	} else if (measurement < 5.0 / 10 * 1024) {
		return KEY_DOWN;
	} else if (measurement < 7.0 / 10 * 1024) {
		return KEY_LEFT;
	} else if (measurement < 9.0 / 10 * 1024) {
		return KEY_SELECT;
	} else {
		return KEY_NONE;
	}
}

KEY Keypad::oneTimeKey()
{
	if (currentKey() != debouncedKey) {
		debouncedKey = currentKey();
		return KEY_NONE;
	}

	if (debouncedKey != lastKey) {
		lastKey = debouncedKey;
		return lastKey;
	} else {
		return KEY_NONE;
	}
}
