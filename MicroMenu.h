/*
 * micromenu.h
 *
 * Created: 5/2/2014
 * tomek0, modified Dean Camera's library
 */ 

#ifndef _MICRO_MENU_H_
#define _MICRO_MENU_H_

	#include <stddef.h>
	#include <stdint.h>

	/** Include header to define the functions/macros used for \ref MENU_ITEM_STORAGE
	 *  and \ref MENU_ITEM_READ_POINTER.
	 */
	#include <avr/pgmspace.h>
	extern const char PROGMEM no_prompt[];
	extern void print_header(void);
	/** Configuration for the desired memory storage location of created menu items.
	 *  This may be configured to locate the menus into FLASH memory space, SRAM, or
	 *  any other suitable memory space.
	 */
	#define MENU_ITEM_STORAGE              PROGMEM
	
	#define HAS_ARG		1
	#define NO_ARG		0
	/** Configuration for the macro or function required to read out a pointer from
	 *  the memory storage space set by \ref MENU_ITEM_STORAGE.
	 *
	 *  \param[in] Addr  Address of the pointer to read
	 */
	#define MENU_ITEM_READ_POINTER(Addr)   (void*)pgm_read_word(Addr)

	/** Type define for a menu item. Menu items should be initialized via the helper
	 *  macro \ref MENU_ITEM(), not created from this type directly in user-code.
	 */
	typedef const struct Menu_Item {
		const struct Menu_Item *Next; /**< Pointer to the next menu item of this menu item */
		const struct Menu_Item *Previous; /**< Pointer to the previous menu item of this menu item */
		const struct Menu_Item *Parent; /**< Pointer to the parent menu item of this menu item */
		const struct Menu_Item *Child; /**< Pointer to the child menu item of this menu item */
		void (*SelectCallback)(void * arg); /**< Pointer to the optional menu-specific select callback of this menu item */
		uint8_t (*EnterCallback)(void * arg); /**< Pointer to the optional menu-specific enter callback of this menu item */
		//const uint8_t has_arg;
		const char * arg_txt;
		const char Text[]; /**< Menu item text to pass to the menu display callback function */
	} Menu_Item_t;

	/** Creates a new menu item entry with the specified links and callbacks.
	 *
	 *  \param[in] Name      Name of the menu entry, must be unique.
	 *  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
	 *  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
	 */
	#define MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, arg_txt, Text) \
		extern Menu_Item_t MENU_ITEM_STORAGE Next;     \
		extern Menu_Item_t MENU_ITEM_STORAGE Previous; \
		extern Menu_Item_t MENU_ITEM_STORAGE Parent;   \
		extern Menu_Item_t MENU_ITEM_STORAGE Child;  \
		Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, arg_txt, Text}

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
	#define MENU_PARENT         MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Parent)

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
	#define MENU_CHILD          MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Child)

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
	#define MENU_NEXT           MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Next)

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
	#define MENU_PREVIOUS       MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Previous)

	/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
	extern Menu_Item_t MENU_ITEM_STORAGE NULL_MENU;

	/** Retrieves the currently selected meny item.
	 *
	 *  \return Pointer to the currently selected meny item.
	 */
	Menu_Item_t* Menu_GetCurrentMenu(void);

	/** Navigates to an absolute or relative menu entry.
	 *
	 * \param[in] NewMenu  Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
	 *                     \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
	 */
	int Menu_Navigate(Menu_Item_t* const NewMenu);
	
	void Menu_SetCurrent(Menu_Item_t* const NewMenu);
	int Menu_getNumChildren(void);
	int Menu_GoBack(void);
	void Menu_Print(void);
	int Menu_GetCurrNumChildren(void);
	uint8_t Menu_is_hidden(int child_num);
	
	int Menu_SelectChild(int child_num, void * arg);

	/** Configures the menu text write callback function, fired for all menu items. Within this callback
	 *  function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
	 *  memory space.
	 *
	 *  \ref WriteFunc  Pointer to a callback function to execute for each selected menu item.
	 */
	//void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text));

	/** Enters the currently selected menu item, running its configured callback function (if any). */
	void Menu_EnterCurrentItem(void);

	void menu_console(char c);
	void Menu_SetHeader(const char * str);



#endif
