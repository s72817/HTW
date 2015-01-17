#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Eigene Dateien
#include "menu.h"
#include "funktionen.h"
#include "error.h"

#define MAXHASH 10
#define BANKCODE 24

/// <summary>
/// Boolsche Variable Verfügbar machen
/// </summary>
typedef int bool;
enum { false, true };

/// <summary>
/// Kundenstruktur
/// </summary>
typedef struct kunde_t{
	int ID;
	long long int Kontonummer; //int Kontonummer[10]; //10-stellig
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
void kunde_delete(kunde **hashtable, long long int Kontonummer) {
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
void kunde_einzahlung(kunde **hashtable,kunde *me, double Betrag) {
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
		error_kontonummer_para(zu_Kontonummer);
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
			printf("Kontonummer: %lld Hash: %d\n",konto->Kontonummer, hash_function(konto->Kontonummer));
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
		printf("Kontonummer: %lld Hash: %d\n",konto->Kontonummer, hash_function(konto->Kontonummer));
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


/// <summary>
/// Hauptroutine
/// </summary>
/// <returns></returns>
int main() {
	menu *mymenu = NULL; // init. die Liste mit NULL = leere liste

	kunde *me = NULL; //Aktuelles Object
	kunde *hashtabelle[MAXHASH] = {NULL}; //Hashtabelle

	int counter = 0; //Kundenzähler
	bool eingeloggt = false; //geloggt
	long long int tmp_KN = 0; //Temporäre Variablen für Kontonummer
	int tmp_PIN = 0; // und PIN anlegen
	int versuche = 0; //PIN-Versuche initialisieren

	//Dynamisches Menu hinzufügen
	//(Zeiger auf Liste, "Elementname", Level)
	menu_add(&mymenu, "Kunde anlegen", 1);
	menu_add(&mymenu, "Anmelden", 1);
	menu_add(&mymenu, "Beenden", 1);
	menu_add(&mymenu, "Einzahlung", 2);
	menu_add(&mymenu, "Auszahlung", 2);
	menu_add(&mymenu, "Ueberweisen", 2);
	menu_add(&mymenu, "Kontostand", 2);
	menu_add(&mymenu, "4-stellige PIN aendern", 2);
	menu_add(&mymenu, "Kundendaten anzeigen", 2);
	menu_add(&mymenu, "Konto loeschen", 2);
	menu_add(&mymenu, "Abmelden", 2);
	menu_add(&mymenu, "Beenden", 2);
	menu_add(&mymenu, "Kunde suchen", 3);
	menu_add(&mymenu, "Kontonummer mit gleichem Hashwert", 3);
	menu_add(&mymenu, "Alle Kunden mit Hash ausgeben", 3);
	menu_add(&mymenu, "zurueck", 3);


	

	//1. Menu
	int auswahl;
	while (1) { //1.Menuebene
		menu_show(mymenu, 1); //1.Menu erzeugen
		printf("\nIhre Wahl:\n");
		scanf_s("%d", &auswahl);
		switch (auswahl) {
		case 1:  //kunde anlegen
			for (int i = 0; i < 10; i++)
				kunde_show(kunde_add(&hashtabelle, counter++));
			break;
		case 2:  //kunde anmelden
			while (1) {
				printf("Kontonummer: ");
				scanf_s("%lld", &tmp_KN); //tmp_Kontonummer mit Benutzereingabe füllen
				while (getchar() != '\n');
				if (tmp_KN >= 1000000000 && tmp_KN <= 9999999999) break; //Wenn Kontonummer zwischen 1000 und 9999 liegt, dann weiter
				else Status("Bitte geben Sie Ihre 10-stellige Kontonummer an!");
			}
			
			if ((me = kunde_suche(&hashtabelle, tmp_KN)) != NULL) {
				//Abfrage, ob blockiert
				if (me->blockiert == 1) {
					printf("\n\n----------------------------\n");
					printf("Die Kontonummer %lld ist blockiert.\nBitte wenden Sie sich an den naechsten\nfreien Service-Mitarbeiter.\n", me->Kontonummer);
					printf("----------------------------\n");
					break;
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
						eingeloggt = true;
						break;
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
			break;
		case 3: return 0;//Beenden
		
		case 55: //Spezial Menu
			do { 
				menu_show(mymenu, 3); //Menu erzeugen
				printf("\nIhre Wahl:\n");
				scanf_s("%d", &auswahl);
				switch (auswahl) {
				case 1:  //kunde suchen
					printf("Kontonummer:");
					scanf_s("%lld", &tmp_KN);
					while (getchar() != '\n');
					if (kunde_suche(&hashtabelle,tmp_KN) != NULL) {
						printf("Kunde gefunden!\n");
						kunde_show(kunde_suche(&hashtabelle,tmp_KN));
					}
					else error_kontonummer();
					break;	
				case 2:  //kunde_output byhash
					printf("Kontonummer:");
					scanf_s("%lld", &tmp_KN);
					while (getchar() != '\n');
					if (kunde_suche(&hashtabelle,tmp_KN) != NULL)
						kunde_output_byhash(&hashtabelle,tmp_KN);
					else error_kontonummer();
					break;
				case 3: //kunde output all
					kunde_output_all(&hashtabelle);
					break;
				case 4: break; //zurück
				
				} 
			} while (auswahl != 4); //wenn x, dann schluss!
			break;
		default:
			Status("Ihrer Eingabe konnte kein Menuepunkt zugeordnet werden!\nBitte versuchen Sie es erneut.");
			break;	
		} //Switch


		while (1) { //2.Menu
			if (eingeloggt == false) break; //Wichtig, da sonst nicht zurück
			menu_show(mymenu, 2); //Menu erzeugen
			printf("\nIhre Wahl:\n");
			scanf_s("%d", &auswahl);
			switch (auswahl) {
			case 1: //Einzahlung
				//Variable muss unbedingt vorher initialisiert werden durch login!
				printf("Einzahlung");
				double tmp_Einzahlung;
				printf("Bitte geben Sie den Betrag ein, welcher eingezahlt werden soll:\n(Sie koennen max. 5000 Euro Einzahlen.)\n");
				while (1) {
					printf("Betrag: ");
					scanf_s("%lf", &tmp_Einzahlung);
					while (getchar() != '\n');
					if (tmp_Einzahlung > 0.00 && tmp_Einzahlung <= 5000.0) break;
					else printf("falsche Eingabe! Bitte wiederholen\n");
				}
				kunde_einzahlung(&hashtabelle, me, tmp_Einzahlung);
				kunde_show(me);
				break;
			case 2: { //Auszahlung
				double tmp_Auszahlung = 0.0;
				printf("Bitte geben Sie den Betrag ein, welcher ausgezahlt werden soll:\n(Es koennen max. %.2lf Euro ausgezahlt werden)\n", me->Guthaben);
				while (1) {
					printf("Betrag:");
					scanf_s("%lf", &tmp_Auszahlung);
					while (getchar() != '\n');
					if (tmp_Auszahlung > 0.0 && tmp_Auszahlung <= me->Guthaben) break;
					else if (tmp_Auszahlung <= 0) printf("Bitte geben Sie mehr als 0 Euro ein!\n");
					else printf("falsche Eingabe! Bitte wiederholen\n");
				}

				//me = kunde_edit(&h,me, tmp_Auszahlung, Auszahlung);
				kunde_auszahlung(&hashtabelle, me, tmp_Auszahlung);
				kunde_show(me);
				//saveDB(k); //Daten speichern
				break;
			}
			case 3: //Überweisung
				printf("Ueberweisung\n");
				long long int tmp_zuKontonummer = 0;
				while (1) {
					printf("Kontonummer:");
					scanf_s("%lld", &tmp_zuKontonummer);
					while (getchar() != '\n');
					if (tmp_zuKontonummer >= 1000000000 && tmp_zuKontonummer <= 9999999999 && tmp_zuKontonummer != me->Kontonummer) break; //Eingabe der eigenen Kontonummer kann schöner abgefangen werden
					else printf("falsche Eingabe! Bitte wiederholen\n");
				}

				double tmp_Ueberweisung = 0.0;
				printf("Betrag:"); //\n(Es koennen max. %.2lf Euro ueberwiesen werden)\n", kunden[SIDKunde].Guthaben);
				while (1) {
					scanf_s("%lf", &tmp_Ueberweisung);
					while (getchar() != '\n');
					if (tmp_Ueberweisung > 0.0) break; //if (tmp_Ueberweisung > 0 && tmp_Ueberweisung <= 1000) break;
					else if (tmp_Ueberweisung <= 0) printf("Bitte geben Sie mehr als 0 Euro ein!\n");
					else printf("falsche Eingabe! Bitte wiederholen\n");
				}
				kunde_ueberweisung(&hashtabelle, me, tmp_Ueberweisung, tmp_zuKontonummer);
				break;
			case 4:	 //Kontostand
				printf("\n\n----------------------------\n");
				printf("Ihr Guthaben betraegt: %.2lf Euro\n", me->Guthaben);
				printf("----------------------------\n");
				break;
			case 5:  //4 - stellige PIN aendern
				printf("4-stellige Pin aendern\n");
				int tmp_oldPIN, tmp_newPIN;
				//Alte PIN einlesen
				printf("Bitte geben Sie ihre aktuelle PIN ein:\n");
				while (1) {
					scanf_s("%d", &tmp_oldPIN);
					while (getchar() != '\n');
					if (tmp_oldPIN >= 1000 && tmp_oldPIN <= 9999) break;
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

				//Prüfen der PIN und der KONTONUMMER AUSLAGERN!!!
				if (me->PIN == tmp_oldPIN) {
					me->PIN = tmp_newPIN;
					//saveDB(kunden); //neue PIN speichern!
					printf("\n\n----------------------------\n");
					printf("Ihre PIN wurde erfolgreich auf %d geaendert.\n", tmp_newPIN);
					printf("----------------------------\n\n");

				}
				else {
					Status("Sie haben eine falsche PIN eingegeben!");
				}

				break;
			case 6: //Kundendaten anzeigen
				kunde_show(me);
				break;
			case 7: //Konto löschen
				printf("Wollen Sie ihre Bank-Beziehung wirklich beenden?\n");
				printf("(J)a oder (N)ein\n");
				char res = (char)getche();
				printf("\n");
				//scanf_s("%c", res);
				if (res == 'j' || res == 'J') {
					Status("Ihr Verhaeltnis ist nun beendet!");
					if (me->Guthaben != 0.0) printf("Es werden Ihnen nun noch %.2lf Euro augezahlt.\n", me->Guthaben);
					kunde_delete(&hashtabelle, me->Kontonummer);
					eingeloggt = false;
					//break;
				}
				break;
			case 8: //abmelden
				eingeloggt = false;
				break;
			case 9: return 0;//Beenden
			default: 
				Status("Ihrer Eingabe konnte kein Menuepunkt zugeordnet werden!\nBitte versuchen Sie es erneut.");
				break;
			}
		}
	} //1.Menu

	return 0;
}