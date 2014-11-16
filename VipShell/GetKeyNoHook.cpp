#include <windows.h>
#include ".\getkeynohook.h"

// Some Global Variables

// Lower Case Key & Some Other Keys
char *LowerCase[]={
  "b",
  "e",
  "[ESC]",
  "[F1]",
  "[F2]",
  "[F3]",
  "[F4]",
  "[F5]",
  "[F6]",
  "[F7]",
  "[F8]",
  "[F9]",
  "[F10]",
  "[F11]",
  "[F12]",
  "`",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "0",
  "-",
  "=",
  "[TAB]",
  "q",
  "w",
  "e",
  "r",
  "t",
  "y",
  "u",
  "i",
  "o",
  "p",
  "[",
  "]",
  "a",
  "s",
  "d",
  "f",
  "g",
  "h",
  "j",
  "k",
  "l",
  ";",
  "'",
  "z",
  "x",
  "c",
  "v",
  "b",
  "n",
  "m",
  ",",
  ".",
  "/",
  "\\",
  "[CTRL]",
  "[WIN]",
  " ",
  "[WIN]",
  "[Print Screen]",
  "[Scroll Lock]",
  "[Insert]",
  "[Home]",
  "[PageUp]",
  "[Del]",
  "[End]",
  "[PageDown]",
  "[Left]",
  "[UP]",
  "[Right]",
  "[Down]",
  "[Num Lock]",
  "/",
  "*",
  "-",
  "+",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  ".",
};

// Upper Case Key & Some Other Keys
char *UpperCase[]={
  "b",
  "e",
  "[ESC]",
  "[F1]",
  "[F2]",
  "[F3]",
  "[F4]",
  "[F5]",
  "[F6]",
  "[F7]",
  "[F8]",
  "[F9]",
  "[F10]",
  "[F11]",
  "[F12]",
  "~",
  "!",
  "@",
  "#",
  "$",
  "%",
  "^",
  "&",
  "*",
  "(",
  ")",
  "_",
  "+",
  "[TAB]",
  "Q",
  "W",
  "E",
  "R",
  "T",
  "Y",
  "U",
  "I",
  "O",
  "P",
  "{",
  "}",
  "A",
  "S",
  "D",
  "F",
  "G",
  "H",
  "J",
  "K",
  "L",
  ":",
  "\"",
  "Z",
  "X",
  "C",
  "V",
  "B",
  "N",
  "M",
  "<",
  ">",
  ".?",
  "©¦",
  "[CTRL]",
  "[WIN]",
  " ",
  "[WIN]",
  "[Print Screen]",
  "[Scroll Lock]",
  "[Insert]",
  "[Home]",
  "[PageUp]",
  "[Del]",
  "[End]",
  "[PageDown]",
  "[Left]",
  "[Up]",
  "[Right]",
  "[Down]",
  "[Num Lock]",
  "/",
  "*",
  "-",
  "+",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  ".",
};

// Ascii Keys,Forget About It
int SpecialKeys[]={
  8,
  13,
  27,
  112,
  113,
  114,
  115,
  116,
  117,
  118,
  119,
  120,
  121,
  122,
  123,
  192,
  49,
  50,
  51,
  52,
  53,
  54,
  55,
  56,
  57,
  48,
  189,
  187,
  9,
  81,
  87,
  69,
  82,
  84,
  89,
  85,
  73,
  79,
  80,
  219,
  221,
  65,
  83,
  68,
  70,
  71,
  72,
  74,
  75,
  76,
  186,
  222,
  90,
  88,
  67,
  86,
  66,
  78,
  77,
  188,
  190,
  191,
  220,
  17,
  91,
  32,
  92,
  44,
  145,
  45,
  36,
  33,
  46,
  35,
  34,
  37,
  38,
  39,
  40,
  144,
  111,
  106,
  109,
  107,
  96,
  97,
  98,
  99,
  100,
  101,
  102,
  103,
  104,
  105,
  110,
};


CGetKeyNoHook::CGetKeyNoHook(void)
{
}

CGetKeyNoHook::~CGetKeyNoHook(void)
{
}

int CGetKeyNoHook::GetKeyVisual(char** pRet)
{
	if(pRet)
		*pRet = NULL;
	static int bKstate[256] = {0};     // Declare The Key State Array
	static int x;
	static int shift;   // Variable To Hode State Of Shift Key
	static int state;   // Variable To Hode State Of Some Special Key Like CapsLock,Shift And ect
	for(int i=0;i<92;i++)     // Looping To Check Visual Keys
	{
		shift = GetKeyState(VK_SHIFT);   // Check Whether Shift Is Pressed
		x = SpecialKeys[ i ];   // Match The Key
		if (GetAsyncKeyState(x) & 0x8000)   // Check Combination Keys
		{
			// See Whether CapsLocak Or Shift Is Pressed
			if (((GetKeyState(VK_CAPITAL) != 0) && (shift > -1) && (x > 64) && (x < 91)))   //Caps Lock And Shift Is Not Pressed
			{
				bKstate[x] = 1;     //Uppercase Characters A-Z
			}
			else if (((GetKeyState(VK_CAPITAL) != 0) && (shift < 0) && (x > 64) && (x < 91)))   //Caps Lock And Shift Is Pressed
			{
				bKstate[x] = 2;     //Lowercase a-z
			}
			else if (shift < 0)   // Shift Is Pressed
			{
				bKstate[x] = 3;       //Uppercase Characters A-Z
			}else
				bKstate[x] = 4;     //Lowercase a-z
		}
		else
		{
			if (bKstate[x] != 0)     // No Combination Keys Detected
			{
				state = bKstate[x];   // Retrieve The Current State
				bKstate[x] = 0;   // Reset The Current State
				if (x == 8)     // Back Space Is Detected
				{
				//	KeyBuffer[strlen(KeyBuffer) - 1] = 0;   // One Key Back Then
					if(pRet)
						*pRet = "<Back>";
					return x;
				//	continue;   // Start A New Loop
				}else if (x == 13)   // Enter Is Detected
				{
					if(pRet)
						*pRet = "<Enter>";
					return x;
				}else if ((state%2) == 1)   //Must Be Upper Case Characters
				{
					if(pRet)
						*pRet = UpperCase[ i ];
					return x;
				}else if ((state%2) == 0)   // Must Be Lower Case Characters
				{
					if(pRet)
						*pRet = LowerCase[ i ];
					if( LowerCase[ i ][0] == '[' )
						return 0;
					return LowerCase[ i ][0];
				}
			}
		}
	}// End Of For Loop
	return NULL;
}