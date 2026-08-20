#pragma once
#include "SAL_F.h"
namespace SYSTEM::SUBSYSTEMS::WATCHDOG {
	/// <summary>
	/// one onavidable error case with the simple watchdog is that if the counter overflows between to checks the watchdog will not trigger.
	/// howeversince the counter is int64_t internaly this shoud usualy not happen too quickly (at 3.6 ghz overflow should only occur every 81 yeras)
	/// </summary>
	namespace SIMPLE {
		 struct Watchdog;
		/// <summary>
		/// Starts the non preemtive watchdog. will returne nullptr  on failiure.
		/// another runing watchdog counts as a failiure.
		/// sets teh max allowed Cycles to cycles.
		/// use only after C++ runtime startup
		/// </summary>
		/// <param name="timeMS"></param>
		/// <returns></returns>
		 NODISCARD_MSG("discarding the return value can lead to non functional watchdogs and corupt internal state") Watchdog* StartWatchdog(__int64 Cycles);

		 /// <summary>
		 /// checks if the previousely started watchdog is still within the allowed Cycle bufget. will not return controll if past the cycle  limit.
		 /// /// use only after C++ runtime startup
		 /// </summary>
		 void CheckWatchdg(Watchdog* watchdog);

		 /// <summary>
		 /// Stops the current watchdog.
		 /// will return falso on failiure
		 /// no runing watchdog counts as a failiure
		 /// /// use only after C++ runtime startup
		 /// </summary>
		 /// <returns></returns>
		NODISCARD_MSG("discarding the returnvalue can lead to non functional watchdogs and corrupt internal state") bool StopWatchdog(Watchdog* watchdog);
	}
}