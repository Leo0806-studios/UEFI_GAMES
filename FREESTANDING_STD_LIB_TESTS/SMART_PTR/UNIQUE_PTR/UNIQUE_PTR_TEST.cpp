#include <CppUnitTest.h>
#include <HEADER/SMART_POINTERS/UNIQUE_PTR/UNIQUE_PTR.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace FREE_STD_LIB_TESTS
{
	TEST_CLASS(UNIQUE_PTR_TEST_INT) {
		TEST_METHOD(TestUniquePointerCreation) {
			// Create a unique pointer to an integer
			STD::UniquePointer<int> uniquePtr = STD::UniquePointer<int>::MakeUnique(new int(42), new STD::GenericUniqueBlock<int>());
			Assert::IsNotNull(uniquePtr.operator->());
			Assert::AreEqual(42, *uniquePtr.operator->());
		}
		TEST_METHOD(TestUniquePointerMove) {
			// Create a unique pointer and move it
			STD::UniquePointer<int> uniquePtr1 = STD::UniquePointer<int>::MakeUnique(new int(100), new STD::GenericUniqueBlock<int>());
			STD::UniquePointer<int> uniquePtr2 = STD::move(uniquePtr1);
			Assert::IsNull(uniquePtr1.operator->());
			Assert::IsNotNull(uniquePtr2.operator->());
			Assert::AreEqual(100, *uniquePtr2.operator->());
		}

	};
}


