// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma warning(disable:26439)
#include "../../HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include <HEADER/UTILLITY/UTILLITY_F.h>
namespace SYSTEM {
	namespace SYSTEM_INFO {
		extern size_t QueryCPUFrequency();
		SYSTEM::STARTUP::PAGING::GlobalPageMap GlobalPageMap = {};
		SYSTEM::STARTUP::GDT::GDT GlobalGDT = {};
		SystemInfo SystemInfo::GLobalInst = SystemInfo();
		SystemInfo::SystemInfo(SystemInfo&& other)  {
			this->cpuCores = STD::move(other.cpuCores);
			this->installedRam = STD::move(other.installedRam);
		}
		SystemInfo& SystemInfo::operator=(SystemInfo&& other)
		{
			this->cpuCores = STD::move(other.cpuCores);
			this->installedRam = STD::move(other.installedRam);
			return *this;
		}
		 void SystemInfo::InitSystemInfo()
		{
			GLobalInst = SystemInfo();
			GLobalInst.installedRam = SYSTEM::STARTUP::PAGING::QueryInstalledRam();

		}

		  const SystemInfo& SystemInfo::GetInstance() {
			 return GLobalInst;
		 }


	
	

		  size_t QueryCPUFrequency()
		  {

			  return size_t();
		  }

	}
}