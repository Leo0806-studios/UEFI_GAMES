#include <CppUnitTest.h>
#include <HEADER/ATOMICS/ATOMIC_F.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace FREE_STD_LIB_TESTS
{
	TEST_CLASS(ATOMIC_TEST_INT) {
public:
	TEST_METHOD(AtomicLoadTest) {
		STD::atomic<int> atomicInt(42);
		int value = atomicInt.load();
		Assert::AreEqual(42, value);
	}
	TEST_METHOD(AtomicStoreTest) {
		STD::atomic<int> atomicInt;
		(void)atomicInt.store(100);
		int value = atomicInt.load();
		Assert::AreEqual(100, value);
	}
	TEST_METHOD(AtomicExchangeTest) {
		STD::atomic<int> atomicInt(10);
		int oldValue = atomicInt.exchange(20);
		Assert::AreEqual(10, oldValue);
		Assert::AreEqual(20, atomicInt.load());
	}
	TEST_METHOD(AtomicCompareExchangeTest) {
		STD::atomic<int> atomicInt(30);
		int expected = 30;
		bool result = atomicInt.compare_exchange_strong(expected, 40);
		Assert::IsTrue(result);
		Assert::AreEqual(40, atomicInt.load());
		Assert::AreEqual(30, expected); // expected should be updated to the old value
	}
	};
}