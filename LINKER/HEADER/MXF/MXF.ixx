export module MXF;
import HEADERS;
import std;
export namespace MXF_LINKER {
	//this file contains the definition the MXF file Format.
	

	
	//this struct does not need to be packed as it is the easy to work with representation.
	// iwill copy everything to a vector of bytes and write it to a file anyways
	struct MXF
	{
		unsigned int FileType = 0;
		unsigned __int64 FullHeaderSize = 0; // this is the size of all headers. if used as an offset from the beginning of the file it will point to the first non header byte
		static constexpr inline char Magic[8] = {'M', 'X', 'F','E','X','E','C', 0}; // this is the magic number of the file. it is used to identify the file as a MXF file.


		/*
		HEADER BITMAP
		first bit is the main header.
		second bit is the Secton Poiter header.each entry in this header is a pinte to the beginning of the section, its length and the section name.
		third is relocation header. this header contains the relocation information for the file.
		4th is the symbol table header. this header contains the symbol table for the file.
		5th is a export table.
		6th is a import table
		others are reserved for future use.
		*/
		std::bitset<64> HeaderBitmap = 0; // this is a bitmap of the headers that are present in the file. each bit represents a header. if the bit is set then the header is present.
		std::vector<unsigned __int8> Headers; // this is a vector of bytes that contains all the headers in the file. the headers are packed in the order they are defined in the header bitmap.
		std::vector<unsigned __int8> Sections; // this is a vector of bytes that contains all the sections in the file. the sections are packed in the order they are defined in the section pointer header.
	};
}