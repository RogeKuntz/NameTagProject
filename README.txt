# Author  : Roge' Kuntz
# Date    : 06/25/2020
# Modified: 06/25/2020
This project is intended to result in a programmable nametag.
It uses a raspberry pi B3, with an I2C 1602 Serial LCD module and a 4x4 key matrix.
The key matrix will be used for input, and the LCD for display.

The program will have ? modes: Display, Edit, file select, and Settings.
Display: Will display whatever the text is, scrolling it across the LCD if it is too long to fit.
	Two lines that scroll. ? multiple lines possible?, when switch?
	Button for mode switch.
Edit: Edit what the text is. 
	Button for mode switch.
	[1] Blinking cursor for edit place, thing in corner that shows char type to insert. Press one 
	button to insert that type of char where the cusor is (moving whatever is in the cursor's 
	place to the right), and another button to remove whatever is just in front of the cursor.
	AKA: same text editing dynamic as nano. Use Solid rectangle cursor/greenhighlight with black text.
		Insert char in place of cursor, move stuff at and behind cursor over
		Delete char before cursor, moving stuff at and after cursor over
		Deleting at start of string does nothing
		Cannot add stuff after null-terminator, or delete null-terminator.
		Cursor stays in middle/end, just doesn't let you scroll further in either direction.
			Lets the cursor go 1 char beyond the end of the string. (Onto null char, but 
			cannot actually delete null char).
		Skip unprintable ascii characters.
		Sample char.
	[2] The type of char to insert can be changed by scrolling up/down.
	[3] Move cursor between top & bottom line.
	[4] Can jump to next char type with other key(eg: lowercase, uppercase, numbers, special)
	[5] Can switch to direct edit char type (rather than insert). Use different cusor.
	When switch out of edit mode, save any changes.
File select: 
	Select from files in a certain folder. 
		Display file name only.
		Use up/down cursor key to change file.
		Use insert key to select.
		Highlight current selected file.
	Save last file used in a particular file. If it does not exist, create file, and use first .txt file in folder.
	If no .txt files in folder, create new, and set last file used to that.
	If listed .txt file not found, use first .txt file in folder.
	Create new file option: enter name, create .txt file with that name, and switch to using that file.
	
Settings: Change settings: 
	Use U/D keys to nav settings.
	Use L/R key to select options for particular setting.
	Scroll speed
	Scroll direction ?
	Loop behavior (loop immediatly at EOF, scroll untill off-screen at EOF, bounce, etc)

Buttons needed: 
	[1 each]: insert char, delete char, mode switch, change char, sample char, next char type
	[2 each]: move cursor L/R, move cursor U/D, ascii u/d.
	
	12 keys needed:

	┌───┬┴┴┴┼┴┴┴┼───┐	┌───┬┴┴┴┼┴┴┴┼───┐
	│CuL│CuR│AU │CuU│	│ 0 │ 1 │ 2 │ 3 │
	├───┼───┼───┼───┤	├───┼───┼───┼───┤
	│Ins│Del│AD │CuD│	│ 4 │ 5 │ 6 │ 7 │
	├───┼───┼───┼───┤	├───┼───┼───┼───┤
	│SCh│CCh│NCh│   │	│ 8 │ 9 │10 │11 │
	├───┼───┼───┼───┤	├───┼───┼───┼───┤
	│   │   │   │Mod│	│12 │13 │14 │15 │
	└───┴───┴───┴───┘	└───┴───┴───┴───┘
	
Abbrev.	│KeyNum	│Name		│
────────┼───────┼───────────────┼───
CuL	│0	│Cursor Left	│
CuR	│1	│Cursor Right	│
AU	│2	│Ascii Up	│
CuU	│3	│Cursor Up	│
Ins	│4	│Insert		│
Del	│5	│Delete		│
AD	│6	│Ascii Down	│
CuD	│7	│Cursor Down	│
SCh	│8	│Sample Char	│
CCh	│9	│Change Char	│
NCh	│10	│Next Char Type	│
~	│11	│~		│
~	│12	│~		│
~	│13	│~		│
~	│14	│~		│
Mod	│15	│Mode		│
