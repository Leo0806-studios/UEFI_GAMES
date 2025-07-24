#pragma once
import std;
constexpr int MakeFourCC(char a, char b, char c, char d);

namespace MXF_LINKER {
#pragma pack(push)
#pragma pack(1)
	struct RelocationHeaderEntry {
		unsigned __int64 Address=0; // address of the relocation
		unsigned __int64 Size=0;    // size of the relocation in bytes
		unsigned int Type = 0;        // type of the relocation //0 = ignore, 1=relative, 2=absolute, 3=section relative, 4=section relative with offset
	};
#pragma pack(pop)
#pragma warning(push)
#pragma warning (disable: 4820) // Disable padding warning for this struct
	struct RelocationHeader {
		const int HeaderId = MakeFourCC( 'R', 'H', 'D', 0 ); // 'RHD' in ASCII, this is the identifier for the Relocation Header
		size_t numEntries = 0;//i know this is redundant eith the vector but it makes parsing easier and makes it harder to forgett storing the isze
		std::vector<RelocationHeaderEntry> Relocations; // this is a vector of relocations. each relocation contains the address, size and type of the relocation.

		RelocationHeader() = default;
		RelocationHeader(const RelocationHeader& other) :
			HeaderId(other.HeaderId),
			Relocations(other.Relocations) {
		}
		RelocationHeader(RelocationHeader&& other) noexcept :
			HeaderId(other.HeaderId),
			Relocations(std::move(other.Relocations)) {
		}
		RelocationHeader& operator=(const RelocationHeader& other) {
			if (this != &other) {
				Relocations = other.Relocations;
			}
			return *this;
		}
		RelocationHeader& operator=(RelocationHeader&& other) noexcept {
			if (this != &other) {
				Relocations = std::move(other.Relocations);
			}
			return *this;
		}
		~RelocationHeader() = default;
	};
#pragma warning(pop)
}