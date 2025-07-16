#pragma once
#include "../STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "../STARTUP/GDT/GDT.h"
namespace SYSTEM {
	namespace SYSTEM_INFO {
		extern SYSTEM::STARTUP::PAGING::GlobalPageMap GlobalPageMap;
		class SystemInfo {
		public:
			size_t installedRam = 0;
			size_t cpuCores = 0;
		private:
			SystemInfo(const SystemInfo& other) = delete;
			SystemInfo& operator=(const SystemInfo& other)=delete;
			SystemInfo(SystemInfo&& other);
			SystemInfo& operator=(SystemInfo&& other);
			SystemInfo() = default;
			static SystemInfo GLobalInst;
		public:
			static void InitSystemInfo();
			static const SystemInfo& GetInstance();

		};
	}
}