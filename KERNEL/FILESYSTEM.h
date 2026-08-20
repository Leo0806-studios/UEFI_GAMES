#pragma once

//for now this is only an idea/plan and does not represent ither the on disk or in memory format
import std;
import std.compat;
namespace SYSTEM::SUBSYSTEMS::FILESYSTEM {
#define COMBINECHARS(A,B,C,D) ((A) | ((B) << 8) | ((C) << 16) | ((D) << 24))
	enum class MagicFS :uint32_t {
		FileNode = COMBINECHARS('F', 'N', 'D', 'E'),
		DirNode = COMBINECHARS('D', 'N', 'D', 'E'),
		ExtendedMetadata = COMBINECHARS('E', 'M', 'D', 'A'),
		Footer = COMBINECHARS('F', 'O', 'O', 'T'),
		VolumeNode = COMBINECHARS('V', 'O', 'L', 'N'),
	};
	enum class FileNodeFlags : uint32_t {
		none = 0b0,
		InlineName = 0b1 << 0,
		Shared = 0b1 << 1,//if multiple files share the same logical block. the extendet metadata table contains a pointer to the nex file in the block, and a pointer to the first file in the block and how many files till the end
		Fragmented = 0b1 << 2,//if the file is fragmented. the extendet metadata table contains a pointer to the next fragment of the file
	};
	enum class ExtendetMetadataType {
		None = 0,	
		SharedFile = 1,
	};
	class FileNode {
		MagicFS magic = MagicFS::FileNode;
		uint32_t flags = 0; 
		uint64_t logicalBlock = 0;
		union {
			char nameInline[];//nullterminated string. only valid if the InlineName flag is set
			uint64_t nameBlock = 0;
		};
	};
	class ExtendetMetadata {
		MagicFS magic = MagicFS::ExtendedMetadata;
		ExtendetMetadataType type = ExtendetMetadataType::None;
		uint64_t ECCHash = 0; //hash of the entire file and nodes associated with it excluding this field. if it does not match the file might be corrupted.
		union {
			struct SharedFileMetadata {
				uint64_t offsettToNextFileInBlock = 0;
				uint64_t offsettToFirstFileInBlock = 0;//treat this number as negative
				uint64_t CountFillesTillEnd = 0;//the amount of files in the block past this one. so the last one wil have 0, the first one N, and the second one N-1, etc
			} sharedFileMetadata;
			struct FragmentedFileMetadata {
				uint64_t LogicalBlockOfNextFragment = 0;
			} fragmentedFileMetadata;
		};
	};
	class Footer {
		MagicFS magic = MagicFS::Footer;
	};
	class DirectoryNode {
		MagicFS magic = MagicFS::DirNode;
	};
	class VolumeNode {
		MagicFS magic = MagicFS::VolumeNode;
	};
	class MasterFileTable{
		enum class MTFEntryFlags : uint64_t {
			none = 0b0,
			directoryEnd =0b1 <<0 ,
			directoryStart = 0b1 << 1,// dirs are stored like nested curly braces in c++ programs. a entry with the Directory start flag opens one (an is also the entry to the directory node and one with DirectoryEnd set closes it
		};
		struct MTFEntry {
			uint64_t logicalBlock = 0;
			uint64_t flags = std::to_underlying(MTFEntryFlags::none);
		};
		std::vector<MTFEntry> entries;
	};
}