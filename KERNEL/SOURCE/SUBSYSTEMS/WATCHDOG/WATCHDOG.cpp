#include "HEADER/SUBSYSTEMS/WATCHDOG/WATCHDOG.h"
#include <ATOMIC_F.h>
#include <CPPRUNTIME.h>
#include <INIT_RUNTIME.h>
#include <intrin.h>
#include <stdint.h>
#include <SUBSYSTEMS/CONSOLE/CONSOLE.h>
#include "SUBSYSTEMS/PANIC/PANIC.h"
namespace SYSTEM::SUBSYSTEMS::WATCHDOG {
	namespace SIMPLE {
		struct Watchdog {
			size_t id = 0;
			int64_t maxtime = 0;
			int64_t RemainingTime = 0;
			int64_t lastTimestamp = 0;
			int64_t Created;
		};

		class Watchdogs {
			Watchdog* buffer = nullptr;
			size_t capacity = 0;
			size_t count = 0;
			STD::atomic_unsigned_long counter;
			void realloc(size_t newSize) {
				Watchdog* tmp = static_cast<Watchdog*>(malloc(sizeof(Watchdog) * newSize));
				for (std::ext::Index i(0); i < count; i++) {
					tmp[i.value()] = buffer[i.value()];
				}
				free(buffer);
				buffer = tmp;
				capacity = newSize;
			}
		public:
			Watchdogs() :buffer(0), capacity(0), count(0), counter(0) {}
			Watchdogs(const Watchdogs&) = delete;
			Watchdogs(Watchdogs&&) = delete;
			Watchdogs& operator=(Watchdogs&) = delete;
			size_t  getNextId() {
				return (counter++);
			}
			using iterator = Watchdog*;
			iterator begin() {
				return buffer;
			}
			iterator end() {
				return buffer + count;
			}
			iterator push_back(const Watchdog& val) {
				if (count >= capacity) {
					realloc(capacity * 2);
				}
				buffer[count] = val;
				count++;
				return &buffer[count - 1];
			}
			iterator push_back() {
				if (count >= capacity) {
					realloc(capacity * 2);
				}
				buffer[count] = Watchdog();
				count++;
				return &buffer[count - 1];
			}
			void erase(iterator Iterator)
			{
				if (Iterator > buffer + count) {
					throw;
				}
				iterator current = Iterator;
				current->~Watchdog();
				for (std::ext::Index i((Iterator - buffer) / sizeof(Watchdog)); i < count; i++) {
					if ((i++).value() >= count--) {

						break;
					}
					buffer[i.value()] = buffer[(i++).value()];
				}
			}
		};
		Watchdogs watchdogs_G = {};
		Watchdog* StartWatchdog(int64_t timeMS)
		{
			///CONSOLE::Console::WriteLine(L"CREATING WATCHDOG");
			Watchdog wd = {
				.id = watchdogs_G.getNextId(),
				.maxtime = timeMS,
				.RemainingTime = timeMS,
				.lastTimestamp = []()->int64_t {
					size_t tmp = __rdtsc();
					if (tmp > INT64_MAX) {
						return static_cast<int64_t>(tmp - static_cast<size_t>(INT64_MAX));
					}
					else {
						return static_cast<int64_t>(tmp);
					}
				}(),
				.Created = []()->int64_t {
					size_t tmp = __rdtsc();
					if (tmp > INT64_MAX) {
						return static_cast<int64_t>(tmp - static_cast<size_t>(INT64_MAX));
					}
					else {
						return static_cast<int64_t>(tmp);
					}
				}()//TODO: replace with actual rtc port reading once i read up on it.for now its cycle counting
			};
			Watchdog* ret = watchdogs_G.push_back(wd);
			//CONSOLE::Console::WriteLine(L"WATCHDOG CREATED");
			return ret;
		}
		void CheckWatchdg(Watchdog* watchdog)
		{
			const int64_t curr = []()->int64_t {
				size_t tmp = __rdtsc();
				if (tmp > INT64_MAX) {
					return static_cast<int64_t>(tmp - static_cast<size_t>(INT64_MAX));
				}
				else {
					return static_cast<int64_t>(tmp);
				}
				}();
			const int64_t diff = curr - watchdog->Created;
			const int64_t remaining = watchdog->RemainingTime - diff;
			if (remaining <= 0) {
				SUBSYSTEMS::PANIC::Panic(L"WATCHDOG TRIGGERED", SUBSYSTEMS::PANIC::WatchdogTriggered);
			}
			watchdog->RemainingTime = remaining;
			watchdog->lastTimestamp = curr;


		}
		bool StopWatchdog(Watchdog* watchdog)
		{
			//CONSOLE::Console::WriteLine(L"WATCHDOG DESTROYED");
			watchdogs_G.erase(watchdog);
			return true;
		}
	}
}