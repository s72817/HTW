//Funktionen.h by Richard Kerber

void num_to_arr(int arr[], int len, long long int num);
long long int arr_to_num(int *arr, int len);
int digits(long long int x);
void get_acc_no(int *Arr);
int check_acc_no(int acc_no[]);

/// <summary>
/// Long Long Integer -> int Array
/// </summary>
/// <param name="arr">int Array</param>
/// <param name="len">Länge des Arrays</param>
/// <param name="num">Zahl</param>
void num_to_arr(int arr[], int len, long long int num) {
	for (int i = 10 - 1; i >= 0; --i) {
		arr[i] = num % 10;
		num /= 10;
	}
}

/// <summary>
/// Integer Array -> Long Long Integer
/// </summary>
/// <param name="arr">int Array</param>
/// <param name="len">Länge des Arrays</param>
/// <returns>long long Integer</returns>
long long int arr_to_num(int *arr, int len) {
	long long int k = 0;
	for (int i = 0; i < len; i++)
		k = 10 * k + arr[i];
	return k;
}

//TODO!
/// <summary>
/// Zählt Ziffern eines Long Long Integer
/// </summary>
/// <param name="x">Zahl</param>
/// <returns>Anzahl der Ziffern</returns>
//int digits(long long int x) { return log10(x) + 1; }
int digits(long long int x) {
	int count = 0;
	while (x != 0)
	{
		x /= 10;             /* n=n/10 */
		++count;
	}
	return count;
}

/// <summary>
/// Generiert eine 10-stellige Kontonummer. 
/// 1.u.2. Ziffer = Bankcode, 3.-9.Ziffer = Kundencode,
/// 10.Ziffer = Prüfziffer (vgl. ISBN)
/// </summary>
/// <param name="Arr">The arr.</param>
void get_acc_no(int *Arr) {
	//1. und 2. Stellle = Bankcode
	Arr[0] = 2; Arr[1] = 4;
	do {
		//3.-9. Ziffer = Kundencode
		for (int i = 2; i < 9; i++)
			Arr[i] = rand() % 10;
		//Algorithmus für 10.Ziffer = Prüfziffer (vgl. ISBN)
		long sum = 0;
		for (int i = 0; i < 9; i++)
			sum += ((i + 1)*Arr[i]);
		Arr[9] = sum % 11;
	} while (Arr[9] >= 10); //als Prüfziffer darf keine Zahl größer 9 herauskommen
}

/// <summary>
/// Prüft die angegebene Kontonummer inkl. Prüfziffer
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