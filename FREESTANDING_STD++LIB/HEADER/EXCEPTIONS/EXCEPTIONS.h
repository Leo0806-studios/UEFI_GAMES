#pragma once
namespace STD {
	class exception {
		const char* msg = nullptr;
	public:
		exception(const char* msg):msg(msg){}
		virtual const char* what(){
			return msg;
		}
		exception(const exception& other) : msg(other.msg) {}
		exception(exception&& other) noexcept : msg(other.msg) {
			other.msg = nullptr; // move the message pointer to the new object
		}
		exception& operator=(const exception& other) {
			if (this != &other) {
				msg = other.msg; // copy the message pointer
			}
			return *this;
		}
		exception& operator=(exception&& other) noexcept {
			if (this != &other) {
				msg = other.msg; // move the message pointer
				other.msg = nullptr; // set the other object's message pointer to null
			}
			return *this;
		}
		virtual ~exception()
		{

		}

	};

	class bad_alloc :public exception {
	public:
		bad_alloc():exception("bad alloc"){}
	};
	class Bad_free :public exception {
		public:
			Bad_free() :exception("bad free") {}
	};
}