extern "C" {

	void* malloc(size_t size);
	void free(void* ptr);

	void _free_dbg(void* ptr);
}