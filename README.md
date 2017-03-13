## Micromenu v2
### Example text-based console menu and code generator

An implementation of Dean Camera's MicroMenu v2 with the following features:
- Numeric based console navigation, with formatted menus including built in argument display/setters
- Python script to generate hierarchical menu code and function definitions
- Easy to test out on an Arduino (thanks to DMBS, see below)

### Testing on an Arduino Uno
#### In Windows you'll need:
- MinGW
- WinAVR

1. In the `example/Makefile`, change the `AVRDUDE_PORT` (for example: `./com8`)
2. In the `example` directory, run `make avrdude`
3. Open a serial terminal (default baud is 9600 8N1, set in `usart.c`), type `menu` for menu entry


### Menu Code-generator
The MicroMenu library is awesome because it all resides in flash memory, but the tradeoff is you have to define each menu-node and its relationships explicitly.
To attempt to relieve the hassle, a Python script is included that will read a simple ascii-formatted file and output code you can put straight in the project.

#### Usage
Uh, just run it. It reads the included menuGeneratorInput.txt in the same directory. Edit that or change the script up.

`python menuGenerator.py'

#### Input File
Comma delimited, and the number of tabs preceding each line determine the menu hierarchy.

format:
`menuName,menuGetter,menuSetter,menuArgText`

##### *only menuName is required, any other values can just be blank (or `,,`)

example:
```
menu1,get,set,"Input Menu1Val: "
	menu1a,get,setter,"Input Menu1aVal: "
	menu1b,,setter
		menu1b_1,,setter
menu2,get
```
produces:
```
*********** GENERATED MENU STRUCTURE **********************
MENU_ITEM(Menu_Trunk, menu1, NULL_MENU, NULL_MENU, menu1, NULL, NULL, NULL, "Menu_Trunk");
        MENU_ITEM(menu1, menu2, NULL_MENU, Menu_Trunk, menu1a, get, set, "Input Menu1Val: ", "menu1");
                MENU_ITEM(menu1a, menu1b, NULL_MENU, menu1, NULL_MENU, get, setter, "Input Menu1aVal: ", "menu1a");
                MENU_ITEM(menu1b, NULL_MENU, menu1a, NULL_MENU, menu1b_1, NULL, setter, NULL, "menu1b");
                        MENU_ITEM(menu1b_1, NULL_MENU, NULL_MENU, menu1b, NULL_MENU, NULL, setter, NULL, "menu1b_1");
        MENU_ITEM(menu2, NULL_MENU, menu1, NULL_MENU, NULL_MENU, get, NULL, NULL, "menu2");

*********** GENERATED MENU FUNCTIONS **********************
void menu1_get(void * arg)
void menu1_set(void * arg)
void menu1a_get(void * arg)
void menu1a_setter(void * arg)
void menu1b_setter(void * arg)
void menu1b_1_setter(void * arg)
void menu2_get(void * arg)
```

### Notes
Yes, I know using the printf library is a bit contradictory when using such a lightweight library as MicroMenuV2. Take from this what you will.
