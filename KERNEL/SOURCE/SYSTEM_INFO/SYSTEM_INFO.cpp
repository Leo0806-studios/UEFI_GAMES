#include "../../HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include <UTILLITY.h>
namespace SYSTEM {
	namespace SYSTEM_INFO {
		SYSTEM::STARTUP::PAGING::GlobalPageMap GlobalPageMap = {};
		SystemInfo::SystemInfo(SystemInfo&& other) {
			this->cpuCores = STD::move(other.cpuCores);
			this->installedRam = STD::move(other.installedRam);
		}
		SystemInfo& SystemInfo::operator=(SystemInfo&& other) 
		{
			this->cpuCores = STD::move(other.cpuCores);
			this->installedRam = STD::move(other.installedRam);
			return *this;
		}
	}
}