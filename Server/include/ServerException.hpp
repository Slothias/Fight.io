#ifndef SERVEREXCEPTION_H_INCLUDED
#define SERVEREXCEPTION_H_INCLUDED
#include <exception>


class WSAStartupException :public std::exception
{
public:
    const char* what() const throw() { return "WSAStartup failed with error!"; };
};

class SocketException: public std::exception
{
public:
    const char* what() const throw() { return "Socket failed with error."; };
};

class BindException: public std::exception
{
    public:
    const char* what() const throw() { return "bind failed with error"; };
};
class ListenException: public std::exception
{
public:

 const char* what() const throw() { return "listen failed with error"; };
};

#endif // SERVEREXCEPTION_H_INCLUDED
