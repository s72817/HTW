//-------------------------------------------------------------------
// APL 2.0 by Richard Kerber
// Bibilotheksnummer: s72817
// 
// Projekt: Ein interaktives Banksystem
// Bei Github: https://github.com/s72817/HTW/tree/master/APL/APL
//
// menu.h
//-------------------------------------------------------------------


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/// <summary>
/// Menu-Struktur mit Ebenen
/// </summary>
/// <param name="item">Menu-Name</param>
/// <param name="level">Menuebene</param>
typedef struct _menu{
	char item[40];
	int level;
	struct _menu *next;
} menu;

/// <summary>
/// Menuelement hinzufugen
/// </summary>
/// <param name="list">Zeiger auf Liste</param>
/// <param name="item">Menu-Name</param>
/// <param name="level">Menuebene</param>
void menu_add(menu **list, char *item, int level){

	menu *newEl;
	menu *lst_iter = *list;

	newEl = (menu*)malloc(sizeof(*newEl)); // erzeuge ein neues Element
	strcpy_s(newEl->item, sizeof(newEl->item), item);
	newEl->level = level;
	newEl->next = NULL; // Wichtig für das Erkennen des Listenendes

	if (lst_iter != NULL) { // sind Elemente vorhanden
		while (lst_iter->next != NULL) // suche das letzte Element
			lst_iter = lst_iter->next;
		lst_iter->next = newEl; // Hänge das Element hinten an
	}
	else // wenn die liste leer ist, ist newEl das erste Element
		*list = newEl;
}

/// <summary>
/// Menu wird erstellt
/// </summary>
/// <param name="list">Liste</param>
/// <param name="level">Menuebene</param>
void menu_show(menu *list, int level) {
	int counter = 1;
	while (list != NULL) {
		if (list->level == level)
			printf("%d. %s\n", counter++, list->item);
		list = list->next;
	}
}