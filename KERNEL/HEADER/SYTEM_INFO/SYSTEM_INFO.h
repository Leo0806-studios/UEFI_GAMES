#pragma once
#include "../STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "../STARTUP/GDT/GDT.h"
namespace SYSTEM {
	namespace SYSTEM_INFO {
		extern SYSTEM::STARTUP::PAGING::GlobalPageMap GlobalPageMap;
		class SystemInfo {
			size_t installedRam = 0;
			size_t cpuCores = 0;
			SystemInfo(const SystemInfo& other) = delete;
			SystemInfo& operator=(const SystemInfo& other)=delete;
			SystemInfo(SystemInfo&& other);
			SystemInfo& operator=(SystemInfo&& other);
		};
	}
}