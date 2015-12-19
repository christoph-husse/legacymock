
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../citrix.mocking/include/citrix/mocking/GMock.h"
#include "GeneratedMocks/MyClassMock.h"

typedef citrix::mocking::GMock G; // a shorthand 

using namespace LegacyDemo;

TEST(MyClassTest, test) {
    
    G::MockFixture fixture;

    EXPECT_CALL(G::DefaultMock<MyClass>(), multiply(3, 4)).
        WillOnce(testing::Return(-1));

    MyClass someInstance;
    
    // if the original function would be invoked, this assertion would fail...
    ASSERT_EQ(-1, someInstance.multiply(3, 4));
}


int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
