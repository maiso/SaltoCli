#include "SaltoCli.h"
#include <boost/bind.hpp>
 
SaltoCli::SaltoCli(boost::asio::io_service& io_service)
: in(io_service, ::dup(STDIN_FILENO)),
  out(io_service, ::dup(STDOUT_FILENO))
{
    StartAsyncRead();
}

SaltoCli::~SaltoCli()
{
    close();
}

void SaltoCli::StartAsyncRead()
{
  // Start an asynchronous operation to read data into a streambuf until it contains a specified delimiter. 
  auto binding = boost::bind(&SaltoCli::received_stdin, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred);

  boost::asio::async_read_until(in, streambuf, DELIMITER, binding);
}

//Note: This method will be called whenever the asycn_read has completed a read.
void SaltoCli::received_stdin(const boost::system::error_code& error,  std::size_t length)
{
    if (!error)
    {
        //Write the received buffer to stdcout
        boost::asio::async_write(out, streambuf,
          boost::bind(&SaltoCli::async_write_output, this,
            boost::asio::placeholders::error));
    }
    else if (error == boost::asio::error::not_found)
    {
      // Didn't get a newline. Send whatever we have.
    }
    else
    {
      close();
    }
}

//Note: This method will be called whenever the async write has completed.
void SaltoCli::async_write_output(const boost::system::error_code& error)
{
    if (!error)
    {
      //Go and wait for the next line to read.
      StartAsyncRead();
    }
    else
    {
        close();
    }
}
  
void SaltoCli::close()
{
    std::cout << "Closing application";
    in.close();
    out.close();
}
