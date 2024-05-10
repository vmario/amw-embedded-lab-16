#pragma once

#include <stdint.h>

/**
 * Sterownik ADC.
 */
struct Adc {
	/**
	 * Konfiguruje i uruchamia ADC.
	 */
	void initialize() const;

	/**
	 * Odczytuje wynik pomiaru z ADC.
	 *
	 * @return Wynik pomiaru.
	 */
	uint16_t measure() const;
};

extern const Adc adc; ///< Globalna instancja sterownika ADC.

extern volatile uint16_t keyboardAdc; ///< Wynik pomiaru napięcia z klawiatury.
extern volatile uint16_t temperatureAdc; ///< Wynik pomiaru napięcia z termometru.
