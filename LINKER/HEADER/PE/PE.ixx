export module PE;
import <Windows.h>;
import <vector>;
import <string>;
#pragma warning (push)
#pragma warning(disable : 4820)
export namespace MXF_LINKER {
	class PE {
		IMAGE_DOS_HEADER DosHeader;
		IMAGE_FILE_HEADER FileHeader;
		IMAGE_OPTIONAL_HEADER OptionalHeader;
		std::vector<IMAGE_SECTION_HEADER> SectionHeaders;
		bool is64Bit = true;
		struct Section {
			std::vector<UINT8> Data;
		};
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