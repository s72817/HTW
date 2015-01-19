//kunde.h by Richard Kerber

/// <summary>
/// Kundenstruktur
/// </summary>
typedef struct kunde_t{
	int ID;
	long long int Kontonummer; //10-stellig
	double Guthaben;
	unsigned int PIN;
	bool blockiert;

	struct kunde_t *next;
}kunde;

/// <summary>
/// Die Hashfunktion. 
/// </summary>
/// <param name="Kontonummer">Kontonummer als long long int</param>
/// <returns>Gibt einen Wert zwischen 0 und MaxHash zurück.</returns>
int hash_function(long long int Kontonummer) {
	//Für m sollten Sie idealerweise eine Primzahl so nahe wie möglich am höchsten Index wählen.*/
	return Kontonummer % 7;
}

/// <summary>
/// Initialisiert und fügt einen Kunden der Liste hinzu.
/// </summary>
/// <param name="konto">Zeiger auf Konto-Liste</param>
/// <param name="counter">Zähler als int</param>
/// <returns>angelegten Kunde</returns>
kunde *kunde_add(kunde **hashtable, int counter) {
	long long int Kontonummer = get_kontonummer(counter);

	//Hash Wert generienen
	int hashaddress = hash_function(Kontonummer); //verbesserungswürdig
	/* Zeiger auf errechnete Tabellenadresse durch hash_funktion */
	kunde *konto = hashtable[hashaddress];

	/* Speicher für neues Element allozieren */
	konto = (kunde*)malloc(sizeof(kunde));
	if (konto == NULL) {
		Status("Kein Speicher für neues Element vorhanden\n");
		return NULL;
	}
	//Struct befüllen

	konto->ID = counter;
	konto->Kontonummer = Kontonummer;
	/*for (int i = 0; i < KN_len; i++)
	konto->Kontonummer[i] = KN_Arr[i];*/
	konto->Guthaben = 0.0;
	konto->PIN = GetRandPIN(counter);
	konto->blockiert = 0;
	//Zeiger vom nächsten Element bekommt errechnete Hashadresse
	konto->next = hashtable[hashaddress];
	//Wert zum Hash(Array) hinzufüen
	hashtable[hashaddress] = konto;

	return konto;
}

/// <summary>
/// Suche nach Listenelement mit kontonummer
/// </summary>
/// <param name="konto">kundenliste</param>
/// <param name="Kontonummer">Kontonummer als long long</param>
/// <returns>struct kunde</returns>
kunde *kunde_suche(kunde **hashtable, long long int Kontonummer) {
	int hashaddress = hash_function(Kontonummer);
	kunde *konto = hashtable[hashaddress];
	//kunde *me = NULL;
	while (konto != NULL) {
		if (konto->Kontonummer == Kontonummer)
			return  konto;
		konto = konto->next;
	}
	return NULL;
}

/// <summary>
/// Löscht einen Kunden unter Angabe seiner Kontonummer
/// </summary>
/// <param name="konto">Zeiger auf Konto</param>
/// <param name="hashtabelle">Hashtabelle</param>
/// <param name="KN">Kontonummer</param>
void kunde_loeschen(kunde **hashtable, long long int Kontonummer) {
	kunde *zeiger, *zeiger1; //Hilfsstrukturen
	int hashaddress = hash_function(Kontonummer); //Hashadresse berechnen
	kunde *konto = hashtable[hashaddress]; //Liste mit entsprechenden Elementen füllen

	//Wenn erstes Element
	if (konto != NULL) {
		if (konto->Kontonummer == Kontonummer) { //1.Element
			zeiger = konto->next; //Hilfsstruktur wird mit dem nächsten Zeiger befüllt
			free(konto); //Element löschen
			hashtable[hashaddress] = zeiger; //
		}
		else {
			zeiger = konto;
			while (zeiger->next != NULL) {
				zeiger1 = zeiger->next;
				if (zeiger1->Kontonummer == Kontonummer) {
					zeiger->next = zeiger1->next;
					free(zeiger1);
					hashtable[hashaddress] = konto;
					break;
				}
				zeiger = zeiger1;
			}/* Ende while */
		}/* Ende else */
	}/* Ende if(anfang != NULL) */
	else
		Status("Es sind keine Daten zum Loeschen vorhanden!");
}

/// <summary>
/// Einzahlung
/// </summary>
/// <param name="hashtable">The hashtable.</param>
/// <param name="me">Me.</param>
/// <param name="Betrag">The betrag.</param>
void kunde_einzahlung(kunde **hashtable, kunde *me, double Betrag) {
	me->Guthaben += Betrag;
	printf("\n\n----------------------------\n");
	printf("Sie haben erfolgreich %.2lf Euro eingezahlt.\n", Betrag);
	printf("----------------------------\n\n");
}

/// <summary>
/// Auszahlung
/// </summary>
/// <param name="hashtable">The hashtable.</param>
/// <param name="me">Me.</param>
/// <param name="Betrag">The betrag.</param>
void kunde_auszahlung(kunde **hashtable, kunde *me, double Betrag) {
	double tmp_Guthaben = me->Guthaben;
	//Verhindert, dass Konto überzogen werden kann
	if ((tmp_Guthaben - Betrag) < 0) {
		printf("\n\n----------------------------\n");
		printf("Der Betrag von %.2lf Euro kann nicht ausgezahlt werden,\nda sonst ihr Konto ueberzogen wird!\n", Betrag);
		printf("----------------------------\n\n");
		return;
	}
	me->Guthaben -= Betrag;
	printf("\n\n----------------------------\n");
	printf("Sie haben erfolgreich %.2lf Euro abgehoben.\n", Betrag);
	printf("----------------------------\n\n");
}

/// <summary>
/// Überweisung
/// </summary>
/// <param name="hashtable">The hashtable.</param>
/// <param name="me">Me.</param>
/// <param name="Betrag">The betrag.</param>
/// <param name="zu_Kontonummer">The zu_ kontonummer.</param>
void kunde_ueberweisung(kunde **hashtable, kunde *me, double Betrag, long long int zu_Kontonummer) {
	kunde *kunde2;
	double tmp_Guthaben = me->Guthaben;
	//Verhindert, dass das Konto überzogen werden kann
	if (tmp_Guthaben - Betrag < 0) {
		printf("\n\n----------------------------\n");
		printf("Der Betrag von %.2lf Euro kann nicht ueberwiesen werden,\nda sonst ihr Konto ueberzogen wird.\nSie koennen maximal %.2lf Euro Ueberweisen!\n", Betrag, me->Guthaben);
		printf("----------------------------\n\n");
		return;
	}
	if (kunde_suche(hashtable, zu_Kontonummer) != NULL) {//if (kunde_suchen(all, &Kunde2, zu_Kontonummer) == 1) {
		kunde2 = kunde_suche(hashtable, zu_Kontonummer);
		me->Guthaben -= Betrag; //Den eigenen Kontostand minimieren
		kunde2->Guthaben += Betrag; //Den Betrag dem anderen Konto gut schreiben
		printf("\n\n----------------------------\n");
		printf("Sie haben erfolgreich %.2lf Euro\nan das Konto %lld ueberwiesen\n", Betrag, zu_Kontonummer);
		printf("----------------------------\n\n");
	}
	else {
		error_kontonummer_parax(zu_Kontonummer);
	}
}

/// <summary>
/// Prüfen der Kontonummer & PIN
/// </summary>
/// <param name="me">Me.</param>
/// <param name="Kontonummer">The kontonummer.</param>
/// <param name="PIN">The pin.</param>
/// <returns>0 oder 1, False oder True</returns>
bool kunde_pruefen(kunde *me, long long int Kontonummer, int PIN) {
	return ((me->Kontonummer == Kontonummer) && (me->PIN == PIN));
}

/// <summary>
/// Alle Listenelemente der Hashtabelle werden ausgegeben
/// </summary>
/// <param name="hashtable">The hashtable.</param>
void kunde_output_all(kunde **hashtable) {
	for (int i = 0; i < MAXHASH; i++){
		kunde *konto = hashtable[i];
		while (konto != NULL) {
			printf("Kontonummer: %lld Hash: %d\n", konto->Kontonummer, hash_function(konto->Kontonummer));
			konto = konto->next;
		}
	}
}

/// <summary>
/// gibt die Entsprechenden Listenelemente mit demselben Hashwert aus
/// </summary>
/// <param name="hashtable">The hashtable.</param>
/// <param name="KN">The kn.</param>
void kunde_output_byhash(kunde **hashtable, long long int KN) {
	int hashaddress = hash_function(KN);
	kunde *konto = hashtable[hashaddress];

	//pointer = hashtabelle[hashaddress];
	while (konto != NULL) {
		printf("Kontonummer: %lld Hash: %d\n", konto->Kontonummer, hash_function(konto->Kontonummer));
		konto = konto->next;
	}
}

/// <summary>
/// einzelnen Kunden mit Kundendaten ausgeben
/// </summary>
/// <param name="me">Me.</param>
void kunde_show(kunde *me) {
	//Ausgabe
	printf("\n----------------------------\n");
	//printf("Sie haben erfolgreich einen neuen Kunden angelegt!\n");
	printf("KundenID......: %d\n", me->ID);
	printf("Kontonummer...: %lld", me->Kontonummer);
	printf("\nGuthaben......: %.2lf Euro\n", me->Guthaben);
	printf("Pin...........: %d\n", me->PIN);
	printf("----------------------------\n\n");
}