#pragma once

/**
 * Czyszczenie ekranu.
 */
enum CLEAR_DISPLAY_BITS {
	CLEAR_DISPLAY = 0x01, ///< Instrukcja czyszczenia ekranu.
};

/**
 * Home, sweet home.
 */
enum RETURN_HOME_BITS {
	RETURN_HOME = 0x02, ///< Instrukcja powrotu kursora na początek ekranu.
};

/**
 * Tryb wprowadzania tekstu.
 *
 * Decyduje o tym, co ma się dziać w trakcie wprowadzania danych do wyświetlenia.
 */
enum ENTRY_MODE_SET_BITS {
	ENTRY_MODE_SET = 0x04, ///< Instrukcja konfiguracji trybu wprowadzania tekstu.

	ENTRY_MODE_SET_INCREMENT = 0x02, ///< Przesuwaj kursor w prawo.
	ENTRY_MODE_SET_SHIFT = 0x01, ///< Przesuwaj ekran zamiast kursora.
};

/**
 * Włączanie i wyłączanie wyświetlania.
 */
enum DISPLAY_CONTROL_BITS {
	DISPLAY_CONTROL = 0x08, ///< Instrukcja włączająca wyświetlanie.

	DISPLAY_CONTROL_DISPLAY = 0x04, ///< Wyświetlacz włączony.
	DISPLAY_CONTROL_CURSOR = 0x02, ///< Kursor włączony.
	DISPLAY_CONTROL_BLINK = 0x01, ///< Mruganie kursora włączone.
};

/**
 * Przesuwanie kursora i ekranu.
 */
enum SHIFT_BITS {
	SHIFT = 0x10, ///< Instrukcja przesuwania.

	SHIFT_DISPLAY = 0x08, ///< Przesuwanie ekranu.
	SHIFT_RIGHT = 0x04, ///< Przesuwanie w prawo.
};

/**
 * Dodatkowe funkcje.
 */
enum FUNCTION_SET_BITS {
	FUNCTION_SET = 0x20, ///< Instrukcja ustawiania funkcji.

	FUNCTION_SET_8BIT = 0x10, ///< Szyna 8-bitowa (domyśnie 4-bitowa).
	FUNCTION_SET_DUAL_LINE = 0x08, ///< Wyświetlacz z dwoma liniami tekstu.
	FUNCTION_SET_FONT = 0x08, ///< Font 5×10 (domyślnie 5×8).
};

/**
 * Adres w pamięci CGRAM (Character Generator RAM).
 */
enum SET_CGRAM_ADDRESS_BITS {
	SET_CGRAM_ADDRESS = 0x40, ///< Instrukcja wprowadzania adresu CGRAM.
};

/**
 * Adres w pamięci DDRAM (Display Data RAM).
 */
enum SET_DDRAM_ADDRESS_BITS {
	SET_DDRAM_ADDRESS = 0x80, ///< Instrukcja wprowadzania adresu DDRAM.
};
