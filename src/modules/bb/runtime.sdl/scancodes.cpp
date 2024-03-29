#define MAX_SDL_SCANCODES 285

unsigned short SDL_SCANCODE_MAP[MAX_SDL_SCANCODES]={
	0,  // SDL_SCANCODE_UNKNOWN = 0,
	0,
	0,
	0,
	30, // SDL_SCANCODE_A = 4,
	48,  // SDL_SCANCODE_B = 5,
	46,  // SDL_SCANCODE_C = 6,
	32,  // SDL_SCANCODE_D = 7,
	18,  // SDL_SCANCODE_E = 8,
	33,  // SDL_SCANCODE_F = 9,
	34,  // SDL_SCANCODE_G = 10,
	35,  // SDL_SCANCODE_H = 11,
	23,  // SDL_SCANCODE_I = 12,
	36,  // SDL_SCANCODE_J = 13,
	37,  // SDL_SCANCODE_K = 14,
	38,  // SDL_SCANCODE_L = 15,
	50,  // SDL_SCANCODE_M = 16,
	49,  // SDL_SCANCODE_N = 17,
	24,  // SDL_SCANCODE_O = 18,
	25,  // SDL_SCANCODE_P = 19,
	16,  // SDL_SCANCODE_Q = 20,
	19,  // SDL_SCANCODE_R = 21,
	31,  // SDL_SCANCODE_S = 22,
	20,  // SDL_SCANCODE_T = 23,
	22,  // SDL_SCANCODE_U = 24,
	47,  // SDL_SCANCODE_V = 25,
	17,  // SDL_SCANCODE_W = 26,
	45,  // SDL_SCANCODE_X = 27,
	21,  // SDL_SCANCODE_Y = 28,
	44,  // SDL_SCANCODE_Z = 29,
	2,  // SDL_SCANCODE_1 = 30,
	3,  // SDL_SCANCODE_2 = 31,
	4,  // SDL_SCANCODE_3 = 32,
	5,  // SDL_SCANCODE_4 = 33,
	6,  // SDL_SCANCODE_5 = 34,
	7,  // SDL_SCANCODE_6 = 35,
	8,  // SDL_SCANCODE_7 = 36,
	9,  // SDL_SCANCODE_8 = 37,
	10,  // SDL_SCANCODE_9 = 38,
	11,  // SDL_SCANCODE_0 = 39,
	28,  // SDL_SCANCODE_RETURN = 40,
	1,   // SDL_SCANCODE_ESCAPE = 41,
	14,  // SDL_SCANCODE_BACKSPACE = 42,
	15,  // SDL_SCANCODE_TAB = 43,
	57,  // SDL_SCANCODE_SPACE = 44,
	12,  // SDL_SCANCODE_MINUS = 45,
	13,  // SDL_SCANCODE_EQUALS = 46,
	26,  // SDL_SCANCODE_LEFTBRACKET = 47,
	27,  // SDL_SCANCODE_RIGHTBRACKET = 48,
	0,  // SDL_SCANCODE_BACKSLASH = 49,
	0,  // SDL_SCANCODE_NONUSHASH = 50,
	0,  // SDL_SCANCODE_SEMICOLON = 51,
	0,  // SDL_SCANCODE_APOSTROPHE = 52,
	0,  // SDL_SCANCODE_GRAVE = 53,
	0,  // SDL_SCANCODE_COMMA = 54,
	0,  // SDL_SCANCODE_PERIOD = 55,
	0,  // SDL_SCANCODE_SLASH = 56,
	0,  // SDL_SCANCODE_CAPSLOCK = 57,
	59,  // SDL_SCANCODE_F1 = 58,
	60,  // SDL_SCANCODE_F2 = 59,
	61,  // SDL_SCANCODE_F3 = 60,
	62,  // SDL_SCANCODE_F4 = 61,
	63,  // SDL_SCANCODE_F5 = 62,
	64,  // SDL_SCANCODE_F6 = 63,
	65,  // SDL_SCANCODE_F7 = 64,
	66,  // SDL_SCANCODE_F8 = 65,
	67,  // SDL_SCANCODE_F9 = 66,
	68,  // SDL_SCANCODE_F10 = 67,
	87,  // SDL_SCANCODE_F11 = 68,
	88,  // SDL_SCANCODE_F12 = 69,
	183,  // SDL_SCANCODE_PRINTSCREEN = 70,
	70,  // SDL_SCANCODE_SCROLLLOCK = 71,
	197,  // SDL_SCANCODE_PAUSE = 72,
	0,  // SDL_SCANCODE_INSERT = 73,
	0,  // SDL_SCANCODE_HOME = 74,
	0,  // SDL_SCANCODE_PAGEUP = 75,
	0,  // SDL_SCANCODE_DELETE = 76,
	0,  // SDL_SCANCODE_END = 77,
	0,  // SDL_SCANCODE_PAGEDOWN = 78,
	205, // SDL_SCANCODE_RIGHT = 79,
	203, // SDL_SCANCODE_LEFT = 80,
	208, // SDL_SCANCODE_DOWN = 81,
	200, // SDL_SCANCODE_UP = 82,
	0,  // SDL_SCANCODE_NUMLOCKCLEAR = 83,
	0,  // SDL_SCANCODE_KP_DIVIDE = 84,
	0,  // SDL_SCANCODE_KP_MULTIPLY = 85,
	0,  // SDL_SCANCODE_KP_MINUS = 86,
	0,  // SDL_SCANCODE_KP_PLUS = 87,
	0,  // SDL_SCANCODE_KP_ENTER = 88,
	0,  // SDL_SCANCODE_KP_1 = 89,
	0,  // SDL_SCANCODE_KP_2 = 90,
	0,  // SDL_SCANCODE_KP_3 = 91,
	0,  // SDL_SCANCODE_KP_4 = 92,
	0,  // SDL_SCANCODE_KP_5 = 93,
	0,  // SDL_SCANCODE_KP_6 = 94,
	0,  // SDL_SCANCODE_KP_7 = 95,
	0,  // SDL_SCANCODE_KP_8 = 96,
	0,  // SDL_SCANCODE_KP_9 = 97,
	0,  // SDL_SCANCODE_KP_0 = 98,
	0,  // SDL_SCANCODE_KP_PERIOD = 99,
	0,  // SDL_SCANCODE_NONUSBACKSLASH = 100
	0,  // SDL_SCANCODE_APPLICATION = 101
	0,  // SDL_SCANCODE_POWER = 102
	0,  // SDL_SCANCODE_KP_EQUALS = 103,
	0,  // SDL_SCANCODE_F13 = 104,
	0,  // SDL_SCANCODE_F14 = 105,
	0,  // SDL_SCANCODE_F15 = 106,
	0,  // SDL_SCANCODE_F16 = 107,
	0,  // SDL_SCANCODE_F17 = 108,
	0,  // SDL_SCANCODE_F18 = 109,
	0,  // SDL_SCANCODE_F19 = 110,
	0,  // SDL_SCANCODE_F20 = 111,
	0,  // SDL_SCANCODE_F21 = 112,
	0,  // SDL_SCANCODE_F22 = 113,
	0,  // SDL_SCANCODE_F23 = 114,
	0,  // SDL_SCANCODE_F24 = 115,
	0,  // SDL_SCANCODE_EXECUTE = 116,
	0,  // SDL_SCANCODE_HELP = 117,
	0,  // SDL_SCANCODE_MENU = 118,
	0,  // SDL_SCANCODE_SELECT = 119,
	0,  // SDL_SCANCODE_STOP = 120,
	0,  // SDL_SCANCODE_AGAIN = 121,   /**< redo */
	0,  // SDL_SCANCODE_UNDO = 122,
	0,  // SDL_SCANCODE_CUT = 123,
	0,  // SDL_SCANCODE_COPY = 124,
	0,  // SDL_SCANCODE_PASTE = 125,
	0,  // SDL_SCANCODE_FIND = 126,
	0,  // SDL_SCANCODE_MUTE = 127,
	0,  // SDL_SCANCODE_VOLUMEUP = 128,
	0,  // SDL_SCANCODE_VOLUMEDOWN = 129,
	0,  // SDL_SCANCODE_LOCKINGCAPSLOCK = 130
	0,  // SDL_SCANCODE_LOCKINGNUMLOCK = 131
	0,  // SDL_SCANCODE_LOCKINGSCROLLLOCK = 132
	0,  // SDL_SCANCODE_KP_COMMA = 133
	0,  // SDL_SCANCODE_KP_EQUALSAS400 = 134
	0,  // SDL_SCANCODE_INTERNATIONAL1 = 135
	0,  // SDL_SCANCODE_INTERNATIONAL2 = 136,
	0,  // SDL_SCANCODE_INTERNATIONAL3 = 137
	0,  // SDL_SCANCODE_INTERNATIONAL4 = 138,
	0,  // SDL_SCANCODE_INTERNATIONAL5 = 139,
	0,  // SDL_SCANCODE_INTERNATIONAL6 = 140,
	0,  // SDL_SCANCODE_INTERNATIONAL7 = 141,
	0,  // SDL_SCANCODE_INTERNATIONAL8 = 142,
	0,  // SDL_SCANCODE_INTERNATIONAL9 = 143,
	0,  // SDL_SCANCODE_LANG1 = 144
	0,  // SDL_SCANCODE_LANG2 = 145
	0,  // SDL_SCANCODE_LANG3 = 146
	0,  // SDL_SCANCODE_LANG4 = 147
	0,  // SDL_SCANCODE_LANG5 = 148
	0,  // SDL_SCANCODE_LANG6 = 149
	0,  // SDL_SCANCODE_LANG7 = 150
	0,  // SDL_SCANCODE_LANG8 = 151
	0,  // SDL_SCANCODE_LANG9 = 152
	0,  // SDL_SCANCODE_ALTERASE = 153
	0,  // SDL_SCANCODE_SYSREQ = 154,
	0,  // SDL_SCANCODE_CANCEL = 155,
	0,  // SDL_SCANCODE_CLEAR = 156,
	0,  // SDL_SCANCODE_PRIOR = 157,
	0,  // SDL_SCANCODE_RETURN2 = 158,
	0,  // SDL_SCANCODE_SEPARATOR = 159,
	0,  // SDL_SCANCODE_OUT = 160,
	0,  // SDL_SCANCODE_OPER = 161,
	0,  // SDL_SCANCODE_CLEARAGAIN = 162,
	0,  // SDL_SCANCODE_CRSEL = 163,
	0,  // SDL_SCANCODE_EXSEL = 164,
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  // SDL_SCANCODE_KP_00 = 176,
	0,  // SDL_SCANCODE_KP_000 = 177,
	0,  // SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
	0,  // SDL_SCANCODE_DECIMALSEPARATOR = 179,
	0,  // SDL_SCANCODE_CURRENCYUNIT = 180,
	0,  // SDL_SCANCODE_CURRENCYSUBUNIT = 181,
	0,  // SDL_SCANCODE_KP_LEFTPAREN = 182,
	0,  // SDL_SCANCODE_KP_RIGHTPAREN = 183,
	0,  // SDL_SCANCODE_KP_LEFTBRACE = 184,
	0,  // SDL_SCANCODE_KP_RIGHTBRACE = 185,
	0,  // SDL_SCANCODE_KP_TAB = 186,
	0,  // SDL_SCANCODE_KP_BACKSPACE = 187,
	0,  // SDL_SCANCODE_KP_A = 188,
	0,  // SDL_SCANCODE_KP_B = 189,
	0,  // SDL_SCANCODE_KP_C = 190,
	0,  // SDL_SCANCODE_KP_D = 191,
	0,  // SDL_SCANCODE_KP_E = 192,
	0,  // SDL_SCANCODE_KP_F = 193,
	0,  // SDL_SCANCODE_KP_XOR = 194,
	0,  // SDL_SCANCODE_KP_POWER = 195,
	0,  // SDL_SCANCODE_KP_PERCENT = 196,
	0,  // SDL_SCANCODE_KP_LESS = 197,
	0,  // SDL_SCANCODE_KP_GREATER = 198,
	0,  // SDL_SCANCODE_KP_AMPERSAND = 199,
	0,  // SDL_SCANCODE_KP_DBLAMPERSAND = 200,
	0,  // SDL_SCANCODE_KP_VERTICALBAR = 201,
	0,  // SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
	0,  // SDL_SCANCODE_KP_COLON = 203,
	0,  // SDL_SCANCODE_KP_HASH = 204,
	0,  // SDL_SCANCODE_KP_SPACE = 205,
	0,  // SDL_SCANCODE_KP_AT = 206,
	0,  // SDL_SCANCODE_KP_EXCLAM = 207,
	0,  // SDL_SCANCODE_KP_MEMSTORE = 208,
	0,  // SDL_SCANCODE_KP_MEMRECALL = 209,
	0,  // SDL_SCANCODE_KP_MEMCLEAR = 210,
	0,  // SDL_SCANCODE_KP_MEMADD = 211,
	0,  // SDL_SCANCODE_KP_MEMSUBTRACT = 212,
	0,  // SDL_SCANCODE_KP_MEMMULTIPLY = 213,
	0,  // SDL_SCANCODE_KP_MEMDIVIDE = 214,
	0,  // SDL_SCANCODE_KP_PLUSMINUS = 215,
	0,  // SDL_SCANCODE_KP_CLEAR = 216,
	0,  // SDL_SCANCODE_KP_CLEARENTRY = 217,
	0,  // SDL_SCANCODE_KP_BINARY = 218,
	0,  // SDL_SCANCODE_KP_OCTAL = 219,
	0,  // SDL_SCANCODE_KP_DECIMAL = 220,
	0,  // SDL_SCANCODE_KP_HEXADECIMAL = 221,
	0,  //
	0,  //
	29, // SDL_SCANCODE_LCTRL = 224,
	42, // SDL_SCANCODE_LSHIFT = 225,
	56, // SDL_SCANCODE_LALT = 226, /**< alt, option */
	219,// SDL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
	157,// SDL_SCANCODE_RCTRL = 228,
	54, // SDL_SCANCODE_RSHIFT = 229,
	184,// SDL_SCANCODE_RALT = 230, /**< alt gr, option */
	220,// SDL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  //
	0,  // SDL_SCANCODE_MODE = 257,
	0,  // SDL_SCANCODE_AUDIONEXT = 258,
	0,  // SDL_SCANCODE_AUDIOPREV = 259,
	0,  // SDL_SCANCODE_AUDIOSTOP = 260,
	0,  // SDL_SCANCODE_AUDIOPLAY = 261,
	0,  // SDL_SCANCODE_AUDIOMUTE = 262,
	0,  // SDL_SCANCODE_MEDIASELECT = 263,
	0,  // SDL_SCANCODE_WWW = 264,
	0,  // SDL_SCANCODE_MAIL = 265,
	0,  // SDL_SCANCODE_CALCULATOR = 266,
	0,  // SDL_SCANCODE_COMPUTER = 267,
	0,  // SDL_SCANCODE_AC_SEARCH = 268,
	0,  // SDL_SCANCODE_AC_HOME = 269,
	0,  // SDL_SCANCODE_AC_BACK = 270,
	0,  // SDL_SCANCODE_AC_FORWARD = 271,
	0,  // SDL_SCANCODE_AC_STOP = 272,
	0,  // SDL_SCANCODE_AC_REFRESH = 273,
	0,  // SDL_SCANCODE_AC_BOOKMARKS = 274,
	0,  // SDL_SCANCODE_BRIGHTNESSDOWN = 275,
	0,  // SDL_SCANCODE_BRIGHTNESSUP = 276,
	0,  // SDL_SCANCODE_DISPLAYSWITCH = 277,
	0,  // SDL_SCANCODE_KBDILLUMTOGGLE = 278,
	0,  // SDL_SCANCODE_KBDILLUMDOWN = 279,
	0,  // SDL_SCANCODE_KBDILLUMUP = 280,
	0,  // SDL_SCANCODE_EJECT = 281,
	0,  // SDL_SCANCODE_SLEEP = 282,
	0,  // SDL_SCANCODE_APP1 = 283,
	0,  // SDL_SCANCODE_APP2 = 284,
};
