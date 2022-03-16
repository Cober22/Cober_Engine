#pragma once

// From SDL.h

// SDL_GameControllerButton
#define     SDL_CONTROLLER_BUTTON_INVALID = -1,
#define     SDL_CONTROLLER_BUTTON_A,
#define     SDL_CONTROLLER_BUTTON_B,
#define     SDL_CONTROLLER_BUTTON_X,
#define     SDL_CONTROLLER_BUTTON_Y,
#define     SDL_CONTROLLER_BUTTON_BACK,
#define     SDL_CONTROLLER_BUTTON_GUIDE,
#define     SDL_CONTROLLER_BUTTON_START,
#define     SDL_CONTROLLER_BUTTON_LEFTSTICK,
#define     SDL_CONTROLLER_BUTTON_RIGHTSTICK,
#define     SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
#define     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
#define     SDL_CONTROLLER_BUTTON_DPAD_UP,
#define     SDL_CONTROLLER_BUTTON_DPAD_DOWN,
#define     SDL_CONTROLLER_BUTTON_DPAD_LEFT,
#define     SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
#define     SDL_CONTROLLER_BUTTON_MAX

// SDL_Scancode
#define     SDL_SCANCODE_A = 4,
#define     SDL_SCANCODE_B = 5,
#define     SDL_SCANCODE_C = 6,
#define     SDL_SCANCODE_D = 7,
#define     SDL_SCANCODE_E = 8,
#define     SDL_SCANCODE_F = 9,
#define     SDL_SCANCODE_G = 10,
#define     SDL_SCANCODE_H = 11,
#define     SDL_SCANCODE_I = 12,
#define     SDL_SCANCODE_J = 13,
#define     SDL_SCANCODE_K = 14,
#define     SDL_SCANCODE_L = 15,
#define     SDL_SCANCODE_M = 16,
#define     SDL_SCANCODE_N = 17,
#define     SDL_SCANCODE_O = 18,
#define     SDL_SCANCODE_P = 19,
#define     SDL_SCANCODE_Q = 20,
#define     SDL_SCANCODE_R = 21,
#define     SDL_SCANCODE_S = 22,
#define     SDL_SCANCODE_T = 23,
#define     SDL_SCANCODE_U = 24,
#define     SDL_SCANCODE_V = 25,
#define     SDL_SCANCODE_W = 26,
#define     SDL_SCANCODE_X = 27,
#define     SDL_SCANCODE_Y = 28,
#define     SDL_SCANCODE_Z = 29,

#define     SDL_SCANCODE_1 = 30,
#define     SDL_SCANCODE_2 = 31,
#define     SDL_SCANCODE_3 = 32,
#define     SDL_SCANCODE_4 = 33,
#define     SDL_SCANCODE_5 = 34,
#define     SDL_SCANCODE_6 = 35,
#define     SDL_SCANCODE_7 = 36,
#define     SDL_SCANCODE_8 = 37,
#define     SDL_SCANCODE_9 = 38,
#define     SDL_SCANCODE_0 = 39,

#define     SDL_SCANCODE_RETURN = 40,
#define     SDL_SCANCODE_ESCAPE = 41,
#define     SDL_SCANCODE_BACKSPACE = 42,
#define     SDL_SCANCODE_TAB = 43,
#define     SDL_SCANCODE_SPACE = 44,

#define     SDL_SCANCODE_MINUS = 45,
#define     SDL_SCANCODE_EQUALS = 46,
#define     SDL_SCANCODE_LEFTBRACKET = 47,
#define     SDL_SCANCODE_RIGHTBRACKET = 48,
#define     SDL_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                       *   key on ISO keyboards and at the right end
                                       *   of the QWERTY row on ANSI keyboards.
                                       *   Produces REVERSE SOLIDUS (backslash) and
                                       *   VERTICAL LINE in a US layout, REVERSE
                                       *   SOLIDUS and VERTICAL LINE in a UK Mac
                                       *   layout, NUMBER SIGN and TILDE in a UK
                                       *   Windows layout, DOLLAR SIGN and POUND SIGN
                                       *   in a Swiss German layout, NUMBER SIGN and
                                       *   APOSTROPHE in a German layout, GRAVE
                                       *   ACCENT and POUND SIGN in a French Mac
                                       *   layout, and ASTERISK and MICRO SIGN in a
                                       *   French Windows layout.
                                       */
#define     SDL_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                       *   instead of 49 for the same key, but all
                                       *   OSes I've seen treat the two codes
                                       *   identically. So, as an implementor, unless
                                       *   your keyboard generates both of those
                                       *   codes and your OS treats them differently,
                                       *   you should generate SDL_SCANCODE_BACKSLASH
                                       *   instead of this code. As a user, you
                                       *   should not rely on this code because SDL
                                       *   will never generate it with most (all?)
                                       *   keyboards.
                                       */
#define     SDL_SCANCODE_SEMICOLON = 51,
#define     SDL_SCANCODE_APOSTROPHE = 52,
#define     SDL_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                                       *   and ISO keyboards). Produces GRAVE ACCENT and
                                       *   TILDE in a US Windows layout and in US and UK
                                       *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                       *   and NOT SIGN in a UK Windows layout, SECTION
                                       *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                       *   layouts on ISO keyboards, SECTION SIGN and
                                       *   DEGREE SIGN in a Swiss German layout (Mac:
                                       *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                       *   DEGREE SIGN in a German layout (Mac: only on
                                       *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                       *   French Windows layout, COMMERCIAL AT and
                                       *   NUMBER SIGN in a French Mac layout on ISO
                                       *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                       *   SIGN in a Swiss German, German, or French Mac
                                       *   layout on ANSI keyboards.
                                       */
#define     SDL_SCANCODE_COMMA = 54,
#define     SDL_SCANCODE_PERIOD = 55,
#define     SDL_SCANCODE_SLASH = 56,

#define     SDL_SCANCODE_CAPSLOCK = 57,

#define     SDL_SCANCODE_F1 = 58,
#define     SDL_SCANCODE_F2 = 59,
#define     SDL_SCANCODE_F3 = 60,
#define     SDL_SCANCODE_F4 = 61,
#define     SDL_SCANCODE_F5 = 62,
#define     SDL_SCANCODE_F6 = 63,
#define     SDL_SCANCODE_F7 = 64,
#define     SDL_SCANCODE_F8 = 65,
#define     SDL_SCANCODE_F9 = 66,
#define     SDL_SCANCODE_F10 = 67,
#define     SDL_SCANCODE_F11 = 68,
#define     SDL_SCANCODE_F12 = 69,

#define     SDL_SCANCODE_PRINTSCREEN = 70,
#define     SDL_SCANCODE_SCROLLLOCK = 71,
#define     SDL_SCANCODE_PAUSE = 72,
#define     SDL_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                         does send code 73, not 117) */
#define     SDL_SCANCODE_HOME = 74,
#define     SDL_SCANCODE_PAGEUP = 75,
#define     SDL_SCANCODE_DELETE = 76,
#define     SDL_SCANCODE_END = 77,
#define     SDL_SCANCODE_PAGEDOWN = 78,
#define     SDL_SCANCODE_RIGHT = 79,
#define     SDL_SCANCODE_LEFT = 80,
#define     SDL_SCANCODE_DOWN = 81,
#define     SDL_SCANCODE_UP = 82,

#define     SDL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                   */
#define     SDL_SCANCODE_KP_DIVIDE = 84,
#define     SDL_SCANCODE_KP_MULTIPLY = 85,
#define     SDL_SCANCODE_KP_MINUS = 86,
#define     SDL_SCANCODE_KP_PLUS = 87,
#define     SDL_SCANCODE_KP_ENTER = 88,
#define     SDL_SCANCODE_KP_1 = 89,
#define     SDL_SCANCODE_KP_2 = 90,
#define     SDL_SCANCODE_KP_3 = 91,
#define     SDL_SCANCODE_KP_4 = 92,
#define     SDL_SCANCODE_KP_5 = 93,
#define     SDL_SCANCODE_KP_6 = 94,
#define     SDL_SCANCODE_KP_7 = 95,
#define     SDL_SCANCODE_KP_8 = 96,
#define     SDL_SCANCODE_KP_9 = 97,
#define     SDL_SCANCODE_KP_0 = 98,
#define     SDL_SCANCODE_KP_PERIOD = 99,

#define     SDL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                                 *   keyboards have over ANSI ones,
                                                 *   located between left shift and Y.
                                                 *   Produces GRAVE ACCENT and TILDE in a
                                                 *   US or UK Mac layout, REVERSE SOLIDUS
                                                 *   (backslash) and VERTICAL LINE in a
                                                 *   US or UK Windows layout, and
                                                 *   LESS-THAN SIGN and GREATER-THAN SIGN
                                                 *   in a Swiss German, German, or French
                                                 *   layout. */
#define     SDL_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
#define     SDL_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                                        *   not a physical key - but some Mac keyboards
                                        *   do have a power key. */
#define     SDL_SCANCODE_KP_EQUALS = 103,
#define     SDL_SCANCODE_F13 = 104,
#define     SDL_SCANCODE_F14 = 105,
#define     SDL_SCANCODE_F15 = 106,
#define     SDL_SCANCODE_F16 = 107,
#define     SDL_SCANCODE_F17 = 108,
#define     SDL_SCANCODE_F18 = 109,
#define     SDL_SCANCODE_F19 = 110,
#define     SDL_SCANCODE_F20 = 111,
#define     SDL_SCANCODE_F21 = 112,
#define     SDL_SCANCODE_F22 = 113,
#define     SDL_SCANCODE_F23 = 114,
#define     SDL_SCANCODE_F24 = 115,
#define     SDL_SCANCODE_EXECUTE = 116,
#define     SDL_SCANCODE_HELP = 117,
#define     SDL_SCANCODE_MENU = 118,
#define     SDL_SCANCODE_SELECT = 119,
#define     SDL_SCANCODE_STOP = 120,
#define     SDL_SCANCODE_AGAIN = 121,   /**< redo */
#define     SDL_SCANCODE_UNDO = 122,
#define     SDL_SCANCODE_CUT = 123,
#define     SDL_SCANCODE_COPY = 124,
#define     SDL_SCANCODE_PASTE = 125,
#define     SDL_SCANCODE_FIND = 126,
#define     SDL_SCANCODE_MUTE = 127,
#define     SDL_SCANCODE_VOLUMEUP = 128,
#define     SDL_SCANCODE_VOLUMEDOWN = 129,

#define     SDL_SCANCODE_KP_COMMA = 133,
#define     SDL_SCANCODE_KP_EQUALSAS400 = 134,

#define     SDL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                                     footnotes in USB doc */
#define     SDL_SCANCODE_INTERNATIONAL2 = 136,
#define     SDL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
#define     SDL_SCANCODE_INTERNATIONAL4 = 138,
#define     SDL_SCANCODE_INTERNATIONAL5 = 139,
#define     SDL_SCANCODE_INTERNATIONAL6 = 140,
#define     SDL_SCANCODE_INTERNATIONAL7 = 141,
#define     SDL_SCANCODE_INTERNATIONAL8 = 142,
#define     SDL_SCANCODE_INTERNATIONAL9 = 143,
#define     SDL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
#define     SDL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
#define     SDL_SCANCODE_LANG3 = 146, /**< Katakana */
#define     SDL_SCANCODE_LANG4 = 147, /**< Hiragana */
#define     SDL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
#define     SDL_SCANCODE_LANG6 = 149, /**< reserved */
#define     SDL_SCANCODE_LANG7 = 150, /**< reserved */
#define     SDL_SCANCODE_LANG8 = 151, /**< reserved */
#define     SDL_SCANCODE_LANG9 = 152, /**< reserved */

#define     SDL_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
#define     SDL_SCANCODE_SYSREQ = 154,
#define     SDL_SCANCODE_CANCEL = 155,
#define     SDL_SCANCODE_CLEAR = 156,
#define     SDL_SCANCODE_PRIOR = 157,
#define     SDL_SCANCODE_RETURN2 = 158,
#define     SDL_SCANCODE_SEPARATOR = 159,
#define     SDL_SCANCODE_OUT = 160,
#define     SDL_SCANCODE_OPER = 161,
#define     SDL_SCANCODE_CLEARAGAIN = 162,
#define     SDL_SCANCODE_CRSEL = 163,
#define     SDL_SCANCODE_EXSEL = 164,

#define     SDL_SCANCODE_KP_00 = 176,
#define     SDL_SCANCODE_KP_000 = 177,
#define     SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
#define     SDL_SCANCODE_DECIMALSEPARATOR = 179,
#define     SDL_SCANCODE_CURRENCYUNIT = 180,
#define     SDL_SCANCODE_CURRENCYSUBUNIT = 181,
#define     SDL_SCANCODE_KP_LEFTPAREN = 182,
#define     SDL_SCANCODE_KP_RIGHTPAREN = 183,
#define     SDL_SCANCODE_KP_LEFTBRACE = 184,
#define     SDL_SCANCODE_KP_RIGHTBRACE = 185,
#define     SDL_SCANCODE_KP_TAB = 186,
#define     SDL_SCANCODE_KP_BACKSPACE = 187,
#define     SDL_SCANCODE_KP_A = 188,
#define     SDL_SCANCODE_KP_B = 189,
#define     SDL_SCANCODE_KP_C = 190,
#define     SDL_SCANCODE_KP_D = 191,
#define     SDL_SCANCODE_KP_E = 192,
#define     SDL_SCANCODE_KP_F = 193,
#define     SDL_SCANCODE_KP_XOR = 194,
#define     SDL_SCANCODE_KP_POWER = 195,
#define     SDL_SCANCODE_KP_PERCENT = 196,
#define     SDL_SCANCODE_KP_LESS = 197,
#define     SDL_SCANCODE_KP_GREATER = 198,
#define     SDL_SCANCODE_KP_AMPERSAND = 199,
#define     SDL_SCANCODE_KP_DBLAMPERSAND = 200,
#define     SDL_SCANCODE_KP_VERTICALBAR = 201,
#define     SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
#define     SDL_SCANCODE_KP_COLON = 203,
#define     SDL_SCANCODE_KP_HASH = 204,
#define     SDL_SCANCODE_KP_SPACE = 205,
#define     SDL_SCANCODE_KP_AT = 206,
#define     SDL_SCANCODE_KP_EXCLAM = 207,
#define     SDL_SCANCODE_KP_MEMSTORE = 208,
#define     SDL_SCANCODE_KP_MEMRECALL = 209,
#define     SDL_SCANCODE_KP_MEMCLEAR = 210,
#define     SDL_SCANCODE_KP_MEMADD = 211,
#define     SDL_SCANCODE_KP_MEMSUBTRACT = 212,
#define     SDL_SCANCODE_KP_MEMMULTIPLY = 213,
#define     SDL_SCANCODE_KP_MEMDIVIDE = 214,
#define     SDL_SCANCODE_KP_PLUSMINUS = 215,
#define     SDL_SCANCODE_KP_CLEAR = 216,
#define     SDL_SCANCODE_KP_CLEARENTRY = 217,
#define     SDL_SCANCODE_KP_BINARY = 218,
#define     SDL_SCANCODE_KP_OCTAL = 219,
#define     SDL_SCANCODE_KP_DECIMAL = 220,
#define     SDL_SCANCODE_KP_HEXADECIMAL = 221,

#define     SDL_SCANCODE_LCTRL = 224,
#define     SDL_SCANCODE_LSHIFT = 225,
#define     SDL_SCANCODE_LALT = 226, /**< alt, option */
#define     SDL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
#define     SDL_SCANCODE_RCTRL = 228,
#define     SDL_SCANCODE_RSHIFT = 229,
#define     SDL_SCANCODE_RALT = 230, /**< alt gr, option */
#define     SDL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

#define     SDL_SCANCODE_MODE = 257, 
#define     SDL_SCANCODE_AUDIONEXT = 258,
#define     SDL_SCANCODE_AUDIOPREV = 259,
#define     SDL_SCANCODE_AUDIOSTOP = 260,
#define     SDL_SCANCODE_AUDIOPLAY = 261,
#define     SDL_SCANCODE_AUDIOMUTE = 262,
#define     SDL_SCANCODE_MEDIASELECT = 263,
#define     SDL_SCANCODE_WWW = 264,
#define     SDL_SCANCODE_MAIL = 265,
#define     SDL_SCANCODE_CALCULATOR = 266,
#define     SDL_SCANCODE_COMPUTER = 267,
#define     SDL_SCANCODE_AC_SEARCH = 268,
#define     SDL_SCANCODE_AC_HOME = 269,
#define     SDL_SCANCODE_AC_BACK = 270,
#define     SDL_SCANCODE_AC_FORWARD = 271,
#define     SDL_SCANCODE_AC_STOP = 272,
#define     SDL_SCANCODE_AC_REFRESH = 273,
#define     SDL_SCANCODE_AC_BOOKMARKS = 274,

#define     SDL_SCANCODE_BRIGHTNESSDOWN = 275,
#define     SDL_SCANCODE_BRIGHTNESSUP = 276,
#define     SDL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                                    switch, video mode switch */
#define     SDL_SCANCODE_KBDILLUMTOGGLE = 278,
#define     SDL_SCANCODE_KBDILLUMDOWN = 279,
#define     SDL_SCANCODE_KBDILLUMUP = 280,
#define     SDL_SCANCODE_EJECT = 281,
#define     SDL_SCANCODE_SLEEP = 282,
 
#define     SDL_SCANCODE_APP1 = 283,
#define     SDL_SCANCODE_APP2 = 284,

#define     SDL_SCANCODE_AUDIOREWIND = 285,
#define     SDL_SCANCODE_AUDIOFASTFORWARD = 286,

#define     SDL_NUM_SCANCODES = 512 