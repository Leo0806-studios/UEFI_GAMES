
extern "C" {
	unsigned int Print(
		const wchar_t* fmt,
		...
	);
}
#include "STARTUP/PE_PARSER/PE_PARSER.h"
#include "STRING_F.h"
#include "SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
#include "SUBSYSTEMS/PANIC/PANIC.h"
#include "UTILLITY_F.h"
#include "Windows.h"
#include <cstring>
#define MEMCMP(a,b,length)\
[&](){\
for (size_t i = 0; i < (length); i++) {\
	\
		if (((unsigned char*)a)[i] != ((unsigned char*)b)[i]) {\
				return false; \
		}\
}\
return true;\
}()

void* MEMCPY(void* dest, void* src, size_t len) {
	for (size_t i = 0; i < len; i++) {
		reinterpret_cast<unsigned char*>(dest)[i] = reinterpret_cast<unsigned char*>(src)[i];
	}
	return dest;
}
namespace SYSTEM::STARTUP::PE_PARSER {
	struct Section {
		size_t setionStart = 0;//this is the offset in the pe file.
		size_t length = 0;//this is the size of the section
		char Name[IMAGE_SIZEOF_SHORT_NAME + 1]; // Name of the section, can be used for debugging or identification

	};
	using PhysicalAllocator = SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
	using PanicCode = SUBSYSTEMS::PANIC::PanicCode;
	struct Internals {
		void* ImageBase = nullptr;
		IMAGE_DOS_HEADER dosHeader;
		IMAGE_FILE_HEADER FileHeader;
		IMAGE_OPTIONAL_HEADER OptionalHeader;
		IMAGE_SECTION_HEADER* SectionHeaders = nullptr;//the allocated space for this will always be 4K , i page
		Section* Sections = nullptr;//the allocated space for this will always be 4k, 1 page
	};
	bool PePerser::CreateAndParse(void* imageBase)
	{
		if (!imageBase) {
			SUBSYSTEMS::PANIC::Panic(L"IMMAGE BASE IS NULL", PanicCode::ImportantPointerNull);

		}

		Internals* internals = reinterpret_cast<Internals*>(PhysicalAllocator::AllocatePage()); //im allocating one page (4K) for the parsed pe data. this should be enough and hive lots of room to spare
		if (!internals) {
			SUBSYSTEMS::PANIC::Panic(L"COULDNT ALLOCATE SPACE FOR PE TABLE", PanicCode::PhysicalAllocFailed);
		}
		internals->ImageBase = imageBase;






		// Read DOS header
		MEMCPY(&internals->dosHeader, imageBase, sizeof(IMAGE_DOS_HEADER));
		if (internals->dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
			SUBSYSTEMS::PANIC::Panic(L"KERNEL IMAGE CORRUPT", PanicCode::KernelImageCorrupted);
		}
		// Read NT headers

		auto ntHeadersOffset = internals->dosHeader.e_lfanew + 4;
		MEMCPY(&internals->FileHeader, static_cast<unsigned char*>(imageBase) + ntHeadersOffset, sizeof(IMAGE_FILE_HEADER));

		MEMCPY(&internals->OptionalHeader, static_cast<unsigned char*>(imageBase) + ntHeadersOffset + sizeof(IMAGE_FILE_HEADER), sizeof(IMAGE_OPTIONAL_HEADER));
		auto inmghed = reinterpret_cast<IMAGE_OPTIONAL_HEADER*>(static_cast<unsigned char*>(imageBase) + ntHeadersOffset + sizeof(IMAGE_FILE_HEADER));
		if (!MEMCMP(&internals->OptionalHeader, inmghed, sizeof(IMAGE_OPTIONAL_HEADER))) {
			SUBSYSTEMS::PANIC::Panic(L"PE_IMAGE_CORRUPTED", PanicCode::KernelImageCorrupted);
		}
		if (internals->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
			SUBSYSTEMS::PANIC::Panic(L"KERNEL IMAGE CORRUPTED", PanicCode::KernelImageCorrupted);
		}

		const size_t sectionCount = internals->FileHeader.NumberOfSections;
		constexpr 	size_t sectionHeaderSize = sizeof(IMAGE_SECTION_HEADER);


			for (size_t i = 0; i < sectionCount; ++i) {
				auto sectionOffset = ntHeadersOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) + i * sectionHeaderSize;
				IMAGE_SECTION_HEADER sectionHeader = {};
				Print(L"Offset %lu, size %lu \n", sectionOffset, sectionHeaderSize);
				MEMCPY(&sectionHeader, static_cast<unsigned char*>(imageBase) + sectionOffset, sectionHeaderSize);

				if (internals->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
					SUBSYSTEMS::PANIC::Panic(L"KERNEL IMAGE CORRUPTED", PanicCode::KernelImageCorrupted);
				}

				char namesection[IMAGE_SIZEOF_SHORT_NAME + 1] = {};
				for (int j = 0; j < IMAGE_SIZEOF_SHORT_NAME; ++j) {
					if (sectionHeader.Name[j] == 0) break;
					namesection[j] = static_cast<char>(sectionHeader.Name[j]);

				}




				internals->SectionHeaders = static_cast<IMAGE_SECTION_HEADER*>(PhysicalAllocator::AllocatePage());
				if (internals->SectionHeaders == nullptr) {
					SUBSYSTEMS::PANIC::Panic(L"COULDNT ALLOCATE SPACE DURING PE PARSING", PanicCode::PhysicalAllocFailed);
				}
				internals->SectionHeaders[i] = sectionHeader;
				Section sec;
				sec.length = sectionHeader.SizeOfRawData;
				sec.setionStart = sectionHeader.PointerToRawData;
				MEMCPY(&sec.Name, &namesection, sizeof((namesection)));
				internals->Sections = static_cast<Section*>(PhysicalAllocator::AllocatePage());
				if (internals->Sections == nullptr) {
					SUBSYSTEMS::PANIC::Panic(L"ALLOCATIOND FAILED DURING PARSING OF PE", PanicCode::PhysicalAllocFailed);
				}
				internals->Sections[i] = sec;
				Print(L"SECTION NAME: %a , OFFSET: %lu , SIZE: %lu\n", sec.Name, sec.setionStart, sec.length);
			}
		Print(L"internals ptr is %p \n", PePerser::internals());
		STD::ignore = PePerser::internals() = internals;
		auto a = reinterpret_cast<Internals*> (PePerser::internals());
		if (!MEMCMP(&internals->OptionalHeader, inmghed, sizeof(IMAGE_OPTIONAL_HEADER))) {
			//SUBSYSTEMS::PANIC::Panic(L"PE CONSITENCY CHECK FAILIED", PanicCode::KernelImageCorrupted);
		}
		Print(L"internals ptr is %p \n", a);
	/*	for (size_t i = 0; i < internals->FileHeader.NumberOfSections; i++) {
			Print(L"Section name %a , section offset %d\n", internals->Sections[i].Name, internals->Sections[i].setionStart);
		}*/
		return true;
	}
	size_t PePerser::GetSectionOffsetByName(const char* name)
	{
		const size_t len = STD::strlen(name);
		if (len > IMAGE_SIZEOF_SHORT_NAME + 1) {
			SUBSYSTEMS::PANIC::Panic(L"STRING TOO LONG", PanicCode::StringTooLong);
		}
		Internals* internals = static_cast<Internals*>(SYSTEM::STARTUP::PE_PARSER::PePerser::internals());
		if (internals == nullptr) {
			SUBSYSTEMS::PANIC::Panic(L"PE PARSER INTERNALS NULL", PanicCode::ImportantPointerNull);
		}
		size_t sectionStart = 0;

		for (size_t i = 0; i < internals->FileHeader.NumberOfSections; i++) {
			if (MEMCMP(internals->SectionHeaders[i].Name, name, (IMAGE_SIZEOF_SHORT_NAME))) {
				sectionStart = (internals->SectionHeaders[i].PointerToRawData);
				Print(L"FOUND SECTION %a AT OFFSET %lu\n", internals->Sections[i].Name, sectionStart);
				break;
			}
		}
		if (sectionStart == 0) {
			//SUBSYSTEMS::PANIC::Panic(L"PE DATA CORRUPTED", PanicCode::KernelImageCorrupted);
		}
		return sectionStart;
	}
}