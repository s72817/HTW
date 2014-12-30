#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Eigene Dateien
#include "menu.h"
#include "funktionen.c"
//#include "menu.c"

#define MaxHash 10
#define KN_len 10 //Anz der Ziffernn der Kontonummer

/// <summary>
/// Kundenstruktur
/// </summary>
typedef struct kunde_t{
	int ID;
	int Kontonummer[10]; //10-stellig
	double Guthaben;
	unsigned int PIN;
	int isblocked;

	struct kunde_t *next;
}kunde;

/// <summary>
/// Die Hashtabelle (Hash-Array) mit den max. Einträgen
/// </summary>
kunde *hashtabelle[MaxHash];

/// <summary>
/// Die Hashfunktion. 
/// </summary>
/// <param name="Kontonummer">Kontonummer als unsigned long int</param>
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
/// <returns>Kunde-Struktur</returns>
kunde *kunde_add(kunde *konto, int counter) {
	int KN_Arr[10];
	get_acc_no(KN_Arr, counter);

	//Hash Wert generienen
	int hashaddress = hash_function(arr_to_num(KN_Arr, KN_len)); //verbesserungswürdig
	/* Zeiger auf errechnete Tabellenadresse durch hash_funktion */
	konto = hashtabelle[hashaddress];

	/* Speicher für neues Element allozieren */
	konto = malloc(sizeof(kunde));
	if (konto == NULL) {
		printf("Kein Speicher für neues Element vorhanden\n");
		return NULL;
	}
	//Struct befüllen
	
	konto->ID = counter;
	for (int i = 0; i < KN_len; i++)
		konto->Kontonummer[i] = KN_Arr[i];
	konto->Guthaben = 0.0;
	konto->PIN = counter * 5 + 1; //TODO: PIN Generator
	konto->isblocked = 0;
	//Zeiger vom nächsten Element bekommt errechnete Hashadresse
	konto->next = hashtabelle[hashaddress];
	//Wert zum Hash(Array) hinzufüen
	hashtabelle[hashaddress] = konto;
	

	printf("\n----------------------------\n");
	printf("Sie haben erfolgreich einen neuen Kunden angelegt!\n");
	printf("KundenID......: %d\n", konto->ID);
	printf("Kontonummer...: %lld", arr_to_num(konto->Kontonummer, KN_len));
	printf("\nGuthaben......: %.2lf Euro\n", konto->Guthaben);
	printf("Pin...........: %d\n", konto->PIN);
	printf("----------------------------\n"); 

	return konto;
}

/// <summary>
/// Suche nach Listenelement mit kontonummer
/// </summary>
/// <param name="konto">kundenliste</param>
/// <param name="Kontonummer">Kontonummer als long long</param>
/// <returns>struct kunde</returns>
kunde *kunde_suche(kunde *konto, long long int Kontonummer) {
	int hashaddress = hash_function(Kontonummer);
	konto = hashtabelle[hashaddress];
	kunde *me = NULL;
	while (konto != NULL) {
		if (arr_to_num(konto->Kontonummer, KN_len) == Kontonummer) {
			me = konto;
			break;
		}
		konto = konto->next;
	}
	return me;
}


/// <summary>
/// Löscht einen Kunden unter Angabe seiner Kontonummer
/// </summary>
/// <param name="konto">Zeiger auf Konto</param>
/// <param name="hashtabelle">Hashtabelle</param>
/// <param name="KN">Kontonummer</param>
void kunde_delete(kunde *konto, long long int Kontonummer) {
	kunde *zeiger, *zeiger1; //Hilfsstrukturen
	int hashaddress = hash_function(Kontonummer); //Hashadresse berechnen
	konto = hashtabelle[hashaddress]; //Liste mit entsprechenden Elementen füllen

	//Wenn erstes Element
	if (konto != NULL) {
		if (arr_to_num(konto->Kontonummer, KN_len) == Kontonummer) { //1.Element
			zeiger = konto->next; //Hilfsstruktur wird mit dem nächsten Zeiger befüllt
			free(konto); //Element löschen
			hashtabelle[hashaddress] = zeiger; //
		}
		else {
			zeiger = konto;
			while (zeiger->next != NULL) {
				zeiger1 = zeiger->next;
				if (arr_to_num(zeiger1->Kontonummer, KN_len) == Kontonummer) {
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
		printf("Es sind keine Daten zum Loeschen vorhanden!\n");
}


void kunde_output_all(kunde *konto, kunde **hashtabelle) {
	for (int i = 0; i < MaxHash; i++){
		konto = hashtabelle[i];
		while (konto != NULL) {
			printf("Kontonummer: %d\nHash: %d\n", konto->Kontonummer, hash_function(arr_to_num(konto->Kontonummer, KN_len)));
			konto = konto->next;
		}
	}
}

//gibt die Entsprechenden Listenelemente mit demselben Hashwert aus!!
void kunde_output_byhash(kunde *konto, long long int KN) {
	int hashaddress = hash_function(KN);
	konto = hashtabelle[hashaddress];

	//pointer = hashtabelle[hashaddress];
	while (konto != NULL) {
		printf("Kontonummer: %lld Hash: %d\n",arr_to_num(konto->Kontonummer, KN_len), hash_function(arr_to_num(konto->Kontonummer, KN_len)));
		konto = konto->next;
	}
}


int main() {
	menu *mymenu = NULL; // init. die Liste mit NULL = leere liste

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
	menu_add(&mymenu, "zurueck", 3);

	kunde *k = NULL; //komplette Liste
	kunde me; //Aktuelles Object

	//kunde *hashtable[MaxHash]; //Hash-Tabelle

	int counter = 0;
	int islogged = 0;


	//Menu
	int auswahl;
	while (1) { //1.Menuebene
		menu_show(mymenu, 1); //Menu erzeugen
		printf("\nIhre Wahl:\n");
		scanf_s("%d", &auswahl);
		switch (auswahl) {
		case 1: kunde_add(&k, counter++);
		case 2: {
			long long int tmp_KN;
			printf("Kontonummer:");
			scanf_s("%lld", &tmp_KN);
			while (getchar() != '\n');

			if (digits(tmp_KN) == 10) {
				int KN[KN_len];
				num_to_arr(KN, KN_len, tmp_KN);
				if (!check_acc_no(KN)) printf("Falsch\n"); //Prüfziffer checken

				if (kunde_suche(k, tmp_KN) != NULL) {
					me = *kunde_suche(k, tmp_KN);
					if (me.isblocked = 1) {
						printf("\n\n----------------------------\n");
						printf("Die Kontonummer %d ist blockiert.\nBitte wenden Sie sich an den naechsten\nfreien Service-Mitarbeiter.\n", me.Kontonummer);
						printf("----------------------------\n");
						break;
					}
					islogged = 1;
					printf("Nummer %lld gefunden!\n", arr_to_num(me.Kontonummer, KN_len));
				}
				else
					printf("Nummer nicht gefunden!\n");
				break;
			}
			else printf("Bitte geben Sie ihre 10-stellige Kontonummer an!\n");
			break;
		}
		default:
			printf("Ihrer Eingabe konnte kein Menuepunkt zugeordnet werden!\nBitte versuchen Sie es erneut.\n");
			break;
			
			
		} //Switch
	} //1.Menu

	getch();
	return 0;
}

//printf("1 - Kunde anlegen\n");
//printf("2 - Kunde suchen\n");
//printf("3 - Kunden unsortiert ausgeben\n");
//printf("4 - Kunde loeschen\n");
//
//scanf_s("%d", &auswahl);
//switch (auswahl) {
//case 1:
//	//kunde_add(&k, counter++);
//	for (int i = 0; i < 20; i++)
//		kunde_add(k, counter++);
//
//	break;
//case 2:
//{
//	long long int tmp_KN;
//	printf("Kontonummer:");
//	scanf_s("%lld", &tmp_KN);
//	while (getchar() != '\n');
//
//	if (digits(tmp_KN) == 10) {
//		int KN[KN_len];
//		num_to_arr(KN, KN_len, tmp_KN);
//		if (!check_acc_no(KN)) printf("Falsch\n");
//
//		if (kunde_suche(k, tmp_KN) != NULL) {
//			me = *kunde_suche(k, tmp_KN);
//			printf("Nummer %lld gefunden!\n", arr_to_num(me.Kontonummer, KN_len));
//		}
//		else
//			printf("Nummer nicht gefunden!\n");
//		break;
//	}
//	else printf("Bitte geben Sie ihre 10-stellige Kontonummer an!\n");
//
//}
//break;
//case 3: {
//	long long int tmp_KN;
//	printf("Kontonummer:");
//	scanf_s("%lld", &tmp_KN);
//	while (getchar() != '\n');
//	if (kunde_suche(k, tmp_KN) != NULL)
//		kunde_output_byhash(k, tmp_KN);
//	else printf("Kontonummer nicht gefunden!\n");
//	break;
//}
//case 4: {
//	long long int tmp_KN;
//	printf("Kontonummer:");
//	scanf_s("%lld", &tmp_KN);
//	if (kunde_suche(k, tmp_KN) != NULL)
//		kunde_delete(k, tmp_KN);
//	else printf("Kontonummer nicht gefunden!\n");
//}
//}

/// <summary>
/// String_to_uls the specified string.
/// </summary>
/// <param name="string">The string.</param>
/// <returns></returns>
//unsigned long int string_to_ul(char string[]) {return atol(string);}

/*int KN[10];

long long int into;
scanf_s("%lld", &into);
printf("Eingabe: %lld\n", into);

num_to_arr(KN, 10, into);

for (int i=0;i<10;i++) {
printf("%d", KN[i]);
}*/

/*for (int i = 0; i<10; i++)
printf("%ld\n", GetRand);*/

/*int Arr[10] = { 3, 8, 2, 7, 4, 1, 8, 2, 6, 7 };
int B[10];
for (int i = 0; i < 10; i++) {
B[i] = Arr[i];
printf("%d", B[i]);
}*/

/*int Arr[10];
for (int i = 0; i < 10; i++) {
get_acc_no(Arr);
for (int i = 0; i < 10; i++)
printf("%d", Arr[i]);
if (check_acc_no(Arr) == 1) printf(" Richtig!");
printf("\n");
}*/

/*int KN[KN_len] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1 };
int KN1[KN_len] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1 };
if (memcmp(KN, KN1, sizeof(KN)) == 0)
printf("gleich");
else
printf("ungleich");*/

//int i;
//int array1[KN_len] = { 2, 4, 1, 7, 4, 0, 9, 4, 8, 8 };
//int array2[KN_len] = { 2, 4, 1, 7, 4, 0, 9, 4, 8, 8 };

/////*for (i = 0; i < KN_len; i++) {
////	array1[i] = i;
////	array2[i] = i;
////}*/
//////array2[5] = 100; /* Verändert array2 an Pos. 5. */

//if (memcmp(array1, array2, sizeof(array1)) == 0)
//	printf("Beide Arrays haben den gleichen Inhalt\n");
//else
//	printf("Die Arrays sind unterschiedlich\n");

/*long long int KNummer;
printf("Kontonummer:");
scanf_s("%lld", &KNummer);
printf("%d", digits(KNummer));*/