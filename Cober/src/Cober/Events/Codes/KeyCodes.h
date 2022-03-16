#pragma once

// From SDL.h
// SDL_Scancode
#define     CB_KEY_A = 4,
#define     CB_KEY_B = 5,
#define     CB_KEY_C = 6,
#define     CB_KEY_D = 7,
#define     CB_KEY_E = 8,
#define     CB_KEY_F = 9,
#define     CB_KEY_G = 10,
#define     CB_KEY_H = 11,
#define     CB_KEY_I = 12,
#define     CB_KEY_J = 13,
#define     CB_KEY_K = 14,
#define     CB_KEY_L = 15,
#define     CB_KEY_M = 16,
#define     CB_KEY_N = 17,
#define     CB_KEY_O = 18,
#define     CB_KEY_P = 19,
#define     CB_KEY_Q = 20,
#define     CB_KEY_R = 21,
#define     CB_KEY_S = 22,
#define     CB_KEY_T = 23,
#define     CB_KEY_U = 24,
#define     CB_KEY_V = 25,
#define     CB_KEY_W = 26,
#define     CB_KEY_X = 27,
#define     CB_KEY_Y = 28,
#define     CB_KEY_Z = 29,
            
#define     CB_KEY_1 = 30,
#define     CB_KEY_2 = 31,
#define     CB_KEY_3 = 32,
#define     CB_KEY_4 = 33,
#define     CB_KEY_5 = 34,
#define     CB_KEY_6 = 35,
#define     CB_KEY_7 = 36,
#define     CB_KEY_8 = 37,
#define     CB_KEY_9 = 38,
#define     CB_KEY_0 = 39,
            
#define     CB_KEY_RETURN = 40,
#define     CB_KEY_ESCAPE = 41,
#define     CB_KEY_BACKSPACE = 42,
#define     CB_KEY_TAB = 43,
#define     CB_KEY_SPACE = 44,
            
#define     CB_KEY_MINUS = 45,
#define     CB_KEY_EQUALS = 46,
#define     CB_KEY_LEFTBRACKET = 47,
#define     CB_KEY_RIGHTBRACKET = 48,
#define     CB_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
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
#define     CB_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
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
#define     CB_KEY_SEMICOLON = 51,
#define     CB_KEY_APOSTROPHE = 52,
#define     CB_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
#define     CB_KEY_COMMA = 54,
#define     CB_KEY_PERIOD = 55,
#define     CB_KEY_SLASH = 56,
            
#define     CB_KEY_CAPSLOCK = 57,
            
#define     CB_KEY_F1 = 58,
#define     CB_KEY_F2 = 59,
#define     CB_KEY_F3 = 60,
#define     CB_KEY_F4 = 61,
#define     CB_KEY_F5 = 62,
#define     CB_KEY_F6 = 63,
#define     CB_KEY_F7 = 64,
#define     CB_KEY_F8 = 65,
#define     CB_KEY_F9 = 66,
#define     CB_KEY_F10 = 67,
#define     CB_KEY_F11 = 68,
#define     CB_KEY_F12 = 69,
            
#define     CB_KEY_PRINTSCREEN = 70,
#define     CB_KEY_SCROLLLOCK = 71,
#define     CB_KEY_PAUSE = 72,
#define     CB_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                      does send code 73, not 117) */
#define     CB_KEY_HOME = 74,
#define     CB_KEY_PAGEUP = 75,
#define     CB_KEY_DELETE = 76,
#define     CB_KEY_END = 77,
#define     CB_KEY_PAGEDOWN = 78,
#define     CB_KEY_RIGHT = 79,
#define     CB_KEY_LEFT = 80,
#define     CB_KEY_DOWN = 81,
#define     CB_KEY_UP = 82,
            
#define     CB_KEY_NUMLOCKCLEAR = 83, /*< num lock on PC, clear on Mac keyboards */

#define     CB_KEY_KP_DIVIDE = 84,
#define     CB_KEY_KP_MULTIPLY = 85,
#define     CB_KEY_KP_MINUS = 86,
#define     CB_KEY_KP_PLUS = 87,
#define     CB_KEY_KP_ENTER = 88,
#define     CB_KEY_KP_1 = 89,
#define     CB_KEY_KP_2 = 90,
#define     CB_KEY_KP_3 = 91,
#define     CB_KEY_KP_4 = 92,
#define     CB_KEY_KP_5 = 93,
#define     CB_KEY_KP_6 = 94,
#define     CB_KEY_KP_7 = 95,
#define     CB_KEY_KP_8 = 96,
#define     CB_KEY_KP_9 = 97,
#define     CB_KEY_KP_0 = 98,
#define     CB_KEY_KP_PERIOD = 99,
            
#define     CB_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                           *   keyboards have over ANSI ones,
                                           *   located between left shift and Y.
                                           *   Produces GRAVE ACCENT and TILDE in a
                                           *   US or UK Mac layout, REVERSE SOLIDUS
                                           *   (backslash) and VERTICAL LINE in a
                                           *   US or UK Windows layout, and
                                           *   LESS-THAN SIGN and GREATER-THAN SIGN
                                           *   in a Swiss German, German, or French
                                           *   layout. */
#define     CB_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
#define     CB_KEY_POWER = 102, /**< The USB document says this is a status flag,
                                  *   not a physical key - but some Mac keyboards
                                  *   do have a power key. */
#define     CB_KEY_KP_EQUALS = 103,
#define     CB_KEY_F13 = 104,
#define     CB_KEY_F14 = 105,
#define     CB_KEY_F15 = 106,
#define     CB_KEY_F16 = 107,
#define     CB_KEY_F17 = 108,
#define     CB_KEY_F18 = 109,
#define     CB_KEY_F19 = 110,
#define     CB_KEY_F20 = 111,
#define     CB_KEY_F21 = 112,
#define     CB_KEY_F22 = 113,
#define     CB_KEY_F23 = 114,
#define     CB_KEY_F24 = 115,
#define     CB_KEY_EXECUTE = 116,
#define     CB_KEY_HELP = 117,
#define     CB_KEY_MENU = 118,
#define     CB_KEY_SELECT = 119,
#define     CB_KEY_STOP = 120,
#define     CB_KEY_AGAIN = 121,   /**< redo */
#define     CB_KEY_UNDO = 122,
#define     CB_KEY_CUT = 123,
#define     CB_KEY_COPY = 124,
#define     CB_KEY_PASTE = 125,
#define     CB_KEY_FIND = 126,
#define     CB_KEY_MUTE = 127,
#define     CB_KEY_VOLUMEUP = 128,
#define     CB_KEY_VOLUMEDOWN = 129,
            
#define     CB_KEY_KP_COMMA = 133,
#define     CB_KEY_KP_EQUALSAS400 = 134,
            
#define     CB_KEY_ALTERASE = 153, /**< Erase-Eaze */
#define     CB_KEY_SYSREQ = 154,
#define     CB_KEY_CANCEL = 155,
#define     CB_KEY_CLEAR = 156,
#define     CB_KEY_PRIOR = 157,
#define     CB_KEY_RETURN2 = 158,
#define     CB_KEY_SEPARATOR = 159,
#define     CB_KEY_OUT = 160,
#define     CB_KEY_OPER = 161,
#define     CB_KEY_CLEARAGAIN = 162,
#define     CB_KEY_CRSEL = 163,
#define     CB_KEY_EXSEL = 164,
            
#define     CB_KEY_KP_00 = 176,
#define     CB_KEY_KP_000 = 177,
#define     CB_KEY_THOUSANDSSEPARATOR = 178,
#define     CB_KEY_DECIMALSEPARATOR = 179,
#define     CB_KEY_CURRENCYUNIT = 180,
#define     CB_KEY_CURRENCYSUBUNIT = 181,
#define     CB_KEY_KP_LEFTPAREN = 182,
#define     CB_KEY_KP_RIGHTPAREN = 183,
#define     CB_KEY_KP_LEFTBRACE = 184,
#define     CB_KEY_KP_RIGHTBRACE = 185,
#define     CB_KEY_KP_TAB = 186,
#define     CB_KEY_KP_BACKSPACE = 187,
#define     CB_KEY_KP_A = 188,
#define     CB_KEY_KP_B = 189,
#define     CB_KEY_KP_C = 190,
#define     CB_KEY_KP_D = 191,
#define     CB_KEY_KP_E = 192,
#define     CB_KEY_KP_F = 193,
#define     CB_KEY_KP_XOR = 194,
#define     CB_KEY_KP_POWER = 195,
#define     CB_KEY_KP_PERCENT = 196,
#define     CB_KEY_KP_LESS = 197,
#define     CB_KEY_KP_GREATER = 198,
#define     CB_KEY_KP_AMPERSAND = 199,
#define     CB_KEY_KP_DBLAMPERSAND = 200,
#define     CB_KEY_KP_VERTICALBAR = 201,
#define     CB_KEY_KP_DBLVERTICALBAR = 202,
#define     CB_KEY_KP_COLON = 203,
#define     CB_KEY_KP_HASH = 204,
#define     CB_KEY_KP_SPACE = 205,
#define     CB_KEY_KP_AT = 206,
#define     CB_KEY_KP_EXCLAM = 207,
#define     CB_KEY_KP_MEMSTORE = 208,
#define     CB_KEY_KP_MEMRECALL = 209,
#define     CB_KEY_KP_MEMCLEAR = 210,
#define     CB_KEY_KP_MEMADD = 211,
#define     CB_KEY_KP_MEMSUBTRACT = 212,
#define     CB_KEY_KP_MEMMULTIPLY = 213,
#define     CB_KEY_KP_MEMDIVIDE = 214,
#define     CB_KEY_KP_PLUSMINUS = 215,
#define     CB_KEY_KP_CLEAR = 216,
#define     CB_KEY_KP_CLEARENTRY = 217,
#define     CB_KEY_KP_BINARY = 218,
#define     CB_KEY_KP_OCTAL = 219,
#define     CB_KEY_KP_DECIMAL = 220,
#define     CB_KEY_KP_HEXADECIMAL = 221,
            
#define     CB_KEY_LCTRL = 224,
#define     CB_KEY_LSHIFT = 225,
#define     CB_KEY_LALT = 226, /**< alt, option */
#define     CB_KEY_LGUI = 227, /**< windows, command (apple), meta */
#define     CB_KEY_RCTRL = 228,
#define     CB_KEY_RSHIFT = 229,
#define     CB_KEY_RALT = 230, /**< alt gr, option */
#define     CB_KEY_RGUI = 231, /**< windows, command (apple), meta */
            
#define     CB_KEY_MODE = 257, 
#define     CB_KEY_AUDIONEXT = 258,
#define     CB_KEY_AUDIOPREV = 259,
#define     CB_KEY_AUDIOSTOP = 260,
#define     CB_KEY_AUDIOPLAY = 261,
#define     CB_KEY_AUDIOMUTE = 262,
#define     CB_KEY_MEDIASELECT = 263,
#define     CB_KEY_WWW = 264,
#define     CB_KEY_MAIL = 265,
#define     CB_KEY_CALCULATOR = 266,
#define     CB_KEY_COMPUTER = 267,
#define     CB_KEY_AC_SEARCH = 268,
#define     CB_KEY_AC_HOME = 269,
#define     CB_KEY_AC_BACK = 270,
#define     CB_KEY_AC_FORWARD = 271,
#define     CB_KEY_AC_STOP = 272,
#define     CB_KEY_AC_REFRESH = 273,
#define     CB_KEY_AC_BOOKMARKS = 274,
            
#define     CB_KEY_BRIGHTNESSDOWN = 275,
#define     CB_KEY_BRIGHTNESSUP = 276,
#define     CB_KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                             switch, video mode switch */
#define     CB_KEY_KBDILLUMTOGGLE = 278,
#define     CB_KEY_KBDILLUMDOWN = 279,
#define     CB_KEY_KBDILLUMUP = 280,
#define     CB_KEY_EJECT = 281,
#define     CB_KEY_SLEEP = 282,
            
#define     CB_KEY_APP1 = 283,
#define     CB_KEY_APP2 = 284,
            
#define     CB_KEY_AUDIOREWIND = 285,
#define     CB_KEY_AUDIOFASTFORWARD = 286,