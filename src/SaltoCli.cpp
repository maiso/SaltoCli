#include "SaltoCli.h"
#include <boost/bind.hpp>
 
SaltoCli::SaltoCli(boost::asio::io_service& io_service)
: in(io_service, ::dup(STDIN_FILENO)),
  out(io_service, ::dup(STDOUT_FILENO)),
  m_io_service(io_service)
{
    //m_Console.RegisterObserver(*this);
    m_Console.RegisterWriteCallback(std::bind(&SaltoCli::Write, this, std::placeholders::_1));
    StartAsyncRead();
    
    m_Console.StartCommand();
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
        //Transform the stream buffer to a string
        boost::asio::streambuf::const_buffers_type bufs = streambuf.data();
        std::string str(boost::asio::buffers_begin(bufs),
                        boost::asio::buffers_begin(bufs) + streambuf.size() - 1);
        streambuf.consume(streambuf.size()); //Consume the buffer otherwise the async read will trigger instantly.

        //Send the just received string async to the console
        m_io_service.dispatch(boost::bind(&Console::OnReceivedString, m_Console, str));

        StartAsyncRead();
    }
    else
    {
      close();
    }
}

void SaltoCli::Write(const std::string & sString) 
{
    //std::cout << "Look we are going to write stuff";
    //Write the received buffer to stdcout
    boost::asio::async_write(out, boost::asio::buffer(sString),
      boost::bind(&SaltoCli::async_write_complete, this,
        boost::asio::placeholders::error));
}

//Note: This method will be called whenever the async write has completed.
void SaltoCli::async_write_complete(const boost::system::error_code& error)
{
    if (!error)
    {
      //Go and wait for the next line to read.
      // StartAsyncRead();
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
