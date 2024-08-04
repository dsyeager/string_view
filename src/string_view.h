#pragma once

#include "aton.h"

#include <string_view>
#include <iostream>

namespace dsy
{

/*
   This is a new, personal version of similar classes I've written/maintained in the past.
   It's functionality will grow as my personal projects require.

   This class wraps std::string_view adding convenience methods that I've found useful when building apps
   As std::string_view slowly matures, duplicate functionality will be deprecated here.
*/

class string_view : public std::string_view
{
public:
    string_view() :std::string_view() {}

    string_view(const std::string_view& sv) :std::string_view(sv) {}

    string_view(const char* val, size_t len) :std::string_view(val, len) {}

    string_view(const char* val) :std::string_view(val) {}

    string_view(const std::string& val) :std::string_view(val) {}

    // wishy washy about split vs remove_until
    
    string_view split(char delim)
    {
        size_t pos = this->find(delim);
        string_view ret = this->substr(0, pos);
        if (pos >= this->length() - 1) // -1 to make zero based
            *this = string_view();
        else
            *this = this->substr(pos + 1);
        return ret;
    }

    template<typename NUMB> bool aton(NUMB &val, std::string_view *rem = nullptr)
    {
// TODO: seriously consider using std::from_chars, might be a little slower but probably has better limit checking.
        std::string_view rem_local;
        if (!rem)
            rem = &rem_local;
        val = ::aton<NUMB>(*this, rem);
        return rem->empty();
    }

    // TODO: add _ci (case insensitive) versions of each function

    string_view before(char delim)
    {
        size_t pos = this->find(delim);
        return this->substr(0, this->find(delim));
    }

    string_view before(string_view delim)
    {
        return this->substr(0, this->find(delim));
    }

    string_view after(char delim)
    {
        size_t pos = this->find(delim);
        if (pos != std::string::npos)
        {
            return this->substr(pos + 1);
        }
        return "";
    }

    string_view after(string_view delim)
    {
        size_t pos = this->find(delim);
        if (pos != std::string::npos)
        {
            return this->substr(pos + delim.length());
        }
        return "";
    }

    string_view& ltrim(char delim)
    {
        while (this->front() == delim)
        {
            this->remove_prefix(1);
        }
        return *this;
    }
   
    string_view& rtrim(char delim)
    {
        while (this->back() == delim)
        {
            this->remove_suffix(1);
        }
        return *this;
    }

};

};
