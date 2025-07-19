 namespace MXF_LINKER {
#pragma pack(push, 1)
	struct MainHeader {
		size_t adressofEntryPoint = 0; 
		unsigned short minOsVersion = 0;
		unsigned short MaxOsVersion = 0;

	};
#pragma pack(pop)
}