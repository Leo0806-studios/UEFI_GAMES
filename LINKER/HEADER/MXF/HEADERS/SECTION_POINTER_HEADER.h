#pragma once
#ifdef __INTELLISENSE__
import std;
#endif // __INTELLISENSE__


namespace MXF_LINKER {
	constexpr inline char SectionPointerHeaderId[4] = { 'S', 'P', 'H', 0 }; // 'SPH' in ASCII, this is the identifier for the Section Pointer Header
#pragma pack(push)
#pragma pack(1)
	struct SectionPointerHeaderEntry {
		unsigned __int64 SectionOffset; // offset from the beginning of the file to the section
		unsigned __int64 SectionSize;   // size of the section in bytes
		char SectionName[32];           // name of the section, padded with zeros
	};
#pragma pack(pop)
#pragma warning(push)
#pragma warning (disable: 4820) // Disable padding warning for this struct
	struct SectionPointerHeader {
		const int HeaderId = *reinterpret_cast<const int*>(&SectionPointerHeaderId); // 'SPH' in ASCII, this is the identifier for the Section Pointer Header
		size_t numSections = 0;//i know this is redundant with the vector but it eases parsing and makes it harder to forget the nber of setions 
		std::vector<SectionPointerHeaderEntry> SectionPointers; // this is a vector of section pointers. each pointer points to a section in the file.
		SectionPointerHeader() = default;
		SectionPointerHeader(const SectionPointerHeader& other) :
			HeaderId(other.HeaderId),
			SectionPointers(other.SectionPointers) {
		}
		SectionPointerHeader(SectionPointerHeader&& other) noexcept :
			HeaderId(other.HeaderId),
			SectionPointers(std::move(other.SectionPointers)) {
		}
		SectionPointerHeader& operator=(const SectionPointerHeader& other) {
			if (this != &other) {
				SectionPointers = other.SectionPointers;
			}
			return *this;
		}
		SectionPointerHeader& operator=(SectionPointerHeader&& other) noexcept {
			if (this != &other) {
				SectionPointers = std::move(other.SectionPointers);
			}
			return *this;
		}
		~SectionPointerHeader() = default;
};
#pragma warning(pop)
} // namespace MXF_LINKER
