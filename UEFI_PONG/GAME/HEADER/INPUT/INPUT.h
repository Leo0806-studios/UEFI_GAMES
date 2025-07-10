#pragma once
#include <GLOBALS.h>
/// <summary>
/// enum containing the scancodes. will be expanded in the future
/// </summary>
typedef enum Keys {
	Key_F1 = 0x0b,
	Key_F2 = 0x0c,
	Key_F3 = 0x0d,
	Key_F4 = 0x0e,
	Key_F5 = 0x0f,
	Key_F6 = 0x10,
	Key_up = 0x01,
	Key_left = 0x04,
	Key_down = 0x02,
	Key_right = 0x03,
	Key_escape = 0x17
}Keys;



/// <summary>///
/// returns true if the specified key is pressed during the function.
/// tries specifies how often it checks or key input (eg igher number ==larger window for key press but longer runtime). this is not yet implemented and will allways run once
/// 
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool GetKey(Keys key,int tries);