#pragma once

#include <stdint.h>

/**
 * Szyna danych między wyświetlaczem LCD a µC.
 */
struct LcdDisplayBus {
	/**
	 * Inicjalizuje GPIO.
	 */
	void initGpio() const;

	/**
	 * Inicjalizuje LCD w razie, gdyby reset sprzętowy nie zadziałał poprawnie.
	 */
	void initByInstruction() const;

	/**
	 * Przełącza szynę w tryb 4-bitowy.
	 */
	void init4Bit() const;

	/**
	 * Wysyła półbajt.
	 *
	 * @param nibble Półbajt do wysłania.
	 */
	void nibbleWrite(uint8_t nibble) const;

	/**
	 * Wysyła bajt w trybie 4-bitowym.
	 *
	 * @param nibble Bajt do wysłania.
	 */
	void byteWrite(uint8_t byte) const;

	/**
	 * Wysyła instrukcję.
	 *
	 * Nie odmierza opóźnienia, ponieważ zależy ono od rodzaju instrukcji.
	 *
	 * @param instruction Instrukcja do wysłania.
	 */
	void instructionWrite(uint8_t instruction) const;

	/**
	 * Wysyła dane.
	 *
	 * Odmierza niezbędne opóźnienie.
	 *
	 * @param data Bajt danych do wysłania.
	 */
	void dataWrite(uint8_t data) const;
};
