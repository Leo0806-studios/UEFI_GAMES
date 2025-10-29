#pragma once
namespace SYSTEM {
	namespace STARTUP {
		/// <summary>
		/// this namespace contains alle the functionality related to parsing the kernel PE image. it is used to seperate any functionality that requires Windows.h (and similar windows files) from the rest of the kernel
		/// </summary>
		namespace PE_PARSER {
			
			class PePerser {
				[[nodiscard("nodiscarding the return of this function is almost alwas an error since its main purpose is to provide accces to a datamember")]] static void* internals(void* ptr = nullptr) {
					static void* internals;
					[[unlikely]]if (ptr) {
						internals = ptr;
					}
					return internals; }
			public:
				[[nodiscard("discarding the ruturn value can leave the system in an unstable state on failiure")]] static bool CreateAndParse(void* imageBase);
				static size_t GetSectionOffsetByName(const char* name);
			};
		}
	}
}