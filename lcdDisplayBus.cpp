#include "lcdDisplayBus.hpp"

#include "lcdDisplayInstruction.hpp"

#include <avr/io.h>
#include <util/delay.h>

#define LCD_RS_DIR DDRB ///< Rejestr DDR linii RS.
#define LCD_RS_PORT PORTB ///< Rejestr PORT linii RS.
#define LCD_RS_BIT _BV(PB0) ///< Numer pinu linii RS.

#define LCD_E_DIR DDRB ///< Rejestr DDR linii E.
#define LCD_E_PORT PORTB ///< Rejestr PORT linii E.
#define LCD_E_BIT _BV(PB1) ///< Numer pinu linii E.

#define LCD_DB_DIR DDRD ///< Rejestr DDR linii danych.
#define LCD_DB_PORT PORTD ///< Rejestr PORT linii danych.
#define LCD_DB_OFFSET PD4 ///< Numer pierwszego pinu linii danych.

constexpr uint8_t NIBBLE_MASK{0x0f}; ///< Maska półbajtu.

/**
 * Przycina instrukcję do formy używanej podczas inicjalizacji (starszego półbajtu).
 *
 * @param instruction Wyjściowa instrukcja.
 *
 * @return Instrukcja przycięta do półbajtu.
 */
constexpr uint8_t INIT_INSTRUCTION(uint8_t instruction)
{
	return instruction >> 4;
}

void LcdDisplayBus::initGpio() const
{
	LCD_DB_DIR |= NIBBLE_MASK << LCD_DB_OFFSET;
	LCD_E_DIR |= LCD_E_BIT;
	LCD_RS_DIR |= LCD_RS_BIT;
}

void LcdDisplayBus::initByInstruction() const
{
	_delay_ms(40);
	nibbleWrite(INIT_INSTRUCTION(FUNCTION_SET | FUNCTION_SET_8BIT));
	_delay_us(4100);
	nibbleWrite(INIT_INSTRUCTION(FUNCTION_SET | FUNCTION_SET_8BIT));
	_delay_us(100);
	nibbleWrite(INIT_INSTRUCTION(FUNCTION_SET | FUNCTION_SET_8BIT));
	_delay_us(37);
}

void LcdDisplayBus::init4Bit() const
{
	nibbleWrite(INIT_INSTRUCTION(FUNCTION_SET));
	_delay_ms(37);
}

void LcdDisplayBus::nibbleWrite(uint8_t nibble) const
{
	constexpr uint8_t LCD_DB_PORT_MASK = (~(NIBBLE_MASK << LCD_DB_OFFSET)) & 0xff;

	LCD_E_PORT |= LCD_E_BIT;
	LCD_DB_PORT = (nibble << LCD_DB_OFFSET) | (LCD_DB_PORT & LCD_DB_PORT_MASK);
	_delay_us(1);
	LCD_E_PORT &= ~LCD_E_BIT;
	_delay_us(1);
}

void LcdDisplayBus::byteWrite(uint8_t byte) const
{
	nibbleWrite(byte >> 4);
	nibbleWrite(byte);
}

void LcdDisplayBus::instructionWrite(uint8_t instruction) const
{
	LCD_RS_PORT &= ~LCD_RS_BIT;
	byteWrite(instruction);
}

void LcdDisplayBus::dataWrite(uint8_t data) const
{
	LCD_RS_PORT |= LCD_RS_BIT;
	byteWrite(data);
	_delay_us(37);
}

