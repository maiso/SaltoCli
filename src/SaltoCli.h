#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

namespace posix = boost::asio::posix;
namespace asio = boost::asio;

class SaltoCli
{
public:
    SaltoCli() = delete;
    SaltoCli(boost::asio::io_service& io_service);
    ~SaltoCli();
private:
    void StartAsyncRead();
    void received_stdin(const boost::system::error_code& error, std::size_t length);
    void async_write_output(const boost::system::error_code& error);
    void close();

    static const char DELIMITER = '\n';

    posix::stream_descriptor in;
    posix::stream_descriptor out;
    boost::asio::streambuf streambuf;
};