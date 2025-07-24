#pragma once
import std;
constexpr int MakeFourCC(char a, char b, char c, char d);

 namespace MXF_LINKER {
#pragma pack(push)
#pragma pack(1)
	struct  MainHeader {
		const int HeaderId = MakeFourCC('M', 'H', 'D', '\0'); // 'MHD' in ASCII, this is the identifier for the Main Header
		size_t adressofEntryPoint = 0; 
		unsigned short minOsVersion = 0;
		unsigned short MaxOsVersion = 0;
		MainHeader() = default;

		MainHeader(const MainHeader&other) :
			HeaderId(other.HeaderId),
			adressofEntryPoint(other.adressofEntryPoint),
			minOsVersion(other.minOsVersion),
			MaxOsVersion(other.MaxOsVersion) {
		}
		MainHeader(MainHeader&& other):
			HeaderId(other.HeaderId),
			adressofEntryPoint(other.adressofEntryPoint),
			minOsVersion(other.minOsVersion),
			MaxOsVersion(other.MaxOsVersion) {
		}
		MainHeader& operator=(const MainHeader&other) {
			adressofEntryPoint = other.adressofEntryPoint;
			minOsVersion = other.minOsVersion;
			MaxOsVersion = other.MaxOsVersion;
			return *this;
		}
		MainHeader& operator=(MainHeader&& other) {
			adressofEntryPoint = other.adressofEntryPoint;
			minOsVersion = other.minOsVersion;
			MaxOsVersion = other.MaxOsVersion;
			return *this;
		}
		~MainHeader() = default;
		/// <summary>
		/// Converts the MainHeader object to a byte array.
		/// </summary>
		/// <returns>A std::vector<unsigned __int8> containing the byte representation of the MainHeader object.</returns>
		std::vector<unsigned __int8> ToByteArray() const {
			std::vector<unsigned __int8> bytes;
			bytes.reserve(sizeof(MainHeader));
			std::memcpy(bytes.data(), this, sizeof(MainHeader));
			return bytes;
		}

	};
#pragma pack(pop)
}