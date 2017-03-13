#include <stdio.h>
#include <stdlib.h>
#include "MicroMenu.h"
#include "usart.h"

int menu1b_val = 42;	// an example variable that can be set from the menu

/**
 Menu1's setter (EnterFunc), called when menu item is selected
**/
void menu1a_setter(void * arg)
{
	printf("\r\nMenu1a Func");
}

/**
 Menu1b's setter (EnterFunc), since arg_txt is set, this will prompt for an input, then send that input here via arg
**/
void menu1b_setter(void * arg)
{
	menu1b_val = atoi(arg);		// arg is input from "Enter menu1b val: "
}

/**
 Menu1b's getter. If defined, will print something besides the menu item
**/
void menu1b_getter(void * arg)
{
	printf("%d", menu1b_val);
}

/**
 Menu2 setter. (EnterFunc), called when menu item is selected
**/
void menu2_setter(void * arg)
{
	printf("\r\nMenu2 Func");
}

/************************** GENERATED MENU DEFINES HERE (or write it yourself if you dare) ************************************/

/* Reference: MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, arg_txt, Text) */
MENU_ITEM(Menu_Trunk, Menu1, NULL_MENU, NULL_MENU, Menu1, NULL, NULL, NULL, "Main Menu");
	MENU_ITEM(Menu1, Menu2, NULL_MENU, Menu_Trunk, Menu1a, NULL, NULL, NULL, "Menu1");
		MENU_ITEM(Menu1a, Menu1b, NULL_MENU, Menu1, NULL_MENU, NULL, menu1a_setter , NULL, "Menu1a");
		MENU_ITEM(Menu1b, NULL_MENU, Menu1a, Menu1, NULL_MENU, menu1b_getter, menu1b_setter , "Enter menu1b val: ", "Menu1b");
	MENU_ITEM(Menu2, NULL_MENU, Menu1, Menu_Trunk, NULL_MENU, NULL, menu2_setter, NULL, "Menu2");

/******************************************************************************************************************************/

int main(void)
{

	usart_init();
	menu_setbase(&Menu_Trunk);			// set root of menu
	Menu_SetHeader("MyMenuHeader");		// set header to print in every menu

    while (1)
		menu_console(usart_getc());		// input loop for menu console
}
