#include "mexception.h"

MException::MException()
    :name("No problems!") {}

MException::MException(std::string _name)
    :name(_name) {}

MException::MException(std::string _name, std::exception& ex)
    :std::exception(ex), name(_name) {}

std::string MException::get_name()
{
    return name;
}
