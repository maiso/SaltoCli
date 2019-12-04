#include "SaltoCli.h"

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io;

    SaltoCli saltoCLI(io);

    io.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}