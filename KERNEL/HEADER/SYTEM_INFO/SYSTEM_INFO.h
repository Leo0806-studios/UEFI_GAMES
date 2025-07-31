#pragma once
#include "../STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "../STARTUP/GDT/GDT.h"
namespace SYSTEM {
	/// <summary>
	/// this namespace contains system information that is collected during the boot process.
	/// </summary>
	namespace SYSTEM_INFO {
		/// <summary>
		/// the global page map. this contains information about the usage of physical pages
		/// </summary>
		extern SYSTEM::STARTUP::PAGING::GlobalPageMap GlobalPageMap;


		extern SYSTEM::STARTUP::GDT::GDT GlobalGDT;
		/// <summary>
		/// system info is a global singleton that contains information about the system
		/// </summary>
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
			~SystemInfo() = default;
			/// <summary>
			/// Initializes the system info. it is immutable after this point.
			/// don't call any of the functions in this class before this function is called.
			/// </summary>
			static void InitSystemInfo();
			/// <summary>
			/// returns the global instance of the system info
			/// </summary>
			/// <returns></returns>
			static const SystemInfo& GetInstance();

		};
	}
}