#include "Console.h"

#include <boost/algorithm/string.hpp> 
#include <fstream>

Console::Console()
{
    CommandMap["SET"]    = { 2, std::bind(&Console::HandleCmdSET,    this, std::placeholders::_1), "SET <NAME> <VALUE>", "Set a variable with a given value."}; 
    CommandMap["GET"]    = { 1, std::bind(&Console::HandleCmdGET,    this, std::placeholders::_1), "GET <NAME>", "Get a variable value."};
    CommandMap["DELETE"] = { 1, std::bind(&Console::HandleCmdDELETE, this, std::placeholders::_1), "DELETE <NAME>", "Delete a variable."};
    CommandMap["LIST"]   = { 0, std::bind(&Console::HandleCmdLIST,   this, std::placeholders::_1), "LIST", "List all variables."};
    CommandMap["HIRE"]   = { 1, std::bind(&Console::HandleCmdHIRE,   this, std::placeholders::_1), "HIRE <NAME>", "Who should you hire?"};
    
    //Open the json file to read the variables
    std::ifstream i("SALTO.json");
    //Note: Improvement would be to put this is sperate class for better error handling.
    if(i.fail() == false)
    {
        i >> m_json;
    }

}

Console::~Console()
{
    //Open the json file to store the variables
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
    //Split the received string 
    std::vector<std::string> strs;
    boost::split(strs, sString, boost::is_any_of(" "));

    //Try to run the received command
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
        //In the first token should be the command
        auto it = CommandMap.find(vecTokens[0]);
        if ( it != CommandMap.end() ) 
        {
          //Check if enough arguments are provided
          if(it->second.nrOfTokens == vecTokens.size() -1)
          {
              std::vector<std::string> vecArguments(vecTokens.begin() + 1, vecTokens.end());
              strResult = it->second.callback(vecArguments);
          }
          else
          {
               strResult = "Invalid number of arguments.\n Usage:" + it->second.usage;
          }
        }
    }
    return strResult;
}

std::string Console::HandleCmdSET(const std::vector<std::string> & vecArguments)
{
    std::string strVariable = vecArguments[0];
    m_json[strVariable] = vecArguments[1];
    return strVariable + " has ben set to " + vecArguments[1];
}

std::string Console::HandleCmdGET(const std::vector<std::string> & vecArguments)
{ 
    std::string strVariable = vecArguments[0];
    std::string strResult = "Not Found";
    if (m_json.find(strVariable) != m_json.end()) 
    {
        strResult = m_json[strVariable];
    }
    return strVariable + " is " + strResult;
}

std::string Console::HandleCmdDELETE(const std::vector<std::string> & vecArguments)
{ 
    std::string strVariable = vecArguments[0];
    std::string strResult = "Not Found";
    // find an entry
    if (m_json.find(strVariable) != m_json.end()) 
    {
      // there is an entry with key "foo"
      m_json.erase(strVariable);
      strResult = "erased.";
    }

    return strVariable + " is " + strResult;
}

std::string Console::HandleCmdLIST(const std::vector<std::string> & vecArguments)
{ 
    std::string strResult = "Variable : Value";
    
    for (auto& el : m_json.items()) 
    {
      strResult += "\n" + el.key() + " : " + std::string(el.value());
    }

    return strResult;
}

std::string Console::HandleCmdHIRE(const std::vector<std::string> & vecArguments)
{ 
    std::string strVariable = vecArguments[0];
    std::string strResult = "Bad choice :( Try again. Hint: Who made this application?";

    if (strVariable == "Michiel") 
    {
      strResult = "Good choice!";
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
