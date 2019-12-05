#include "Console.h"

#include <boost/algorithm/string.hpp> 

Console::Console()
{
	CommandMap["SET"]    = { 2, std::bind(&Console::HandleCmdSET,    this, std::placeholders::_1), "SET <NAME> <VALUE>", "Set a variable with a given value."}; 
	CommandMap["GET"]    = { 1, std::bind(&Console::HandleCmdGET,    this, std::placeholders::_1), "GET <NAME>", "Get a variable value."};
    CommandMap["DELETE"] = { 1, std::bind(&Console::HandleCmdDELETE, this, std::placeholders::_1), "DELETE <NAME>", "Delete a variable."};
}

void Console::StartCommand()
{
    PublishString(">");
}

void Console::OnReceivedString(const std::string & sString)
{
    std::vector<std::string> strs;
    boost::split(strs, sString, boost::is_any_of(" "));

    std::string returnString = ParseCommand(strs);

    PublishString(returnString + '\n');
    
    StartCommand();
}

std::string Console::PrintUsage()
{
    std::string usageString = "Available commands:\n";
    for (auto const& cmd : CommandMap)
    {
        usageString += cmd.second.usage + " | " + cmd.second.description + '\n';
    }
    
    return usageString;
}

std::string Console::ParseCommand(const std::vector<std::string> & vecTokens)
{
    std::string strResult = "Command Not Found.\n" + PrintUsage();
    if(vecTokens.size() > 0)
    {
        auto it = CommandMap.find(vecTokens[0]);
        if ( it != CommandMap.end() ) 
        {
          if(it->second.nrOfTokens == vecTokens.size() -1)
          {
               strResult = it->second.callback(vecTokens);
          }
          else
          {
               strResult = "Invalid number of arguments.\n Usage:" + it->second.usage;
          }
        }
    }
    return strResult;
}

std::string Console::HandleCmdSET(const std::vector<std::string> & vecTokens)
{ 
    return "HandleCmdSET";
}
std::string Console::HandleCmdGET(const std::vector<std::string> & vecTokens)
{ 
    return "HandleCmdGET";
}
std::string Console::HandleCmdDELETE(const std::vector<std::string> & vecTokens)
{ 
    return "HandleCmdDELETE";
}
    
void Console::RegisterWriteCallback(std::function<void(const std::string &)> func)
{
    m_WriteCallbacks.push_back(func);
}
void Console::PublishString(const std::string & sString)
{
    for(auto writeCallback : m_WriteCallbacks)
    {
        writeCallback(sString);
    }
}
