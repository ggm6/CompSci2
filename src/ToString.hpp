#ifndef TOSTRING_HPP_INCLUDED
#define TOSTRING_HPP_INCLUDED
// NOTE: The built in to_string function wasn't working for me, because Code::Blocks has some sort of mental illness
#include <sstream>

inline
std::string to_string(int i)
{ // Function takes integer argument, outputs a string stream of just that integer
    std::stringstream ss;
    ss << i;
    return ss.str();
}


#endif // TOSTRING_HPP_INCLUDED
