#pragma once

#include <stdint.h>

/**
 * Sterownik wyświetlacza alfanumerycznego, korzystający ze sterownika Hitachi HD44780.
 * 
 * Zapis następuje na pozycji, w której aktualnie znajduje się kursor.
 */
struct LcdDisplay {
	/**
	 * Inicjalizuje wyświetlacz.
	 */
	void initialize() const;

	/**
	 * Czyści wyświetlacz.
	 */
	void clear() const;

	/**
	 * Zapisuje symbol na wyświetlaczu.
	 * 
	 * @param symbol Symbol do zapisania.
	 */
	void write(char symbol) const;

	/**
	 * Zapisuje tekst na wyświetlaczu.
	 * 
	 * @param symbol Tekst do zapisania.
	 */
	void write(const char* text) const;

	/**
	 * Ustawia kursor na wskazanej pozycji.
	 *
	 * @param line Numer linii.
	 * @param column Numer kolumny.
	 */
	void goTo(uint8_t line = 0, uint8_t column = 0) const;

	/**
	 * Dodaje symbol do pamięci symboli.
	 *
	 * @param symbol Tablica 8 bajtów, opisujących 5×8 pikseli.
	 * @param position Numer symbolu w pamięci (0…7).
	 */
	void addSymbol(const uint8_t* symbol, uint8_t position) const;
};

/**
 * Instancja sterownika wyświetlacza.
 */
extern const LcdDisplay lcdDisplay;
