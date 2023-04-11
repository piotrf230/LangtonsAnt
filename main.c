#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "PF_cells.h"
#include "gifenc.h"

#define CONFLINEMAX 80
#define CONFNAME "config.txt"
#define OUTGIF "out.gif"

#define MAXANTS 9

typedef struct {
	cellmap* position;
	side facing;
	color antcolor;
	char symbol;
} ant;

/* Czyta plik konfiguracyjny, alokuje pamięć, przygotowuje symulację */
int startup(cellmap** topleft, ant** ants, int* antscount, unsigned long long int* steps, char* out, uint16_t size[], uint16_t* delay);

/* Wyświetla planszę w konsoli                                       */
void print_cons(cellmap* topleft, uint16_t size[], int delay, ant* ants, int antscount);
/* Zapisuje klatkę do pliku                                          */
void print_gif(ge_GIF* giffile, cellmap* topleft, uint16_t size[], int delay, ant* ants, int antscount);

/* Wykonuje jeden ruch mrówki                                        */
void ant_action(ant* currant);
/* Sprawdza, czy na komórce jest mrówka                              */
int ant_on_cell(cellmap* cell, ant* ants, int antcount);

int main() {
	cellmap* topleft = 0;
	ant* ants = 0;
	unsigned long long int steps = 0, currstep = 0;
	int antscount = 0, i = 0;
	char output = 0;
	uint16_t size[2], delay = 1;
	uint8_t palette[] = { /* Paleta kolorów do stworzenia pliku GIF  */
		0x00, 0x00, 0x00, /* 0: Czarny                               */
		0xFF, 0x00, 0x00, /* 1: Czerwony                             */
		0x00, 0xFF, 0x00, /* 2: Zielony                              */
		0x00, 0x00, 0xFF, /* 3: Niebieski                            */
		0x00, 0xFF, 0xFF, /* 4: Cyjan                                */
		0xFF, 0x00, 0xFF, /* 5: Magenta                              */
		0xFF, 0xFF, 0x00, /* 6: Żółty                                */
		0xFF, 0xFF, 0xFF  /* 7: Biały                                */
	}; int depth = 3;
	ge_GIF* giffile;

	/* Interpretacja wyniku startu programu                          */
	switch (startup(&topleft, &ants, &antscount, &steps, &output, size, &delay)) {
	case 0:
		fprintf(stderr, "Wczytanie konfiguracji przebieglo pomyslnie\n");
		break;
	case 1:
		fprintf(stderr, "Blad otwarcia pliku config.txt\n");
		return -1;
	case 2:
		fprintf(stderr, "Blad alokacji pamieci\n");
		return -2;
	case 3:
		fprintf(stderr, "Blad - mrowka zadeklarowana przed ilosca\n");
		return -3;
	case 4:
		fprintf(stderr, "Nieprawidlowa ilosc mrowek: %d\n", antscount);
		return -4;
	case 5:
		fprintf(stderr, "Blad - Mrowka o numerze wykraczajacym poza zadeklarowany zakres\n");
		return -5;
	case 6:
		fprintf(stderr, "Blad - Niepoprawny symbol co najmniej jednej z mrowek\n");
		return -6;
	case 7:
		fprintf(stderr, "Niewystarczajace dane w pliku konfiguracyjnym\n");
		return -7;
	}

	/* Informacja dla użytkownika o wczytanych ustawieniach          */
	fprintf(stderr, "\nPlansza %d x %d, %llu krokow, %d mrowek, prędkosc %hu ms na klatkę\n", size[0], size[1], steps, antscount, delay * 10);
	for (i = 0; i < antscount; i++) {
		fprintf(stderr, "Mrowka %c, skierowana na %c\n", ants[i].symbol, cells_side_to_char(ants[i].facing));
	}

	if (output == 'f') {
		giffile = ge_new_gif(OUTGIF, size[0], size[1], palette, depth, 1);
		if (!giffile) {
			fprintf(stderr, "Blad utworzenia pliku GIF\n");
			return -8;
		}
	}

	/* Wykonywanie kolejnych iteracji                                */
	for (currstep = 0; currstep < steps; currstep++) {
		for (i = 0; i < antscount; i++) {
			ant_action(ants + i);
		}

		switch (output) {
		case 'c':
			print_cons(topleft, size, delay, ants, antscount);
			break;
		case 'f':
			print_gif(giffile, topleft, size, delay, ants, antscount);
			break;
		}
	}

	if (output == 'f') ge_close_gif(giffile);

	free(ants);

	cells_free(topleft);
	return 0;
}

int startup(cellmap** topleft, ant** ants, int* antscount, unsigned long long int* steps, char* out, uint16_t size[], uint16_t* delay) {
	FILE* configf;
	char currline[CONFLINEMAX], * attval, currsym = 0, facechar = 0;
	int i = 0, currant = 0, currantpos[2];
	unsigned int set = 0;

	configf = fopen(CONFNAME, "r");
	if (!configf) return 1;

	while (fgets(currline, CONFLINEMAX, configf)) {
		if (currline[0] == '#') continue;

		i = 0;
		while (currline[i]) {
			if (currline[i] == '=') {
				currline[i] = '\0';
				attval = &(currline[i + 1]);
			} else if (currline[i] == ',')
				currline[i] = ' ';
			i++;
		}

		if (!strncmp(currline, "output", 6)) {
			sscanf(attval, "%c", out);
			set |= 1;
		} else if (!strncmp(currline, "size", 4)) {
			sscanf(attval, "%hu %hu", size, size + 1);
			if (cells_set(topleft, size[1], size[0])) return 2;
			set |= 2;
		} else if (!strncmp(currline, "steps", 5)) {
			sscanf(attval, "%llu", steps);
			set |= 4;
		} else if (!strncmp(currline, "delay", 5)) {
			sscanf(attval, "%hu", delay);
			set |= 8;
		} else if (!strncmp(currline, "ants", 4)) {
			sscanf(attval, "%d", antscount);
			if (*antscount > MAXANTS || *antscount < 1) return 4;
			*ants = (ant*)calloc(*antscount, sizeof(ant));
			if (!(*ants)) return 2;
			set |= 16;

		} else if (!strncmp(currline, "inipos", 6)) {
			if (!(*ants)) return 2;
			if (!(*topleft)) return 3;

			currant = currline[6] - '1';
			if (currant >= *antscount) return 5;

			sscanf(attval, "%d %d", currantpos, currantpos + 1);

			(*ants + currant)->position = cells_get_cell(*topleft, *currantpos, *(currantpos + 1));

			set |= 32;

		} else if (!strncmp(currline, "symbol", 6)) {
			if (!(*ants)) return 2;
			if (!(*topleft)) return 3;

			currant = currline[6] - '1';
			if (currant >= *antscount) return 5;

			sscanf(attval, "%c", &currsym);

			if (currsym < '!' || currsym>'~' || currsym == '@') return 6;

			(*ants + currant)->symbol = currsym;

			set |= 64;

		} else if (!strncmp(currline, "face", 4)) {
			if (!(*ants)) return 2;
			if (!(*topleft)) return 3;

			currant = currline[4] - '1';
			if (currant >= *antscount) return 5;

			sscanf(attval, "%c", &facechar);

			(*ants + currant)->facing = cells_char_to_side(facechar);
			set |= 128;
		} else if (!strncmp(currline, "color", 5)) {
			if (!(*ants)) return 2;
			if (!(*topleft)) return 3;

			currant = currline[5] - '1';
			if (currant >= *antscount) return 5;

			(*ants + currant)->antcolor = cells_str_to_color(attval);
			set |= 256;
		}
	}

	if (set != 511) return 7;

	return 0;
}

void print_cons(cellmap* topleft, uint16_t size[], int delay, ant* ants, int antscount) {
	cellmap* tmp1 = 0, * tmp2 = 0;
	uint16_t w = 0, h = 0;
	clock_t startt;
	clock_t delaysec;

	system("clear");
	tmp1 = topleft;
	for (h = 0; h < size[1]; h++) {
		tmp2 = tmp1;
		for (w = 0; w < size[0]; w++) {
			if (ant_on_cell(tmp2, ants, antscount)) fprintf(stdout, "@");
			else if (tmp2->state) fprintf(stdout, "%c", tmp2->state);
			else fprintf(stdout, " ");

			tmp2 = tmp2->next[E];
		}
		fprintf(stdout, "\n");

		tmp1 = tmp1->next[S];
	}

	startt = clock();
	delaysec = (clock_t)((double)delay / 100 * CLOCKS_PER_SEC);
	while (clock() < startt + delaysec);

	return;
}

void print_gif(ge_GIF* giffile, cellmap* topleft, uint16_t size[], int delay, ant* ants, int antscount) {
	cellmap* tmp1 = topleft, * tmp2 = 0;
	uint16_t w = 0, h = 0;

	for (h = 0; h < size[1]; h++) {
		tmp2 = tmp1;
		for (w = 0; w < size[0]; w++) {
			if (ant_on_cell(tmp2, ants, antscount)) *(giffile->frame + h * size[0] + w) = WHITE;
			else *(giffile->frame + h * size[0] + w) = tmp2->cellcolor;
			tmp2 = tmp2->next[E];
		}
		tmp1 = tmp1->next[S];
	}

	ge_add_frame(giffile, delay);

	return;
}

void ant_action(ant* currant) {
	if (currant->position->state) {
		currant->position->state = 0;
		currant->position->cellcolor = BLACK;
		currant->facing = (currant->facing + 1) % SIDES;
	} else {
		currant->position->state = currant->symbol;
		currant->position->cellcolor = currant->antcolor;
		currant->facing = (SIDES + (currant->facing - 1)) % SIDES;
	}
	currant->position = currant->position->next[currant->facing];

	return;
}

int ant_on_cell(cellmap* cell, ant* ants, int antcount) {
	int i = 0;
	for (i = 0; i < antcount; i++) {
		if ((ants + i)->position == cell) return 1;
	}
	return 0;
}