#include "SUBSYSTEMS/PANIC/PANIC.h"
#include "SUBSYSTEMS/RENDER/RENDER.h"
#include "SUBSYSTEMS/CONSOLE/CONSOLE.h"
#include "STRING_F.h"
namespace SYSTEM::SUBSYSTEMS::PANIC {
	void Panic(const wchar_t* msg, PanicCode errorCode)
	{
		SUBSYSTEMS::RENDER::SIMPLE::SimpleFillScreen(0xffff0000);
		const size_t len_in =[&](){
			 size_t len_ = 0;
			for (; msg[len_]; len_++) {}
			return len_;
			}();
		const wchar_t* TITLE = L"THE SYSTEM CRASHED";
		const wchar_t* subtile = L"PLEASE RESTART";
		size_t len_title = [&]() {
			size_t len_ = 0;
			for (; TITLE[len_]; len_++) {}
			return len_;
			}();

		size_t len_subtitle = [&]() {
			size_t len_ = 0;
			for (; subtile[len_]; len_++) {}
			return len_;
			}();


		unsigned int xpos=0 ;
		unsigned int ypos = 50;
		for (size_t i = 0; i < len_title; i++) {
			if (TITLE[i] == ' ') { //-V3539
				xpos += static_cast<unsigned int>(8); //-V127


				continue;
			}
			RENDER::SIMPLE::SimpleDrawChar(xpos, ypos, TITLE[i], RENDER::COLOURS::Black, RENDER::COLOURS::White); //-V3539
			xpos += static_cast<unsigned int>(8); //-V127

		}

		xpos = 0;
		ypos = 70;

		for (size_t i = 0; i < len_subtitle; i++) {
			if (subtile[i] == ' ') { //-V3539
				xpos += static_cast<unsigned int>(8); //-V127


				continue;
			}
			RENDER::SIMPLE::SimpleDrawChar(xpos, ypos, subtile[i], RENDER::COLOURS::Black, RENDER::COLOURS::White); //-V3539
			xpos += static_cast<unsigned int>(8); //-V127

		}
		xpos = 0;
		ypos = 200;


		for (size_t i = 0; i < len_in; i++) {
			if (msg[i] == ' ') { //-V3539
				xpos += static_cast<unsigned int>(8); //-V127


				continue;
			}
			RENDER::SIMPLE::SimpleDrawChar(xpos, ypos, msg[i], RENDER::COLOURS::Black, RENDER::COLOURS::White); //-V3539
			xpos += static_cast<unsigned int>(8); //-V127

		}
		while (true)
		{

		}
	}
}