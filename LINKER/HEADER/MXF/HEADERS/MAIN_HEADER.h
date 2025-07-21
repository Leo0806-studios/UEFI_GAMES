 namespace MXF_LINKER {
	 constexpr inline char MainHeaderId[4] = { 'M', 'H', 'D', 0 }; // 'MHD' in ASCII, this is the identifier for the Main Header
#pragma pack(push)
#pragma pack(1)
	struct  MainHeader {
		const int HeaderId = *reinterpret_cast<const int*>(&MainHeaderId); // 'MHD' in ASCII, this is the identifier for the Main Header
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

	};
#pragma pack(pop)
}