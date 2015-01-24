//-------------------------------------------------------------------
// APL 2.0 by Richard Kerber
// Bibilotheksnummer: s72817
// 
// Projekt: Ein interaktives Banksystem
// Bei Github: https://github.com/s72817/HTW/tree/master/APL/APL
//
// error.h
//-------------------------------------------------------------------

/// <summary>
/// Error - Kontonummer nicht gefunden
/// </summary>
void error_kontonummer() {
	Status("Kontonummer nicht gefunden!");
}

/// <summary>
/// Error - Kontonummer nicht gefunden mit 1 Parameter
/// </summary>
/// <param name="kontonummer">The kontonummer.</param>
void error_kontonummer_para(long long int kontonummer) {
	printf("\n\n----------------------------\n");
	printf("Die Kontonummer: %lld konnte nicht gefunden werden!\n", kontonummer);
	printf("----------------------------\n\n");
}
