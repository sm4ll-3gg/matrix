#ifndef MEXCEPTION_H
#define MEXCEPTION_H

#include<iostream>

class MException : public std::exception
{
    std::string name;
public:
    MException();
    MException(std::string);
    MException(std::string, std::exception&);

    std::string get_name();
};

#endif // MEXCEPTION_H
