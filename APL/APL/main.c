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
			me = kunde_anmelden(&hashtabelle, me, &eingeloggt);
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
				kunde_einzahlung(me);
				break;
			case 2:  //Auszahlung
				kunde_auszahlung(me);
				break;
			case 3: //Überweisung
				kunde_ueberweisung(&hashtabelle, me);
				break;
			case 4:	 //Kontostand
				printf("\n\n----------------------------\n");
				printf("Ihr Guthaben betraegt: %.2lf Euro\n", me->Guthaben);
				printf("----------------------------\n");
				break;
			case 5:  //4 - stellige PIN aendern
				kunde_pin_aendern(me);
				break;
			case 6: //Kundendaten anzeigen
				kunde_show(me);
				break;
			case 7: //Konto löschen
				kunde_loeschen(&hashtabelle, me, &eingeloggt);
				break;
			case 8: //abmelden
				kunde_abmelden(me, &eingeloggt);
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