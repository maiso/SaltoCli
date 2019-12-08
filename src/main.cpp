#include <csignal>

#include "SaltoCli.h"

static SaltoCli * saltoCli ;

void signalHandler( int signum ) 
{
   delete saltoCli;
   exit(signum);  
}

int main(int argc, char* argv[])
{
   // register signal SIGINT and signal handler  
   signal(SIGINT, signalHandler);  
   
  try
  {
    boost::asio::io_service io;

    saltoCli = new SaltoCli(io);
    
    io.run();
    
    delete saltoCli;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  
  return 0;
}
