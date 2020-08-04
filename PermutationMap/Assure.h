/**
* Assure.h
* @Author   Tu Yongce <yongce (at) 126 (dot) com>
* @Created  2008-1-1 
* @Modified 2008-1-1 
* @Version  0.1
*/

#ifndef ASSURE_H_INCLUDED
#define ASSURE_H_INCLUDED

#include <ostream>
#include <exception>

class AssureException: public std::exception
{
};

#define Assure(os, x) (void)((!!(x)) || (ShowFailedMessage(os, #x, __FILE__, __LINE__), 0))

inline void ShowFailedMessage(std::ostream &os, const char* expr, const char *file, size_t line)
{
    os << "Failed: " << expr << ", file \"" << file << "\", line " << line << '\n';
    throw AssureException();
}

#define ANONYMOUS_NAMESPACE_START namespace {
#define ANONYMOUS_NAMESPACE_END   }

#endif // ASSURE_H_INCLUDED
