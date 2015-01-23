//-------------------------------------------------------------------
// APL 2.0 by Richard Kerber
// Bibilotheksnummer: s72817
// 
// Projekt: Ein interaktives Banksystem
// Bei Github: https://github.com/s72817/HTW/tree/master/APL/APL
//
// main.c
//-------------------------------------------------------------------



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Eigene Dateien !!Reihenfolge beachten!!
#include "menu.h" // unabhängiges Menu
#include "funktionen.h" //unabhängige Funktionen
#include "error.h" //unabhängige Fehlermeldungen
#include "kunde.h" //abhängig von funktionen.h und error.h - bietet die Kundenklasse an inkl. Hashtabelle


/// <summary>
/// Hauptroutine
/// </summary>
/// <returns></returns>
int main() {
	menu *mymenu = NULL; // init. die Liste mit NULL = leere liste

	kunde *me = NULL; //Aktuelles Object
	kunde *hashtabelle[MAXHASH] = {NULL}; //Hashtabelle + Initialisierung!

	int counter = 0; //Kundenzähler
	bool eingeloggt = false; //geloggt
	long long int tmp_KN = 0; //Temporäre Variablen für Kontonummer
	int tmp_PIN = 0; // und PIN anlegen
	int versuche = 0; //PIN-Versuche initialisieren

	//menu_liste(&mymenu);


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
				kunde_show(kunde_hinzufuegen(&hashtabelle, counter++));
			break;
		case 2:  //kunde anmelden
			while (1) {
				printf("Kontonummer: ");
				scanf_s("%lld", &tmp_KN); //tmp_Kontonummer mit Benutzereingabe füllen
				while (getchar() != '\n');
				if (tmp_KN >= 1000000000 && tmp_KN <= 9999999999) break; //Wenn Kontonummer zwischen 1000000000 und 9999999999 liegt, dann weiter
				else Status("Bitte geben Sie Ihre 10-stellige Kontonummer an!");
			}
			
			if ((me = kunde_suche(&hashtabelle, tmp_KN)) != NULL) {
				//Abfrage, ob blockiert
				if (me->blockiert == true) {
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

				kunde_auszahlung(&hashtabelle, me, tmp_Auszahlung);
				kunde_show(me);
				break;
			}
			case 3: //Überweisung
				Status("Ueberweisung\n");
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
					kunde_loeschen(&hashtabelle, me->Kontonummer);
					eingeloggt = false;
					//break;
				}
				break;
			case 8: //abmelden
				kunde_abmelden(me, &eingeloggt);
				//eingeloggt = false;
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