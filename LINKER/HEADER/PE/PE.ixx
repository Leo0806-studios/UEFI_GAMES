export module PE;
import <Windows.h>;
import <vector>;
import <string>;
import <unordered_map>;
#pragma warning (push)
#pragma warning(disable : 4820)
export namespace MXF_LINKER {
	class PE {
		friend class MXF;
		IMAGE_DOS_HEADER DosHeader;
		IMAGE_FILE_HEADER FileHeader;
		IMAGE_OPTIONAL_HEADER64 OptionalHeader;
		std::vector<IMAGE_SECTION_HEADER> SectionHeaders;
		size_t offsetofentry = 0;// this is the ofsett of the entry point in the .text section
		bool is64Bit = true;
		struct Section {
			std::string Name; // Name of the section, can be used for debugging or identification
			std::vector<UINT8> Data;
		};
		struct Pair {
			size_t start=0;
			size_t length=0;
			size_t index = 0;
		};
		std::unordered_map<std::string, Pair> sectionMap;
		std::vector<Section> Sections;
		std::vector<UINT8> RawData;//this contains the ful binary data of the PE file.

	public:
		/// <summary>
		/// Constructs a PE object from the specified file path.
		/// </summary>
		/// <param name="filePath">The path to the file to be loaded as a PE object.</param>
		explicit PE(const std::string_view filePath);

		/// <summary>
		/// parses the full file and fills the data structures.
		/// 
		/// </summary>
		void Parse();



	};
}
#pragma warning (pop)