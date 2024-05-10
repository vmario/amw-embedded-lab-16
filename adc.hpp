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

/**
 * Globalna instancja sterownika ADC.
 */
extern const Adc adc;
