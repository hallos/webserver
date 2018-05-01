#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class HttpTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(HttpTest);
	CPPUNIT_TEST(TestMain);
	CPPUNIT_TEST_SUITE_END();

	public:
		//~ Call before tests
		void setUp(void) {}

		//~ Call after tests
		void tearDown(void) {}

		void TestMain(void);
		void TestMain2(void);
};