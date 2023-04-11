#include <stdlib.h>
#include <string.h>
#include "PF_cells.h"

int cells_set(cellmap** topleft, int height, int width) {
	int x = 0, y = 0;
	cellmap* tmp1, * tmp2;
	if (!(height && width)) return -1;

	/* Alokacja pamięci                                        */
	*topleft = (cellmap*)calloc(1, sizeof(cellmap));
	if (!topleft) return -1;

	tmp1 = *topleft;
	for (y = 1; y < height; y++) {
		tmp1->next[S] = (cellmap*)calloc(1, sizeof(cellmap));
		if (!tmp1) return -1;

		tmp2 = tmp1;
		for (x = 1; x < width; x++) {
			tmp2->next[E] = (cellmap*)calloc(1, sizeof(cellmap));
			if (!tmp2) return -1;

			tmp2->next[E]->next[W] = tmp2;
			tmp2 = tmp2->next[E];
		}
		tmp2->next[E] = tmp1;
		tmp1->next[W] = tmp2;

		tmp1->next[S]->next[N] = tmp1;
		tmp1 = tmp1->next[S];
	}
	tmp2 = tmp1;
	for (x = 1; x < width; x++) {
		tmp2->next[E] = (cellmap*)calloc(1, sizeof(cellmap));
		if (!tmp2) return -1;

		tmp2->next[E]->next[W] = tmp2;
		tmp2 = tmp2->next[E];
	}
	tmp2->next[E] = tmp1;
	tmp1->next[W] = tmp2;

	tmp1->next[S] = *topleft;
	(*topleft)->next[N] = tmp1;

	/* Dodanie brakujących połączeń                            */
	tmp1 = *topleft; tmp2 = (*topleft)->next[S];
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			tmp1->next[S] = tmp2; tmp2->next[N] = tmp1;
			tmp1 = tmp1->next[E]; tmp2 = tmp2->next[E];
		}
		tmp1 = tmp2; tmp2 = tmp2->next[S];
	}



	return 0;
}

cellmap* cells_get_cell(cellmap* topleft, int x, int y) {
	cellmap* tmp = topleft;
	while (x--) {
		tmp = tmp->next[E];
	}
	while (y--) {
		tmp = tmp->next[S];
	}
	return tmp;
}

void cells_free(cellmap* topleft) {
	while (topleft->next[S]) {
		topleft->next[W]->next[E] = 0;
		while (topleft->next[E]) {
			if (topleft->next[N]) topleft->next[N]->next[S] = 0;
			if (topleft->next[S]) topleft->next[S]->next[N] = 0;

			topleft = topleft->next[E];
			free(topleft->next[W]);
		}
		if (topleft->next[N]) topleft->next[N]->next[S] = 0;

		topleft = topleft->next[S];
		free(topleft->next[N]);
	}
	topleft->next[W]->next[E] = 0;
	while (topleft->next[E]) {
		topleft = topleft->next[E];
		free(topleft->next[W]);
	}
	free(topleft);
	return;
}

side cells_char_to_side(char sd) {
	switch (sd) {
	case 'N':
		return N;
	case 'E':
		return E;
	case 'S':
		return S;
	case 'W':
		return W;
	default:
		return N;
	}
}

char cells_side_to_char(side sd) {
	switch (sd) {
	case N:
		return 'N';
	case E:
		return 'E';
	case S:
		return 'S';
	case W:
		return 'W';
	default:
		return 'X';
	}
}

color cells_str_to_color(char* str) {
	if (!strncmp(str, "red", 3)) {
		return RED;
	} else if (!strncmp(str, "green", 5)) {
		return GREEN;
	} else if (!strncmp(str, "blue", 4)) {
		return BLUE;
	} else if (!strncmp(str, "cyan", 4)) {
		return CYAN;
	} else if (!strncmp(str, "magenta", 7)) {
		return MAGENTA;
	} else if (!strncmp(str, "yellow", 6)) {
		return YELLOW;
	}
	return BLACK;
}