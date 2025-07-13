#pragma once
namespace STD {
	template<typename Type, size_t size>
		class Array {
			Type _Data[Size] = { 0 };
			using DataType = Type;
			constexpr size_t Size;
			constexpr char ErrorReturn[sizeof(DataType)] = {1};


		public:
			constexpr Array() = default;
			constexpr Type& operator[](size_t index) {
				if constexpr (index < Size) {
					return this->_Data[index];
				}
				else {
					// for now as there are no exceptions ill return a refference to ErrorReturn.
					return *(DataType)&ErrorReturn;
				}

			}
			template <size_t index>
			constexpr Type Get() {
				static_assert(!(index < Size), "index is not within bounds for array accses");
				return _Data[index];
			}

		};
	}
