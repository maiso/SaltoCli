#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Console.h"

using::testing::_;
using ::testing::HasSubstr;

class GivenConstructed : public ::testing::Test
{
public:
    GivenConstructed()
    {
    }
    
    void RegisteredWrite(const std::string &)
    {
        
    }
    
    protected:
    Console m_Console;
};

class GivenRigisterdCallback : public GivenConstructed
{
public:
    GivenRigisterdCallback()
    {
        m_Console.RegisterWriteCallback(std::bind(&GivenRigisterdCallback::Write, this, std::placeholders::_1));
    }
    
    void Write(const std::string & str)
    {
        m_ReceivedString += str;
    }
    protected:
    std::string m_ReceivedString;
};

TEST(ConsoleTest, CanConstructDestruct) {
    
}

TEST_F(GivenConstructed, WhenRegisterCallbackExpectNoThrow) 
{
    EXPECT_NO_THROW(m_Console.RegisterWriteCallback(std::bind(&GivenConstructed::RegisteredWrite, this, std::placeholders::_1)));
}

TEST_F(GivenRigisterdCallback, WhenEmptyStringReceveidExpectCommandNotFoundReturned) 
{
    m_Console.OnReceivedString("");
    
    std::cout << m_ReceivedString;
    EXPECT_THAT(m_ReceivedString, HasSubstr("Command Not Found."));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}
