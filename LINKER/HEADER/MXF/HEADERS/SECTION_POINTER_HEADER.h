#pragma once
import std;
 constexpr int MakeFourCC(char a, char b, char c, char d) ;



namespace MXF_LINKER {
#pragma pack(push)
#pragma pack(1)
	struct SectionPointerHeaderEntry {
		unsigned __int64 SectionOffset; // offset from the beginning of the file to the section
		unsigned __int64 SectionSize;   // size of the section in bytes
		char SectionName[32] = {};           // name of the section, padded with zeros
	};
#pragma pack(pop)
#pragma warning(push)
#pragma warning (disable: 4820) // Disable padding warning for this struct
	struct SectionPointerHeader {
		const int HeaderId = MakeFourCC('S', 'P', 'H', 0); // 'SPH' in ASCII, this is the identifier for the Section Pointer Header
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
		std::vector<unsigned __int8> ToByteArray()  {
			std::vector<unsigned __int8> bytes;
			//bytes.reserve(sizeof(SectionPointerHeader) + SectionPointers.size() * sizeof(SectionPointerHeaderEntry) + sizeof(numSections)+sizeof(HeaderId)); // Reserve space for the header and pointers
			//append the header id
			bytes.push_back(reinterpret_cast<const unsigned char*>(&HeaderId)[0]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&HeaderId)[1]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&HeaderId)[2]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&HeaderId)[3]);
			//just in case the numsections is not assigned yet well assume all the sections are allready compleatly assigned and re count. will stopp at the firs "null" entry
			size_t numSections_ = 0;
			for (const auto& entry : SectionPointers) {
				if (entry.SectionOffset == 0 && entry.SectionSize == 0) {
					break; // Stop counting when we hit an empty entry
				}
				numSections_++;
			}
			this->numSections = numSections_;
			//now that we can sure num sections is correct we can
			//append the number of sections
			bytes.push_back(reinterpret_cast<const unsigned char*>(&numSections)[0]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&numSections)[1]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&numSections)[2]);
			bytes.push_back(reinterpret_cast<const unsigned char*>(&numSections)[3]);

			//now we append the section pointers
			//as i cant be sure its packed i will use a loop to append each entry
			for (const auto& entry : SectionPointers) {
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionOffset)[0]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionOffset)[1]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionOffset)[2]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionOffset)[3]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionSize)[0]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionSize)[1]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionSize)[2]);
				bytes.push_back(reinterpret_cast<const unsigned char*>(&entry.SectionSize)[3]);
				//now append the section name
				for (size_t i = 0; i < sizeof(entry.SectionName); ++i) {
					bytes.push_back(static_cast<unsigned __int8>(entry.SectionName[i]));
				}
			}

			return bytes;
		}
};
#pragma warning(pop)
} // namespace MXF_LINKER
