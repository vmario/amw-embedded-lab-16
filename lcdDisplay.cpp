#include "lcdDisplay.hpp"

#include "lcdDisplayBus.hpp"
#include "lcdDisplayInstruction.hpp"

#include <util/delay.h>

const LcdDisplay lcdDisplay;

/**
 * Instancja sterownika szyny danych.
 */
const LcdDisplayBus bus;

void LcdDisplay::initialize() const
{
	bus.initGpio();
	bus.initByInstruction();
	bus.init4Bit();

	bus.instructionWrite(FUNCTION_SET | FUNCTION_SET_DUAL_LINE);
	_delay_us(37);
	bus.instructionWrite(DISPLAY_CONTROL);
	_delay_us(37);
	bus.instructionWrite(CLEAR_DISPLAY);
	_delay_ms(1.52);
	bus.instructionWrite(ENTRY_MODE_SET | ENTRY_MODE_SET_INCREMENT);
	_delay_us(37);
	bus.instructionWrite(DISPLAY_CONTROL | DISPLAY_CONTROL_DISPLAY);
	_delay_us(37);
}

void LcdDisplay::write(char symbol) const
{
	bus.dataWrite(symbol);
}

void LcdDisplay::write(const char* text) const
{
	while (*text) {
		write(*text++);
	};
}

void LcdDisplay::goTo(uint8_t line, uint8_t column) const
{
	bus.instructionWrite(SET_DDRAM_ADDRESS | ((line * 0x40 + column) & 0x7f));
	_delay_us(37);
}

void LcdDisplay::addSymbol(const uint8_t* symbol, uint8_t position) const
{
	const uint8_t offset = position * 8;
	for (uint8_t i{0}; i < 8; ++i) {
		bus.instructionWrite(SET_CGRAM_ADDRESS | ((offset + i) & 0x3f));
		_delay_us(37);
		bus.dataWrite(symbol[i]);
	}
}
