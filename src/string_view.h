#pragma once

#include "aton.h"
#include "strings.h" 

#include <string_view>
#include <iostream>
#include <vector>

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

    string_view(const std::string_view &sv) :std::string_view(sv) {}

    string_view(const char* val, size_t len) :std::string_view(val, len) {}

    string_view(const char* val) :std::string_view(val) {}

    string_view(const std::string& val) :std::string_view(val) {}

    bool equals_ci(const string_view &sv)
    {
        size_t sz = sv.size();
        return sz == this->size() && 0 == strncasecmp(data(), sv.data(), sz);
    }

    bool starts_with_ci(const string_view &sv)
    {
        size_t sz = sv.size();
        // strncasecmp is non-standard think about exchanging with something from stl
        return sz <= this->size() && 0 == strncasecmp(data(), sv.data(), sz);
    }

    // wishy washy about split vs remove_until
    string_view split(char delim)
    {
        size_t pos = this->find(delim);
        string_view ret = this->substr(0, pos);
        if (pos >= this->size() - 1) // -1 to make zero based
            *this = string_view();
        else
            *this = this->substr(pos + 1);
        return ret;
    }

    string_view rsplit(char delim)
    {
        size_t pos = this->rfind(delim);
        string_view ret = this->substr(0, pos);
        if (pos >= this->size() - 1) // -1 to make zero based
            *this = string_view();
        else
            *this = this->substr(pos + 1);
        return ret;
    }

    size_t split(char delim, std::vector<string_view> &vec)
    {
        size_t pos = this->find(delim);
        size_t start = 0;
        size_t cnt = 0;
        while (pos != std::string_view::npos)
        {
            vec.push_back(this->substr(start, pos - start));
            start = pos + 1;
            pos = this->find(delim, start);
            cnt++;
        }

        if (start < this->size())
        {
            vec.push_back(this->substr(start));
            cnt++;
        }
        return cnt; 
    }

    size_t split(string_view delim, std::vector<string_view> &vec)
    {
        size_t pos = this->find(delim);
        size_t start = 0;
        size_t cnt = 0;
        size_t delim_size = delim.size();
        while (pos != std::string_view::npos)
        {
            vec.push_back(this->substr(start, pos - start));
            start = pos + delim_size;
            pos = this->find(delim, start);
            cnt++;
        }

        if (start < this->size()) // no more delims so just push back
        {
            vec.push_back(this->substr(start));
            cnt++;
        }
        return cnt;
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
            return this->substr(pos + delim.size());
        }
        return "";
    }

    string_view& ltrim(char val)
    {
        while (this->front() == val)
        {
            this->remove_prefix(1);
        }
        return *this;
    }

    string_view& ltrim(string_view vals)
    {
        size_t pos = this->find_first_not_of(vals);
        // 0, N, npos
        this->remove_prefix(pos);
        return *this;
    }
   
    string_view& rtrim(char val)
    {
        while (this->back() == val)
        {
            this->remove_suffix(1);
        }
        return *this;
    }

    string_view& rtrim(string_view vals)
    {
        size_t pos = this->find_last_not_of(vals);
        if (pos == npos)
        {
            *this = string_view();
        }
        else
        {
            this->remove_suffix(this->size() - pos - 1);
        }
        return *this;
    }

}; // end string_view

}; // end namespace dsy
