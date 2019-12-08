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

TEST_F(GivenConstructed, CanConstructDestruct) 
{
}

TEST_F(GivenConstructed, WhenRegisterCallbackExpectNoThrow) 
{
    EXPECT_NO_THROW(m_Console.RegisterWriteCallback(std::bind(&GivenConstructed::RegisteredWrite, this, std::placeholders::_1)));
}

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
    const std::string strVariableName = "TEST";
    const std::string strVariableValue = "123";
    std::string m_ReceivedString;
};

TEST_F(GivenRigisterdCallback, WhenEmptyStringReceveidExpectCommandNotFoundReturned) 
{
    m_Console.OnReceivedString("");
    
    EXPECT_THAT(m_ReceivedString, HasSubstr("Command Not Found."));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}

TEST_F(GivenRigisterdCallback, WhenCorrectSetStringReceveidExpectCommandSetVariableReturned) 
{
    m_Console.OnReceivedString("SET " + strVariableName + " " + strVariableValue);
    
    EXPECT_THAT(m_ReceivedString, HasSubstr(strVariableName + " has ben set to " + strVariableValue));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}

TEST_F(GivenRigisterdCallback, WhenSetStringButNoValue_ExpectInvalidNumberOfArguments) 
{
    m_Console.OnReceivedString("SET TEST" ); //No value provided
    
    EXPECT_THAT(m_ReceivedString, HasSubstr("Invalid number of arguments."));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}

TEST_F(GivenRigisterdCallback, WhenGetStringWithValue_ExpectInvalidNumberOfArguments) 
{
    m_Console.OnReceivedString("GET TEST 1234" ); //No value provided
    
    EXPECT_THAT(m_ReceivedString, HasSubstr("Invalid number of arguments."));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}

class GivenTestVarSet : public GivenRigisterdCallback
{
public:
    GivenTestVarSet()
    {
        m_Console.OnReceivedString("SET " + strVariableName + " " + strVariableValue);
    }
    
protected:
};

TEST_F(GivenTestVarSet, WhenSetStringReceveidExpectCommandSetVariableReturned) 
{

    m_Console.OnReceivedString("GET " + strVariableName);
    
    EXPECT_THAT(m_ReceivedString, HasSubstr(strVariableName + " has ben set to " + strVariableValue));
    EXPECT_THAT(m_ReceivedString, HasSubstr(">"));
}