#ifndef SALTOCLI_H
#define SALTOCLI_H

//System includes
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

//Project includes
#include "Console.h"
#include "IConsoleObserver.h"

namespace posix = boost::asio::posix;
namespace asio = boost::asio;

class SaltoCli
{
public:
    SaltoCli() = delete;
    SaltoCli(boost::asio::io_service& io_service);
    virtual ~SaltoCli();
    
    void Write(const std::string & sString);
private:
    void StartAsyncRead();
    void received_stdin(const boost::system::error_code& error, std::size_t length);
    void async_write_complete(const boost::system::error_code& error);
    void close();

    static const char DELIMITER = '\n';

    posix::stream_descriptor in;
    posix::stream_descriptor out;
    boost::asio::streambuf streambuf;
    boost::asio::io_service& m_io_service;
    Console m_Console;
};

#endif