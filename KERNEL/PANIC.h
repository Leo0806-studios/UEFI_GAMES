#pragma once
namespace SYSTEM::SUBSYSTEMS::PANIC {
	enum PanicCode :int {
		PhysicalAllocFailed,
		KernelImageCorrupted,
		ImportantPointerNull,
		WatchdogTriggered,
		StringTooLong
	};
	__declspec(noreturn) void Panic(const wchar_t* msg, PanicCode errorCode);
}