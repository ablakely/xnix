#ifndef MAP_H
#define MAP_H

// xnix: US English QWERTY keyboad layout

#define LEFT_SHIFT  1
#define RIGHT_SHIFT 2
#define CAPS_LOCK   3

unsigned char kbmap[128] = {

	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	'0', 'a', 's', 'd', 'f', 'g', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	LEFT_SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', RIGHT_SHIFT,
	'*',
	0,	// alt
	' ',	// space bar
	CAPS_LOCK,
	0,	// F1
	0,	// F2
	0, 	// F3
	0,	// F4
	0,	// F5
	0,	// F6
	0,	// F7
	0,	// F8
	0,	// F9
	0,	// F10
	0,	// num lock
	0,	// scroll lock
	0,	// home key
	0,	// up
	0,	// page up
	'-',
	0,	// left
	0,
	0,	// right
	'+',
	0,	// end
	0,	// down
	0,	// insert
	0,	// delete
	0,
	0,
	0,
	0,	// F11
	0,	// F12
	0,	// all other keys are undfined
};


unsigned char uckbmap[128] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t',	// tab
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
	0,	// ctrl
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '~',
	LEFT_SHIFT, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', RIGHT_SHIFT,
	'*',
	0,
	' ',
	CAPS_LOCK,
	0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0,
	0,
	0,
	'-',
	0,
	0,
	0,
	'+',
	0,
	0,
	0,
	0,
	0,
	0, 0, 0,
	0,
	0,
	0,
};

#endif
