#include "Console.h"

#include <boost/algorithm/string.hpp> 
#include <fstream>

Console::Console()
{
	CommandMap["SET"]    = { 2, std::bind(&Console::HandleCmdSET,    this, std::placeholders::_1), "SET <NAME> <VALUE>", "Set a variable with a given value."}; 
	CommandMap["GET"]    = { 1, std::bind(&Console::HandleCmdGET,    this, std::placeholders::_1), "GET <NAME>", "Get a variable value."};
  CommandMap["DELETE"] = { 1, std::bind(&Console::HandleCmdDELETE, this, std::placeholders::_1), "DELETE <NAME>", "Delete a variable."};
  CommandMap["LIST"]   = { 0, std::bind(&Console::HandleCmdLIST,   this, std::placeholders::_1), "LIST", "List all variables."};

  std::ifstream i("SALTO.json");
  
  if(i.fail() == false)
  {
      i >> m_json;
  }

}

Console::~Console()
{
  std::ofstream o("SALTO.json");
  if(o.fail() == false)
  {
    o << m_json;
  }
  else
  {
    std::cerr << "Failed to store data";
  }
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
    m_json[vecTokens[1]] = vecTokens[2];
    return vecTokens[1] + " has ben set to " + vecTokens[2];
}

std::string Console::HandleCmdGET(const std::vector<std::string> & vecTokens)
{ 
    std::string strResult = "Not Found";
    if (m_json.find(vecTokens[1]) != m_json.end()) 
    {
        strResult = m_json[vecTokens[1]];
    }
    return vecTokens[1] + " is " + strResult;
}

std::string Console::HandleCmdDELETE(const std::vector<std::string> & vecTokens)
{ 
    std::string strResult = "Not Found";
    // find an entry
    if (m_json.find(vecTokens[1]) != m_json.end()) 
    {
      // there is an entry with key "foo"
      m_json.erase(vecTokens[1]);
      strResult = "erased.";
    }

    return vecTokens[1] + " is " + strResult;
}

std::string Console::HandleCmdLIST(const std::vector<std::string> & vecTokens)
{ 
    std::string strResult = "Variable : Value";
    
    for (auto& [key, value] : m_json.items()) 
    {
      strResult += "\n" + key + " : " + std::string(value);
    }

    return strResult;
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
