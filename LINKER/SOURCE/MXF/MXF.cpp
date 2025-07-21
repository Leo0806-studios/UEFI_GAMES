import <Windows.h>;
#ifdef __INTELLISENSE__
#include "MXF/MXF.h"

import std;
#include "MXF/HEADERS/HEADERS.h";
#else
import MXF;
import std;
import HEADERS;
#endif // __INTELLISENSE__


namespace MXF_LINKER {
#pragma warning(push)
#pragma warning(disable:4820)
	struct PeRelocBlock {
		struct PeRelocEntry {
			std::bitset<4> type = 0;
			std::bitset<12> offset = 0;
			std::string section = ""; //contains the sction name
			size_t AdjustedRVA = 0;//this is the adjusted adress. it is relative to the beginning of the section it is in
		};
		unsigned int pageRVA = 0;
		std::vector<PeRelocEntry> entries;
	};

#pragma warning(pop)
	MXF::MXF(const PE& Pe)
	{
		std::cout << "[MXF::MXF] Constructing MXF from PE." << std::endl;
		size_t totalHeaderSize = 0;
		this->HeaderBitmap = 0b111;//for now i will only use the first 3 headers. the others are not needed yet.
		this->FileType = 1;// this is the normal mxf file type. for now it is generic for normal executables and shared libraries.
		//ill need the following headers:
		// 1. main header
		// 2. section pointer header
		// 3. relocation header
		// the others are currently not needed.
		//

		// here ill set up the section pointer header. the pointers to the sections will be filled later
		std::cout << "[MXF::MXF] Setting up SectionPointerHeader." << std::endl;
		MXF_LINKER::SectionPointerHeader sectionPointerHeader;
		totalHeaderSize += sizeof(MXF_LINKER::SectionPointerHeader);
		sectionPointerHeader.SectionPointers.resize(Pe.FileHeader.NumberOfSections);
		for (size_t i = 0; i < Pe.FileHeader.NumberOfSections; ++i) {
			std::cout << "[MXF::MXF] Adding section pointer for section " << i << std::endl;

			const auto& section = Pe.SectionHeaders[i];
			MXF_LINKER::SectionPointerHeaderEntry entry = {};
			entry.SectionOffset = section.PointerToRawData;
			entry.SectionSize = section.SizeOfRawData;
			memset(entry.SectionName, 0, sizeof(entry.SectionName)); // Initialize the section name to zero
			std::memcpy(entry.SectionName, section.Name, sizeof(section.Name));
			sectionPointerHeader.SectionPointers[i] = entry;
			totalHeaderSize += sizeof(MXF_LINKER::SectionPointerHeaderEntry);
			sectionPointerHeader.numSections++;
		}
		MXF_LINKER::MainHeader mainHeader = {};
		totalHeaderSize += sizeof(MXF_LINKER::MainHeader);
		mainHeader.MaxOsVersion = 0b100000000;
		mainHeader.minOsVersion = 0;
		// addres of entry point will be filled in later;

		//now ill do the relocation header.
		RelocationHeader relocationHeader = {};
		totalHeaderSize += sizeof(RelocationHeader);
		//now ill parse the relocation header form the pe file to get the count of relocations 
		size_t relocationCount = 0;
		std::vector<RelocationHeaderEntry> relocations;
		//implement Relocation parsing here
		PE::Section relocationSection = {};
		for (const auto& section : Pe.Sections) {
			if (section.Name == ".reloc") {
				relocationSection = section;
				std::cout << "[MXF::MXF] Found relocation section." << std::endl;
				break;
			}
		}
		if (relocationSection.Data.empty()) {
			std::cout << "[MXF::MXF] No relocation section found in PE." << std::endl;
			return; // No relocations to process
		}
		// Parse the relocation section
		std::vector<PeRelocBlock> peBlocks;
		const unsigned char* relocData = relocationSection.Data.data();
		size_t relocDataSize = relocationSection.Data.size();
		size_t offset = 0; // Current offset in the relocation data
		while (offset < relocDataSize) {
			PeRelocBlock PeBlock = {};
			if (relocDataSize - offset < sizeof(IMAGE_BASE_RELOCATION)) {
				std::cerr << "[MXF::MXF] Incomplete relocation entry at offset " << std::hex << offset << std::endl;
				break; // Not enough data for a complete relocation entry
			}
			const IMAGE_BASE_RELOCATION* relocEntry = reinterpret_cast<const IMAGE_BASE_RELOCATION*>(relocData + offset);
			size_t relocSize = relocEntry->SizeOfBlock;
			if (relocSize < sizeof(IMAGE_BASE_RELOCATION)) {
				if (relocSize == 0) {
					std::cout << "[MXF::MXF] Reached end of relocation entries at offset " << std::hex << offset << std::endl;
					break; // End of relocation entries
				}
				std::cerr << "[MXF::MXF] Invalid relocation block size at offset " << std::hex << offset << std::endl;
				break; // Invalid relocation block size
			}
			PeBlock.pageRVA = relocEntry->VirtualAddress;
			// Process the relocation entries
			size_t numRelocs = (relocSize - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			for (size_t i = 0; i < numRelocs; ++i) {
				WORD relocTypeAndOffset = *reinterpret_cast<const WORD*>(relocData + offset + sizeof(IMAGE_BASE_RELOCATION) + i * sizeof(WORD));
				unsigned int relocType = static_cast<unsigned int>(relocTypeAndOffset >> 12); // Extract the type from the high bits
				unsigned int relocOffset = static_cast<unsigned int>(relocTypeAndOffset & 0x0FFF); // Extract the offset from the low bits
				// Create a relocation entry and add it to the header
				RelocationHeaderEntry entry = {};
				entry.Address = relocEntry->VirtualAddress + relocOffset;
				entry.Size = sizeof(WORD); // Size of the relocation entry
				entry.Type = relocType;
				relocationHeader.Relocations.push_back(entry);
				++relocationCount;
				relocationHeader.numEntries++;
				PeRelocBlock::PeRelocEntry Entry = [&]() {
					PeRelocBlock::PeRelocEntry entr = {};
					entr.type = relocType;
					entr.offset = relocOffset;
					auto& sectionMap = Pe.sectionMap;
					//now itterate all the sections to find in wich this entry is
					//im an idiot. i store the pe file in a vector of bytes. i dont need to itterate over it repeatedly and can just index it


					// ill get the element at the index RVA+offset
					size_t index = relocEntry->VirtualAddress + relocOffset;

					//ill itterate over the section map and check if the current entry is in the current section

					for (auto& section : sectionMap) {
						if (index >= section.second.start && index < section.second.start + section.second.length) {
							entr.section = section.first;
							entr.AdjustedRVA = index - section.second.start;
							break;
						}
					}
						return entr;
					}();

				//{
				//	.type = relocType,
				//	.offset = relocOffset
				//};
				PeBlock.entries.push_back(std::move(Entry));
				std::cout << "[MXF::MXF] Relocation entry: Address = 0x" << std::hex << entry.Address
					<< ", Size = " << std::dec << entry.Size
					<< ", ImageBaseOffsetofBlock = 0x" << std::hex << relocEntry->VirtualAddress << std::dec
					<< ", Type = " << relocType << std::endl;
			}
			offset += relocSize; // Move to the next relocation block
			peBlocks.push_back(PeBlock);

		}

		//now that i shoud have all information i can actualy start building the file

		//the first and most easy part is just appending the copyable sections.
		//ill order it the following way:
		//.text
		//.data
		//.rdata
		std::cout << "[MXF::MXF] started appending sections...\n";
		const PE::Section& text = Pe.Sections[Pe.sectionMap.at(".text").index];
		size_t startOfText = 0;
		this->Sections.append_range(text.Data);
		const PE::Section& data = Pe.Sections[Pe.sectionMap.at(".data").index];
		size_t startOfData = this->Sections.size();
		this->Sections.append_range(data.Data);
		const PE::Section& rdata = Pe.Sections[Pe.sectionMap.at(".rdata").index];
		size_t startOfrData = this->Sections.size();
		this->Sections.append_range(rdata.Data);

		//no need to add the .reloc section as the data is stored inside a header


		//we should have the size of all headers by now

		this->FullHeaderSize = totalHeaderSize;
		//adjust the startOf* vars to include the totalHeaderSize;
		startOfData += totalHeaderSize;
		startOfrData += totalHeaderSize;
		startOfText += totalHeaderSize;
		//now i get the section the entry point is in// not neccesary as this is allready done in the pe parsing
		


	}
	void MXF::Build()
	{
	}
	void MXF::Write(const std::string_view OutPath)
	{
		(void)OutPath;
	}
}