//-------------------------------------------------------------------
// APL 2.0 by Richard Kerber
// Bibilotheksnummer: s72817
// 
// Projekt: Ein interaktives Banksystem
// Bei Github: https://github.com/s72817/HTW/tree/master/APL/APL
//
// funktionen.h
//-------------------------------------------------------------------

#define MAXHASH 10

/// <summary>
/// Boolsche Variable Verf�gbar machen
/// </summary>
typedef int bool;
enum { false, true };

/// <summary>
/// Generiert eine 10-stellige Kontonummer.
/// 1.u.2. Ziffer = Bankcode, 3.-9.Ziffer = Kundencode,
/// 10.Ziffer = Pr�fziffer (vgl. ISBN)
/// </summary>
/// <param name="Arr">The arr.</param>
/// <param name="counter">The counter.</param>
long long int get_kontonummer(int counter) {
	
	long long int KN = 2400000000;
	return KN + counter;
	
	////1. und 2. Stellle = Bankcode
	//Arr[0] = 2; Arr[1] = 4;
	//for (int i = 2; i < KN_len; i++)
	//	Arr[i] = 0;

	//long long int sum = arr_to_num(Arr, KN_len);
	//sum += counter;

	////long long int counter = arr_to_num(Arr, KN_len);
	//num_to_arr(Arr, KN_len, sum);

	////1. und 2. Stellle = Bankcode
	//Arr[0] = 2; Arr[1] = 4;
	//do {
	//	//3.-9. Ziffer = Kundencode
	//	for (int i = 2; i < 9; i++)
	//		Arr[i] = rand() % 10;
	//	//Algorithmus f�r 10.Ziffer = Pr�fziffer (vgl. ISBN)
	//	long sum = 0;
	//	for (int i = 0; i < 9; i++)
	//		sum += ((i + 1)*Arr[i]);
	//	Arr[9] = sum % 11;
	//} while (Arr[9] >= 10); //als Pr�fziffer darf keine Zahl gr��er 9 herauskommen
}

/// <summary>
/// Pr�ft die angegebene Kontonummer inkl. Pr�fziffer
/// </summary>
/// <param name="acc_no">Kontonummer als int Array</param>
/// <returns>0 = False, 1 = True</returns>
int check_acc_no(int acc_no[]) {
	int sum = 0;
	for (int i = 0; i < 9; i++)
		sum += (i + 1)*acc_no[i];
	if ((sum % 11) == acc_no[9]) return 1;
	else return 0;
}

/// <summary>
/// Generiert eine 4-stellige PIN zw. 1000 und 9999
/// </summary>
/// <returns>gibt eine beliebige 4 stellige Zahl aus</returns>
int GetRandPIN(int KundeID) {
	int min = 1000, max = 9999;
	return (rand() % (max - min + 1)) + min;
}

/// <summary>
/// Formatierter Ausgabetext
/// </summary>
/// <param name="text">String.</param>
void Status(char *text) {
	printf("\n-------------------------------\n");
	printf("%s\n", text);
	printf("-------------------------------\n\n");
}

