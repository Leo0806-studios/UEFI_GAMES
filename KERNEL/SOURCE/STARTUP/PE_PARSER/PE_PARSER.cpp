#include "STARTUP/PE_PARSER/PE_PARSER.h"
#include "SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
#include "SUBSYSTEMS/PANIC/PANIC.h"
#include <cstring>
#include "Windows.h"
#include "UTILLITY_F.h"
#include "STRING_F.h"
namespace SYSTEM::STARTUP::PE_PARSER {
	struct Section {
		size_t setionStart = 0;//this is the offset in the pe file.
		size_t length = 0;//this is the size of the section
		char Name[IMAGE_SIZEOF_SHORT_NAME+1]; // Name of the section, can be used for debugging or identification
		
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
		memcpy(&internals->dosHeader, imageBase, sizeof(IMAGE_DOS_HEADER));
		if (internals->dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
			SUBSYSTEMS::PANIC::Panic(L"KERNEL IMAGE CORRUPT", PanicCode::KernelImageCorrupted);
		}
		// Read NT headers

		auto ntHeadersOffset = internals->dosHeader.e_lfanew + 4;
		memcpy(&internals->FileHeader, static_cast<unsigned char*>(imageBase)+ntHeadersOffset, sizeof(IMAGE_FILE_HEADER));

		memcpy(&internals->OptionalHeader, static_cast<unsigned char*>(imageBase) + ntHeadersOffset + sizeof(IMAGE_FILE_HEADER), sizeof(IMAGE_OPTIONAL_HEADER));

			if (internals->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
				SUBSYSTEMS::PANIC::Panic(L"KERNEL IMAGE CORRUPTED",PanicCode::KernelImageCorrupted);
			}
	
		size_t sectionCount =internals-> FileHeader.NumberOfSections;
		size_t sectionSize = sizeof(IMAGE_SECTION_HEADER);


		for (size_t i = 0; i < sectionCount; ++i) {
			auto sectionOffset = ntHeadersOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) + i * sectionSize;
			IMAGE_SECTION_HEADER section = {};
			memcpy(&section, static_cast<unsigned char*>(imageBase) + sectionOffset, sectionSize);
			
			char namesection[IMAGE_SIZEOF_SHORT_NAME+1] = {};
			for (int j = 0; j < IMAGE_SIZEOF_SHORT_NAME; ++j) {
				if (section.Name[j] == 0) break;
				namesection[j] = static_cast<char>(section.Name[j]);

			}





			internals->SectionHeaders = static_cast<IMAGE_SECTION_HEADER*>(PhysicalAllocator::AllocatePage());
			if (internals->SectionHeaders == nullptr) {
				SUBSYSTEMS::PANIC::Panic(L"COULDNT ALLOCATE SPACE DURING PE PARSING", PanicCode::PhysicalAllocFailed);
			}
			internals->SectionHeaders[i]=section;
			Section sec;
			sec.length = section.SizeOfRawData;
			sec.setionStart = section.PointerToRawData;
			memcpy(&sec.Name, &namesection, sizeof((namesection)));
			internals->Sections = static_cast<Section*>(PhysicalAllocator::AllocatePage());
			if (internals->Sections == nullptr) {
				SUBSYSTEMS::PANIC::Panic(L"ALLOCATIOND FAILED DURING PARSING OF PE", PanicCode::PhysicalAllocFailed);
			}
			internals->Sections[i] = sec;
			STD::ignore=PePerser::internals(internals);

		}
		return false;
	}
	size_t PePerser::GetSectionOffsetByName(const char* name)
	{
		const size_t len =STD::strlen(name);
		if (len > IMAGE_SIZEOF_SHORT_NAME + 1) {
			SUBSYSTEMS::PANIC::Panic(L"STRING TOO LONG", PanicCode::StringTooLong);
		}
		Internals* internals = static_cast<Internals*>(SYSTEM::STARTUP::PE_PARSER::PePerser::internals());
		if (internals == nullptr) {
			SUBSYSTEMS::PANIC::Panic(L"PE PARSER INTERNALS NULL", PanicCode::ImportantPointerNull);
		}
		size_t sectionStart = 0;
		for (size_t i = 0; i < internals->FileHeader.NumberOfSections; i++) {
			if (memcmp(internals->Sections[i].Name, name, (IMAGE_SIZEOF_SHORT_NAME + 1))){
				sectionStart=( internals->Sections[i].setionStart);
				break;
			}
		}
		if (sectionStart == 0) {
			SUBSYSTEMS::PANIC::Panic(L"PE DATA CORRUPTED", PanicCode::KernelImageCorrupted);
		}
		return (sectionStart);
	}
}