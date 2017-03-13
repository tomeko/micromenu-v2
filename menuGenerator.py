import re, sys

with open('menuGeneratorInput.txt') as f:
    inp = f.readlines()

class MenuItem(object):
	def __init__(self, name="NULL_MENU", level=0, parent="NULL_MENU", child="NULL_MENU", previous="NULL_MENU", nextm="NULL_MENU", getter="NULL", setter="NULL",argtxt="NULL",index=0):
		self.name = name
		self.level = level
		self.parent = parent
		self.child = child
		self.previous = previous
		self.nextm = nextm
		self.index = index
		self.getter = getter
		self.setter = setter
		self.argtxt = argtxt
	def __str__(self):
		return '\t' * self.level + "MENU_ITEM({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, \"{8}\");".format(self.name, self.nextm, self.previous, self.parent, self.child, self.getter, self.setter, self.argtxt, self.name)

def findNext(item, lst):
	for lstitem in lst[item.index+1:]:
		if (lstitem.level == item.level):
			item.nextm = lstitem.name

def findPrevious(item, lst):
	for lstitem in reversed(lst[:item.index]):
		if (lstitem.level == item.level):
			item.previous = lstitem.name

functemplate = "void {0}_{1}(void * arg)"
helptxt = "Error parsing, check sample input:\n\nmenu1,get,set,inputval\n\tmenu1a,get,set\n\tmenu1b\nmenu2"

if __name__ == "__main__":
	menu = []
	menu.append(MenuItem("Menu_Trunk"));	
	if (len(inp) < 1):
		print(helptxt)
		sys.exit()
	for i, item in enumerate(inp):
		me = MenuItem()
		me.level = item.count('\t') + 1 		# for base/trunk menu
		csv = [x.strip() for x in item.split(',')]
		if (len(csv) > 0):
			me.name = csv[0]
		else:
			print(helptxt)
			sys.exit()
		if (len(csv) > 1):				# extra checks here as not all fields are required
			if (len(csv[1]) < 1):
				me.getter = "NULL"
			else:
				me.getter = csv[1]
		if (len(csv) > 2):
			if (len(csv[2]) < 1):
				me.setter = "NULL"
			else:
				me.setter = csv[2]	
		if (len(csv) > 3):
			if (len(csv[3]) < 1):
				me.argtxt = "NULL"
			else:
				me.argtxt = csv[3]
		me.index = i + 1
		menu.append(me)
	
	menu[0].nextm = menu[1].name 	# for base/trunk menu
	for m in menu:					# fill in each menuitem's members sequentially
		findNext(m,menu)
		findPrevious(m,menu)
		if (m.index > 0):
			if (menu[m.index-1].level == m.level - 1):
				m.parent = menu[m.index-1].name
		if (m.index + 1 < len(menu)):
			if (menu[m.index+1].level == m.level + 1):
				m.child = menu[m.index+1].name

	print("\n*********** GENERATED MENU STRUCTURE **********************")
	for m in menu:		# menu structure
		print(m)

	print("\n*********** GENERATED MENU FUNCTIONS **********************")
	for m in menu:		# output generated functions
		if (m.getter != "NULL"):
			print(functemplate.format(m.name, m.getter))
		if (m.setter != "NULL"):
			print(functemplate.format(m.name, m.setter))

