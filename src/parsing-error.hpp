#pragma once

#include <exception>
#include <sstream>


struct incomplete_parsing_error_t
{
    std::stringstream ss;

    incomplete_parsing_error_t()
    {
        ss << "Parsing error: ";
    }

        template<class T>
    incomplete_parsing_error_t& operator<<(T&& t)&
    {
        ss << std::forward<T>(t);
        return *this;
    }

        template<class T>
    incomplete_parsing_error_t&& operator<<(T&& t)&&
    {
        ss << std::forward<T>(t);
        return std::move(*this);
    }
};

struct parsing_error_t : std::exception
{
    std::string inf;

    parsing_error_t(const char* str, int line_number, int column)
    {
        std::stringstream ss;
        ss << "Parsing error: " << str << " at " << line_number << ":" << column;
        inf = std::move(ss).str(); // Problem before C++20: copy of internal string
    }

    parsing_error_t(incomplete_parsing_error_t err, int line_number, int column)
    {
        err.ss << " at " << line_number << ":" << column;
        inf = std::move(err.ss).str(); // Problem before C++20: copy of internal string
    }

    const char* what() const noexcept override
    {
        return inf.c_str();
    }
};
