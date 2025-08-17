#include <CppUnitTest.h>
#include <HEADER/VECTOR/VECTOR.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace FREE_STD_LIB_TESTS
{
	struct TriviallyCopyableType_NotMove {
		int value;
		TriviallyCopyableType_NotMove() : value(0) {}
		TriviallyCopyableType_NotMove(TriviallyCopyableType_NotMove&&) = delete; // No move constructor
		TriviallyCopyableType_NotMove& operator=(TriviallyCopyableType_NotMove&&) = delete; // No move assignment
		TriviallyCopyableType_NotMove(const TriviallyCopyableType_NotMove&) = default; // Copy constructor
		TriviallyCopyableType_NotMove& operator=(const TriviallyCopyableType_NotMove&) = default; // Copy assignment
	};
	struct TriviallyMovableType {
		int value;
	};
	struct NonTrivialType {
		int value;
		NonTrivialType() : value(0) {}
		NonTrivialType(const NonTrivialType& other) : value(other.value) {} // Copy constructor
		NonTrivialType(NonTrivialType&& other) noexcept : value(other.value) { other.value = 0; } // Move constructor
		NonTrivialType& operator=(const NonTrivialType& other) { // Copy assignment
			if (this != &other) {
				value = other.value;
			}
			return *this;
		}
		NonTrivialType& operator=(NonTrivialType&& other) noexcept { // Move assignment
			if (this != &other) {
				value = other.value;
				other.value = 0;
			}
			return *this;
		}
		~NonTrivialType() = default; // Destructor
	};
	TriviallyCopyableType_NotMove* createTriviallyCopyableType_NotMove(size_t size) {
		return new TriviallyCopyableType_NotMove[size];
	}
	TriviallyMovableType* createTriviallyMovableType(size_t size) {
		return new TriviallyMovableType[size];
	}
	NonTrivialType* createNonTrivialType(size_t size) {
		return new NonTrivialType[size];
	}
	void DeallocateTriviallyCopyableType_NotMove(TriviallyCopyableType_NotMove* ptr, size_t size) {
		std::ignore = size; // size is not used in this case
		delete[] ptr;
	}
	void DeallocateTriviallyMovableType(TriviallyMovableType* ptr, size_t size) {
		std::ignore = size; // size is not used in this case

		delete[] ptr;
	}
	void DeallocateNonTrivialType(NonTrivialType* ptr, size_t size) {
		std::ignore = size; // size is not used in this case
		delete[] ptr;
	}
	TEST_CLASS(VectorTest) {
	public:
		TEST_METHOD(TestVectorInsert_TriviallyCopyableType_NotMove) {
			STD::vector<TriviallyCopyableType_NotMove> vec;
			vec.SetAllocator(createTriviallyCopyableType_NotMove);
			vec.SetDeallocator(DeallocateTriviallyCopyableType_NotMove);
			vec.push_back(TriviallyCopyableType_NotMove());
			vec.insert(TriviallyCopyableType_NotMove(), 0);
			Assert::AreEqual(vec[0].value, 0);
			Assert::AreEqual(vec[1].value, 0);
			Assert::AreEqual(vec.Size(), size_t(2));
		}
		TEST_METHOD(TestVectorInsert_TriviallyMovableType) {
			STD::vector<TriviallyMovableType> vec;
			vec.SetAllocator(createTriviallyMovableType);
			vec.SetDeallocator(DeallocateTriviallyMovableType);
			vec.push_back(TriviallyMovableType{ 1 });
			vec.insert(TriviallyMovableType{ 2 }, 0);
			Assert::AreEqual(vec[0].value, 2);
			Assert::AreEqual(vec[1].value, 1);
			Assert::AreEqual(vec.Size(), size_t(2));
		}
		TEST_METHOD(TestVectorInsert_NonTrivialType) {
			STD::vector<NonTrivialType> vec;
			vec.SetAllocator(createNonTrivialType);
			vec.SetDeallocator(DeallocateNonTrivialType);
			vec.push_back(NonTrivialType());
			vec.insert(NonTrivialType(), 0);
			Assert::AreEqual(vec[0].value, 0);
			Assert::AreEqual(vec[1].value, 0);
			Assert::AreEqual(vec.Size(), size_t(2));
		}
		TEST_METHOD(TestVectorPushBack_TriviallyCopyableType_NotMove) {
			STD::vector<TriviallyCopyableType_NotMove> vec;
			vec.SetAllocator(createTriviallyCopyableType_NotMove);
			vec.SetDeallocator(DeallocateTriviallyCopyableType_NotMove);
			vec.push_back(TriviallyCopyableType_NotMove());
			Assert::AreEqual(vec[0].value, 0);
			Assert::AreEqual(vec.Size(), size_t(1));
		}
		TEST_METHOD(TestVectorPushBack_TriviallyMovableType) {
			STD::vector<TriviallyMovableType> vec;
			vec.SetAllocator(createTriviallyMovableType);
			vec.SetDeallocator(DeallocateTriviallyMovableType);
			vec.push_back(TriviallyMovableType{ 1 });
			Assert::AreEqual(vec[0].value, 1);
			Assert::AreEqual(vec.Size(), size_t(1));
		}
		TEST_METHOD(TestVectorPushBack_NonTrivialType) {
			STD::vector<NonTrivialType> vec;
			vec.SetAllocator(createNonTrivialType);
			vec.SetDeallocator(DeallocateNonTrivialType);
			vec.push_back(NonTrivialType());
			Assert::AreEqual(vec[0].value, 0);
			Assert::AreEqual(vec.Size(), size_t(1));
		}
	};
}