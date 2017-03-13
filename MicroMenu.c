/*
 * micromenu.c
 *
 * Created: 5/2/2014
*  tomek0, modified Dean Camera's library
 */ 
/**
              MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.
	                                  */

#include <stdio.h>
#include <string.h>
#include "micromenu.h"
#include "usart.h"

#define TABSIZE	20
#define MAX_MENU_INPUT_SIZE	32
#define HIDDEN_MENU_NAME	"Hidden Menu"

static char input_buffer[MAX_MENU_INPUT_SIZE] = {0};		// for arg inputs
static char console_buffer[MAX_MENU_INPUT_SIZE] = {0};		// for console navigation
static char * menu_header;
const char PROGMEM no_prompt[] = "";

static Menu_Item_t * trunk;

static uint8_t in_menu = 0;
static uint8_t num_children = 0;
static uint8_t console_buff_pnt = 0;

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t PROGMEM NULL_MENU = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
//static void (*MenuWriteFunc)(const char* Text) = NULL;

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;

static Menu_Item_t* tmpMenuItem = &NULL_MENU;

Menu_Item_t* Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void Menu_SetCurrent(Menu_Item_t* const NewMenu)
{
	CurrentMenuItem = NewMenu;
}

uint8_t Menu_is_hidden(int child_num)
{
	tmpMenuItem = CurrentMenuItem;
	
	tmpMenuItem = MENU_ITEM_READ_POINTER(&CurrentMenuItem->Child);
	while (--child_num)
		tmpMenuItem = MENU_ITEM_READ_POINTER(&tmpMenuItem->Next);
	
	if (!memcmp_P(HIDDEN_MENU_NAME, tmpMenuItem->Text,sizeof(HIDDEN_MENU_NAME)))
		return 1;
	return 0;
}

// returns 0 if leaf, 1 if not
int Menu_SelectChild(int child_num, void * arg)
{
	tmpMenuItem = CurrentMenuItem;
	
	CurrentMenuItem = MENU_ITEM_READ_POINTER(&CurrentMenuItem->Child);
	while (--child_num)
		CurrentMenuItem = MENU_ITEM_READ_POINTER(&CurrentMenuItem->Next);
		
	uint8_t (*EnterCallback)(void * arg) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->EnterCallback);
	char * tmparg = MENU_ITEM_READ_POINTER(&CurrentMenuItem->arg_txt);
	// if it's a leaf node, do the enterfunc, then set current item back to parent
	if (tmparg != NULL){
		usart_puts("\r\n");
		usart_puts(tmparg);
		usart_getstr((char *)&input_buffer);
	}
	if (EnterCallback){
		EnterCallback((void *)input_buffer);
		CurrentMenuItem = tmpMenuItem;
		Menu_Print();
		return 0;
	}
	return 1;
}

int Menu_GoBack(void)
{
	if (MENU_ITEM_READ_POINTER(&CurrentMenuItem->Parent) == &NULL_MENU)
		return 0;
	else
		CurrentMenuItem = MENU_ITEM_READ_POINTER(&CurrentMenuItem->Parent);
	return 1;
}

int Menu_GetCurrNumChildren(void)
{
	return num_children;
}

void print_tab(uint8_t spaces)
{
	while (spaces--)
		usart_putc(' ');
}

void Menu_SetHeader(const char * str)
{
	menu_header = str;
}

void Menu_Print(void)
{
	uint8_t cnt = 0;
	printf_P(PSTR("\r\n*** %s"), menu_header);
	
	printf_P(PSTR(" | %S\r\n"),CurrentMenuItem->Text);
	tmpMenuItem = MENU_ITEM_READ_POINTER(&CurrentMenuItem->Child);
	while (tmpMenuItem != &NULL_MENU)	// print children
	{
		printf_P(PSTR(" %d. %S"), ++cnt, tmpMenuItem->Text);
		if (MENU_ITEM_READ_POINTER(&tmpMenuItem->Child) != &NULL_MENU)
			printf_P(PSTR("..."));
		void (*SelectCallback)(void * arg) = MENU_ITEM_READ_POINTER(&tmpMenuItem->SelectCallback);
		if (SelectCallback){
			print_tab(TABSIZE - strlen_P(tmpMenuItem->Text));
			usart_putc('[');
			SelectCallback((void *)&cnt);	// passes index as arg
			usart_putc(']');
		}
		usart_puts("\r\n");
		tmpMenuItem = MENU_ITEM_READ_POINTER(&tmpMenuItem->Next);
	}
	printf_P(PSTR(" 0. Back\r\n"));
	num_children = cnt;
}

void Menu_EnterCurrentItem(void)
{
	if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
		return;

	uint8_t (*EnterCallback)(void * arg) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->EnterCallback);

 	if (EnterCallback)
 		EnterCallback(NULL);
}

void reset_buffer(void)
{
	memset((void *)&console_buffer,0,sizeof(console_buffer));
	console_buff_pnt = 0;
}

void menu_exit()
{
	usart_puts("\r\nExiting Menu...\r\n");
	in_menu = 0;
}


void menu_setbase(Menu_Item_t * mnu)
{
	trunk = mnu;
	reset_buffer();
}

void menu_console(char c)
{
	switch (c)
	{
		case BS:
			if (console_buff_pnt > 0){
				console_buff_pnt--;
				usart_putc(BS);
			}
			break;
		
		case LF: break;
		case CR:
			usart_putc(c);
			if (in_menu){
				if (console_buffer[0] > '0')
				{
					uint8_t print_menu = 1;
					uint8_t inp = console_buffer[0]-'0';
					if (inp <= Menu_GetCurrNumChildren())
						print_menu = Menu_SelectChild(inp, NULL);
					else
						printf_P(PSTR("\r\nInvalid selection"));
					if (print_menu)
						Menu_Print();
				}else if (console_buffer[0] == '0'){
					if (!Menu_GoBack()){					
						menu_exit();
					}else{
						Menu_Print();
					}
				}else{
					printf_P(PSTR("\r\nInvalid selection"));
					Menu_Print();
				}
			}else{
				if (!memcmp_P(console_buffer,PSTR("menu"),4)){
					in_menu = 1;
					Menu_SetCurrent(trunk);
					Menu_Print();
				}
			}
			reset_buffer();
		break;
		default:
			if (console_buff_pnt < MAX_MENU_INPUT_SIZE ){
				console_buffer[console_buff_pnt++] = c;
					usart_putc(c);
			}else{
				reset_buffer();
			}
		break;
	}
}