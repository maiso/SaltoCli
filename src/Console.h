#ifndef CONSOLE_H
#define CONSOLE_H

//System includes
#include <cstdlib>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

//Project includes
#include "IConsoleObserver.h" 

namespace posix = boost::asio::posix;
namespace asio = boost::asio;

class Console
{
public:
    Console();
    virtual ~Console()= default;
    
    void StartCommand();
    
    void OnReceivedString(const std::string & sString);
    
    void RegisterWriteCallback(std::function<void(const std::string &)>);

private:
    std::string PrintUsage();
    void PublishString(const std::string & sString);
    std::string ParseCommand(const std::vector<std::string> & vecTokens);
    
    std::string HandleCmdSET(const std::vector<std::string> & vecTokens);
    std::string HandleCmdGET(const std::vector<std::string> & vecTokens);
    std::string HandleCmdDELETE(const std::vector<std::string> & vecTokens);
    
    std::vector<std::function<void(const std::string &)>> m_WriteCallbacks;
    
    typedef std::function<std::string(const std::vector<std::string> &)> CommandHandlerFunc;
    struct cmdStruct{
        size_t nrOfTokens;
        CommandHandlerFunc callback;
        std::string usage;
        std::string description;
    };
    std::map<std::string, cmdStruct> CommandMap;

};

#endif
