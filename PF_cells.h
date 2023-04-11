#ifndef PF_CELLS_H
#define PF_CELLS_H

/* Stała zawierająca ilość kierunków                                                             */
#define SIDES 4
/* Enumerator służący czytelnemu określeniu kierunków                                            */
typedef enum {
	N,
	E,
	S,
	W
} side;

/* Enumerator służący czytelnemu określeniu kolorów                                              */
typedef enum {
	BLACK,
	RED,
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	WHITE
} color;

/* Struktura zawierająca dane komórki                                                            */
typedef struct cell {
	struct cell* next[SIDES];
	char state;
	color cellcolor;
} cellmap;

/* Funkcja tworząca planszę o określonym wymiarze                                                */
int cells_set(cellmap** topleft, int height, int width);

/* Funkcja zwracająca adres komórki na określonej pozycji                                        */
cellmap* cells_get_cell(cellmap* topleft, int x, int y);

/* Funkcja zwalniająca pamięć wszystkich komórek                                                 */
void cells_free(cellmap* topleft);

/* Funkcja konwertująca znak kierunku na enumerator. Zwraca 0 (północ) dla niepoprawnej wartości */
side cells_char_to_side(char sd);

/* Funkcja konwertująca enumerator kierunku na znak. Zwraca '\0' dla niepoprawnej wartości       */
char cells_side_to_char(side sd);

/* Funkcja konwertująca łańcuch na enumerator koloru                 */
color cells_str_to_color(char* str);

#endif