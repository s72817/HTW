//error.h by Richard Kerber

#include <stdio.h>


/// <summary>
/// Error - Kontonummer nicht gefunden
/// </summary>
void error_kontonummer() {
	Status("Kontonummer nicht gefunden!");
}

void error_kontonummer_parax(long long int kontonummer) {
	printf("\n\n----------------------------\n");
	printf("Die Kontonummer: %lld konnte nicht gefunden werden!\n", kontonummer);
	printf("----------------------------\n\n");
}
