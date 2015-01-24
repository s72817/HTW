//-------------------------------------------------------------------
// APL 2.0 by Richard Kerber
// Bibilotheksnummer: s72817
// 
// Projekt: Ein interaktives Banksystem
// Bei Github: https://github.com/s72817/HTW/tree/master/APL/APL
//
// kunden.h
//-------------------------------------------------------------------

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
/// einzelnen Kunden mit Kundendaten ausgeben
/// </summary>
/// <param name="me">Me.</param>
void kunde_show(kunde *me) {
	//Ausgabe
	printf("\n----------------------------\n");
	printf("KundenID......: %d\n", me->ID);
	printf("Kontonummer...: %lld", me->Kontonummer);
	printf("\nGuthaben......: %.2lf Euro\n", me->Guthaben);
	printf("Pin...........: %d\n", me->PIN);
	printf("----------------------------\n\n");
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
/// Initialisiert und fügt einen Kunden der Liste hinzu.
/// </summary>
/// <param name="konto">Zeiger auf Konto-Liste</param>
/// <param name="counter">Zähler als int</param>
/// <returns>angelegten Kunde</returns>
kunde *kunde_hinzufuegen(kunde **hashtable, int counter) {
	long long int Kontonummer = get_kontonummer(counter);

	//Hash Wert generienen
	int hashaddress = hash_function(Kontonummer); //verbesserungswürdig
	/* Zeiger auf errechnete Tabellenadresse durch hash_funktion */
	kunde *konto = hashtable[hashaddress];

	/* Speicher für neues Element allozieren */
	if ((konto = (kunde*)malloc(sizeof(kunde))) == NULL) {
		Status("Kein Speicher für neues Element vorhanden\n");
		return NULL;
	}

	//Struct befüllen
	konto->ID = counter;
	konto->Kontonummer = Kontonummer;
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
	while (konto != NULL) {
		if (konto->Kontonummer == Kontonummer)
			return  konto;
		konto = konto->next;
	}
	return NULL;
}

/// <summary>
/// Kunde_anmelden
/// </summary>
/// <param name="hashtabelle">The hashtabelle.</param>
/// <param name="me">Me.</param>
/// <param name="eingeloggt">The eingeloggt.</param>
kunde *kunde_anmelden(kunde **hashtabelle, kunde *me, bool *eingeloggt) {
	long long int tmp_KN = 0; //Temporäre Variablen für Kontonummer
	int tmp_PIN = 0; // und PIN anlegen
	int versuche = 0; //PIN-Versuche initialisieren
	while (1) {
		printf("Kontonummer: ");
		scanf_s("%lld", &tmp_KN); //tmp_Kontonummer mit Benutzereingabe füllen
		while (getchar() != '\n');
		if (tmp_KN >= 1000000000 && tmp_KN <= 9999999999) break; //Wenn Kontonummer zwischen 1000000000 und 9999999999 liegt, dann weiter
		else Status("Bitte geben Sie Ihre 10-stellige Kontonummer an!");
	}

	if ((me = kunde_suche(hashtabelle, tmp_KN)) != NULL) {
		//Abfrage, ob blockiert
		if (me->blockiert == true) {
			printf("\n\n----------------------------\n");
			printf("Die Kontonummer %lld ist blockiert.\nBitte wenden Sie sich an den naechsten\nfreien Service-Mitarbeiter.\n", me->Kontonummer);
			printf("----------------------------\n");
			return NULL;
		}
		while (1) { //zaehler <= 2
			//abfrage nach PIN
			while (1) {
				printf("PIN: ");
				scanf_s("%d", &tmp_PIN);
				while (getchar() != '\n');
				if (tmp_PIN >= 1000 && tmp_PIN <= 9999) break;
				else Status("Bitte geben Sie Ihre 4-stellige PIN an!");
			}
			if (kunde_pruefen(me, tmp_KN, tmp_PIN) == 1) { //kunden[SIDKunde].PIN) == 1)
				Status("Sie haben sich erfolgreich angemeldet!");
				*eingeloggt = true;
				return me;
			}
			else {
				versuche += 1; //Pin Eingabemöglichkeit wird um eins hoch gesetzt
				//Prüfen, ob 3x hintereinander die falsche PIN eingegeben wurde!
				if (versuche == 3) {
					Status("Sie haben 3x hintereinander\ndie falsche PIN eingegeben!\nSie sind nun blockiert!");
					me->blockiert = true; //Kunde wird hiermit blockiert
					break;
				}
			}
			// <3, weil bis 2 gezählt wird
			if (versuche < 3) printf("Bitte wiederholen (%d. Versuch)\n", versuche + 1);
		}
	}
	else error_kontonummer();
	return NULL;
}

/// <summary>
/// Kunde abmelden - Kunde wird auf NULL gesetzt
/// </summary>
/// <param name="me">Me.</param>
/// <param name="eingeloggt">The eingeloggt.</param>
/// <returns></returns>
kunde *kunde_abmelden(kunde *me, bool *eingeloggt) {
	Status("Sie haben sich erfolgreich abgemeldet.");
	*eingeloggt = false;
	return NULL;
}

/// <summary>
/// Kunde_pin_aendern
/// </summary>
/// <param name="me">Me.</param>
void kunde_pin_aendern(kunde *me) {
	int tmp_oldPIN, tmp_newPIN = 0;

	Status("4-stellige Pin aendern");

	//Alte PIN einlesen
	printf("Bitte geben Sie ihre aktuelle PIN ein:\n");
	while (1) {
		scanf_s("%d", &tmp_oldPIN);
		while (getchar() != '\n');
		if (tmp_oldPIN >= 1000 && tmp_oldPIN <= 9999 && tmp_oldPIN == me->PIN) break;
		else printf("falsche Eingabe! Bitte wiederholen\n");
	}

	//neue PIN einlesen
	printf("Bitte geben Sie eine neue 4-stellige PIN ein:\n");
	while (1) {
		scanf_s("%d", &tmp_newPIN);
		while (getchar() != '\n');
		if (tmp_newPIN >= 1000 && tmp_newPIN <= 9999) break;
		else if (tmp_newPIN < 1000) printf("Bitte geben Sie eine Pin groesser gleich 1000 an!\n");
		else printf("falsche Eingabe! Bitte wiederholen\n");
	}

	if (me->PIN == tmp_oldPIN) {
		me->PIN = tmp_newPIN;

		printf("\n\n----------------------------\n");
		printf("Ihre PIN wurde erfolgreich auf %d geaendert.\n", tmp_newPIN);
		printf("----------------------------\n\n");

	}
	else {
		Status("Sie haben eine falsche PIN eingegeben!");
	}
}

/// <summary>
/// Löscht einen Kunden unter Angabe seiner Kontonummer
/// </summary>
/// <param name="hashtabelle">Hashtabelle</param>
/// <param name="me">Me.</param>
/// <param name="eingeloggt">The eingeloggt.</param>
void kunde_loeschen(kunde **hashtabelle, kunde *me, bool *eingeloggt) {
	printf("Wollen Sie ihre Bank-Beziehung wirklich beenden?\n");
	printf("(J)a oder (N)ein\n");
	char res = (char)getche();
	printf("\n");

	if (res == 'j' || res == 'J') {
		Status("Ihr Verhaeltnis ist nun beendet!");
		if (me->Guthaben > 0.0) printf("Es werden Ihnen nun noch %.2lf Euro augezahlt.\n\n", me->Guthaben);
		*eingeloggt = false;

		//eigentliches Löschen
		kunde *zeiger, *zeiger1; //Hilfsstrukturen
		int hashaddress = hash_function(me->Kontonummer); //Hashadresse berechnen
		kunde *konto = hashtabelle[hashaddress]; //Liste mit entsprechenden Elementen füllen

		//Wenn erstes Element
		if (konto != NULL) {
			if (konto->Kontonummer == me->Kontonummer) { //1.Element
				zeiger = konto->next; //Hilfsstruktur wird mit dem nächsten Zeiger befüllt
				free(konto); //Element löschen
				hashtabelle[hashaddress] = zeiger; //
			}
			else {
				zeiger = konto;
				while (zeiger->next != NULL) {
					zeiger1 = zeiger->next;
					if (zeiger1->Kontonummer == me->Kontonummer) {
						zeiger->next = zeiger1->next;
						free(zeiger1);
						hashtabelle[hashaddress] = konto;
						break;
					}
					zeiger = zeiger1;
				}/* Ende while */
			}/* Ende else */
		}/* Ende if(anfang != NULL) */
		else
			Status("Es sind keine Daten zum Loeschen vorhanden!");
	} //END IF
	else { 
		Status("Keine Option gewaehlt!");
		return; //Abbrechen
	}
}

/// <summary>
/// Einzahlung
/// </summary>
/// <param name="me">Me.</param>
void kunde_einzahlung(kunde *me) {
	double Betrag = 0.0;

	Status("Einzahlung");
	printf("Bitte geben Sie den Betrag ein, welcher eingezahlt werden soll:\n");
	while (1) {
		printf("Betrag:");
		scanf_s("%lf", &Betrag);
		while (getchar() != '\n');
		if (Betrag > 0.0) break;
		else if (Betrag <= 0) return; //printf("Bitte geben Sie mehr als 0 Euro ein!\n");
		else printf("falsche Eingabe! Bitte wiederholen\n");
	}

	me->Guthaben += Betrag;
	printf("\n\n----------------------------\n");
	printf("Sie haben erfolgreich %.2lf Euro eingezahlt.\n", Betrag);
	printf("----------------------------\n\n");

	//kunde_show(me);
}

/// <summary>
/// Auszahlung
/// </summary>
/// <param name="me">Me.</param>
void kunde_auszahlung(kunde *me) {
	double tmp_Guthaben = me->Guthaben;
	double Betrag = 0.0;

	Status("Auszahlung");
	printf("Bitte geben Sie den Betrag ein, welcher ausgezahlt werden soll:\n(Es koennen max. %.2lf Euro ausgezahlt werden)\n", me->Guthaben);
	while (1) {
		printf("Betrag:");
		scanf_s("%lf", &Betrag);
		while (getchar() != '\n');
		if (Betrag > 0.0 && Betrag <= me->Guthaben) break;
		else if (Betrag <= 0) return; //printf("Bitte geben Sie mehr als 0 Euro ein!\n");
		else printf("falsche Eingabe! Bitte wiederholen\n");
	}

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

	//kunde_show(me);
}

/// <summary>
/// Überweisung
/// </summary>
/// <param name="hashtable">The hashtable.</param>
/// <param name="me">Me.</param>
void kunde_ueberweisung(kunde **hashtable, kunde *me) {
	kunde *kunde2 = NULL;
	double tmp_Guthaben = me->Guthaben;
	long long int zuKontonummer = 0;
	double Betrag = 0.0;
	
	Status("Ueberweisung");
	
	while (1) {
		printf("Kontonummer des Empfaengers:");
		scanf_s("%lld", &zuKontonummer);
		while (getchar() != '\n');
		if (zuKontonummer >= 1000000000 && zuKontonummer <= 9999999999 && zuKontonummer != me->Kontonummer) break; //Eingabe der eigenen Kontonummer könnte schöner abgefangen werden
		else printf("falsche Eingabe! Bitte wiederholen\n");
	}

	while (1) {
		printf("Betrag:");
		scanf_s("%lf", &Betrag);
		while (getchar() != '\n');
		if (Betrag <= 0) return;
		else if (tmp_Guthaben - Betrag < 0) {
			printf("\n\n----------------------------\n");
			printf("Der Betrag von %.2lf Euro kann nicht ueberwiesen werden,\nda sonst ihr Konto ueberzogen wird.\nSie koennen maximal %.2lf Euro Ueberweisen!\n", Betrag, me->Guthaben);
			printf("----------------------------\n\n");
			//return;
		}
		else break;
	}


	if (kunde_suche(hashtable, zuKontonummer) != NULL) {//if (kunde_suchen(all, &Kunde2, zu_Kontonummer) == 1) {
		kunde2 = kunde_suche(hashtable, zuKontonummer);
		me->Guthaben -= Betrag; //Den eigenen Kontostand minimieren
		kunde2->Guthaben += Betrag; //Den Betrag dem anderen Konto gut schreiben
		printf("\n\n----------------------------\n");
		printf("Sie haben erfolgreich %.2lf Euro\nan das Konto %lld ueberwiesen\n", Betrag, zuKontonummer);
		printf("----------------------------\n\n");
	}
	else {
		error_kontonummer_para(zuKontonummer);
	}

	//kunde_show(me);
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
	while (konto != NULL) {
		printf("Kontonummer: %lld Hash: %d\n", konto->Kontonummer, hash_function(konto->Kontonummer));
		konto = konto->next;
	}
}