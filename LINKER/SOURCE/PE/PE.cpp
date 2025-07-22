import	PE;
import std;
import <string>;
import <Windows.h>;
namespace MXF_LINKER {
	PE::PE(const std::string_view filePath) {
		std::ifstream file(filePath.data(), std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + std::string(filePath));
		}
		std::streamsize size = file.tellg();//this is bytes i think.//yes it is
		file.seekg(0, std::ios::beg);
		RawData.resize(static_cast<size_t>(size));
		if (!file.read(reinterpret_cast<char*>(RawData.data()), size)) {
			throw std::runtime_error("Failed to read file: " + std::string(filePath));
		}
		//copy the file into the buffer
		RawData = std::move(std::vector<UINT8>(RawData.begin(), RawData.end()));
		file.close();

	}
	void PE::Parse()
	{
		if (RawData.size() < sizeof(IMAGE_DOS_HEADER)) {
			throw std::runtime_error("File is too small to be a valid PE file.");
		}
		// Read DOS header
		std::memcpy(&DosHeader, RawData.data(), sizeof(IMAGE_DOS_HEADER));
		std::cout << "[PE::Parse] DOS Header e_magic: 0x" << std::hex << DosHeader.e_magic << ", e_lfanew: 0x" << DosHeader.e_lfanew << std::endl;
		if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
			throw std::runtime_error("Invalid DOS signature.");
		}
		// Read NT headers
		if (RawData.size() < DosHeader.e_lfanew + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER)) {
			throw std::runtime_error("File is too small to contain NT headers.");
		}
		auto ntHeadersOffset = DosHeader.e_lfanew + 4;
		std::cout << "[PE::Parse] NT Headers Offset: 0x" << std::hex << ntHeadersOffset << std::endl;
		std::memcpy(&FileHeader, RawData.data() + ntHeadersOffset, sizeof(IMAGE_FILE_HEADER));
		std::cout << "[PE::Parse] FileHeader.Machine: 0x" << std::hex << FileHeader.Machine << std::endl;
		if (FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 && FileHeader.Machine != IMAGE_FILE_MACHINE_I386) {
			throw std::runtime_error("Unsupported machine type.");
		}
		is64Bit = (FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64);
		std::memcpy(&OptionalHeader, RawData.data() + ntHeadersOffset + sizeof(IMAGE_FILE_HEADER), sizeof(IMAGE_OPTIONAL_HEADER));
		std::cout << "[PE::Parse] OptionalHeader.Magic: 0x" << std::hex << OptionalHeader.Magic << std::endl;

		if (is64Bit) {
			if (OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
				throw std::runtime_error("Invalid optional header magic for 64-bit PE.");
			}
		}
		else {
			if (OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
				throw std::runtime_error("Invalid optional header magic for 32-bit PE.");
			}
		}
		size_t sectionCount = FileHeader.NumberOfSections;
		size_t sectionSize = sizeof(IMAGE_SECTION_HEADER);
		std::cout << "[PE::Parse] NumberOfSections: " << std::dec << sectionCount << ", SectionHeaderSize: " << sectionSize << std::endl;

		if (RawData.size() < ntHeadersOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) + sectionCount * sectionSize) {
			throw std::runtime_error("File is too small to contain section headers.");
		}
		for (size_t i = 0; i < sectionCount; ++i) {
			auto sectionOffset = ntHeadersOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) + i * sectionSize;
			IMAGE_SECTION_HEADER section = {};
			std::memcpy(&section, RawData.data() + sectionOffset, sectionSize);
			std::cout << "[PE::Parse] Section " << i << " Name: ";
			std::string namesection = "";
			for (int j = 0; j < IMAGE_SIZEOF_SHORT_NAME; ++j) {
				if (section.Name[j] == 0) break;
				std::cout << static_cast<char>(section.Name[j]);
				namesection += static_cast<char>(section.Name[j]);

			}
			

				DWORD entryPointRVA = OptionalHeader.AddressOfEntryPoint;
				DWORD sectionVA = section.VirtualAddress;
				DWORD sectionSize_ = section.Misc.VirtualSize;

				if (entryPointRVA >= sectionVA && entryPointRVA < sectionVA + sectionSize_) {
					size_t entryPointOffsetInSection = entryPointRVA - sectionVA;
					//size_t entryPointFileOffset = sectionRawPtr + entryPointOffsetInSection;
					std::cout <<std::dec<< "[PE::Parse] section start: " << std::hex << sectionVA
						<< " section length: " << sectionSize_ <<std::dec<< '\n';
					std::cout << "[PE::Parse] Entry point file offset in "<< namesection<<": 0x" << std::hex << entryPointOffsetInSection << std::endl;
					// Store or use entryPointFileOffset as needed
				}


			//}
			std::cout << ", SizeOfRawData: 0x" << std::hex << section.SizeOfRawData << ", PointerToRawData: 0x" << section.PointerToRawData << std::endl;
			SectionHeaders.push_back(section);
			Section sec;
			sec.length = section.SizeOfRawData;
			sec.setionStart = section.PointerToRawData;
			sec.Name = namesection;
			sec.Data.resize(section.SizeOfRawData);
			if (section.PointerToRawData + section.SizeOfRawData <= RawData.size()) {
				std::memcpy(sec.Data.data(), RawData.data() + section.PointerToRawData, section.SizeOfRawData);
			}
			else {
				throw std::runtime_error("Section data exceeds file size.");
			}
			sec.Name = namesection;
				Sections.push_back(sec);

		}
		DWORD entryPointRVA = OptionalHeader.AddressOfEntryPoint;
		//itterate ove the section vector  to check if the entry point is in it

		for (auto& sect : this->Sections) {
			if (entryPointRVA >= sect.setionStart && entryPointRVA < sect.setionStart + sect.length) {
				//we found the section. now get the offset within the section
				this->offsetofentry = entryPointRVA - sect.setionStart;
				std::cout << "[PE::Parse] section start: " << std::hex << sect.setionStart
					<< " section length: " << sect.length << '\n';
				this->entrySectionName = sect.Name;

				std::cout << "[PE::Parse] found entry point in section " << sect.Name 
					<< " offset into section i 0x"<<std::hex<<offsetofentry << '\n';
			}
		}

		//as the section map has been removed this will be move to the MEX::MEX constructor
		//for (const auto& section : SectionHeaders) {
		//	DWORD sectionVA = section.VirtualAddress;
		//	DWORD sectionSize_ = section.Misc.VirtualSize;

		//	if (entryPointRVA >= sectionVA && entryPointRVA < sectionVA + sectionSize_) {
		//		this->offsetofentry = entryPointRVA - sectionVA;
		//		//get the name of the section the entry point is in
		//		for (auto& sec : this->sectionMap) {

		//				if (entryPointRVA >= sec.second.start && entryPointRVA < sec.second.start + sec.second.length) {
		//					entrySectionName = sec.first;
		//					break;
		//				}
		//			
		//		}
		//		// Optional: Print debugging info
		//		std::cout << "[PE::Parse] Entry Point RVA: 0x" << std::hex << entryPointRVA
		//			<< " found in section starting at VA: 0x" << sectionVA
		//			<< ", offset into section: 0x" << offsetofentry 
		//			<< ", Section name : "<< entrySectionName<< std::endl;

		//		break;
		//	}
		//}
		// Additional parsing can be done here, such as reading imports, exports, etc.

		//print sectionMapt to be sure //section map has been removed


	}

} // namespace MXF_LINKER