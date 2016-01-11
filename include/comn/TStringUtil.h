/*
 File:        TStringUtil.h
 Date:        2007-9-14
 Author:      
 Description:
 History:
 <author>        <time>      <desc>

 */

#ifndef TSTRINGUTIL__
#define TSTRINGUTIL__

#include <string>
#include <algorithm>
#include <vector>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef max
#undef max
#endif //max

namespace comn
{

using std::string;

class StringUtil
{
public:
    static int compareNoCase(const string& src, const string& dest)
    {
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
        return _stricmp( src.c_str(), dest.c_str() );
#else
        return strcasecmp(src.c_str(), dest.c_str());
#endif

    }

    static string mid(const string& src, size_t pos, size_t count =
            std::string::npos)
    {
        return src.substr(pos, (string::npos == count) ? src.size() : count);
    }

    static string left(const string& src, size_t count)
    {
        return src.substr(0, count);
    }

    static string right(const string& src, size_t count)
    {
        size_t start = std::max((size_t) 0, src.size() - count);
        return src.substr(start, count);
    }

    static void toUpper(string& src)
    {
        for (size_t idx = 0; idx < src.size(); ++idx)
        {
            src[idx] = toupper(src[idx]);
        }
    }

    static void toLower(string& src)
    {
        for (size_t idx = 0; idx < src.size(); ++idx)
        {
            src[idx] = tolower(src[idx]);
        }
    }

    static void makeReverse(string& src)
    {
        string strTmp(src);
        std::copy(strTmp.rbegin(), strTmp.rend(), src.begin());
    }

    static size_t remove(string& src, char tch)
    {
        size_t len = src.size();
        src.erase(std::remove(src.begin(), src.end(), tch), src.end());
        return (len - src.size());
    }

    static size_t insert(string& src, size_t idx, char tch)
    {
        string::iterator pos = getIteratorFromIndex(src, idx);
        src.insert(pos, tch);
        return src.size();
    }

    static size_t insert(string& src, size_t idx, const string& str)
    {
        string::iterator pos = getIteratorFromIndex(src, idx);
        src.insert(pos, str.begin(), str.end());
        return src.size();
    }

    //
    static size_t remove(string& src, size_t idx, size_t count = 1)
    {
        size_t nCountNew = std::max((size_t) 0, count);
        string::iterator posBegin = getIteratorFromIndex(src, idx);
        string::iterator posEnd = getIteratorFromIndex(src, idx + nCountNew);
        src.erase(posBegin, posEnd);
        return src.size();
    }

    static string::iterator getIteratorFromIndex(string& src, size_t idx)
    {
        string::iterator pos = src.begin();
        size_t len = src.size();
        if ((size_t) idx >= len)
        {
            ;//
            pos = src.end();
        }
        else
        {
            pos += idx;
        }
        return pos;
    }

    //
    static void trimLeft(string& src)
    {
        size_t i = 0;
        for (i = 0; i < src.size(); i++)
        {
            if (!isTrimChar(src[i]))
            {
                break;
            }
        }
        //
        if (i < src.size())
        {
            src.erase(0, i);
        }
        else
        {
            src.clear();
        }
    }

    //
    static void trimLeft(string& src, char tch)
    {
        size_t i = 0;
        for (i = 0; i < src.size(); i++)
        {
            if (src[i] != tch)
            {
                break;
            }
        }
        src.erase(0, i);
    }

    //
    static void trimRight(string& src, char tch)
    {
        size_t len = src.size();
        if (len > 0)
        {
            size_t i = len - 1;
            for (; i != string::npos; --i)
            {
                if (src[i] != tch)
                {
                    break;
                }
            }
            src.erase(i + 1, len);
        }
    }

    static void trimRight(string& src)
    {
        size_t len = src.size();
        if (len > 0)
        {
            size_t i = len - 1;
            for (; i >= 0; --i)
            {
                if (!isTrimChar(src[i]))
                {
                    break;
                }
            }
            src.erase(i + 1, len);
        }
    }

    static bool isTrimChar(char tch)
    {
        return ((tch == ' ') || (tch == '\t') || (tch == '\r') || (tch == '\n'));
    }

    static void trim(string& src)
    {
        trimLeft(src);
        trimRight(src);
    }

    static size_t replace(string& src, char chOld, char chNew)
    {
        size_t count = 0;
        for (size_t i = 0; i < src.size(); ++i)
        {
            if (src[i] == chOld)
            {
                src[i] = chNew;
                count++;
            }
        }
        return count;
    }

    static size_t replace(string& src, const string& strOld,
            const string& strNew)
    {
        size_t count = 0;
        size_t idx = src.find(strOld, 0);
        while (idx != string::npos)
        {
            src.replace(idx, strOld.size(), strNew);
            count++;
            idx += strNew.size();
            idx = src.find(strOld, idx);
        }
        return count;
    }

    static bool startsWith(const string& src, const string& target)
    {
        bool ret = false;
        if (src.size() >= target.size())
        {
            string tmp = src.substr(0, target.size());
            ret = (tmp.compare(target) == 0);
        }
        return ret;
    }

    static bool endsWith(const string& src, const string& target)
    {
        bool ret = false;
        if (src.size() >= target.size())
        {
            string tmp = src.substr(src.size() - target.size());
            ret = (tmp.compare(target) == 0);
        }
        return ret;
    }

    static bool ensureStartsWith(string& src, char ch)
    {
        bool touched = false;
        if (src.empty())
        {
            src.insert(0, 1, ch);
            touched = true;
        }
        else if (src[0] != ch)
        {
            src.insert(0, 1, ch);
            touched = true;
        }
        return touched;
    }

    static bool ensureEndsWith(string& src, char ch)
    {
        bool touched = false;
        if (src.empty())
        {
            src.append(1, ch);
            touched = true;
        }
        else if (src[src.length() - 1] != ch)
        {
            src.append(1, ch);
            touched = true;
        }
        return touched;
    }

    static bool ensureStartsWith(string& src, const string& prefix)
    {
        bool touched = false;
        if (src.empty())
        {
            src = prefix;
            touched = true;
        }
        else if (!startsWith(src, prefix))
        {
            src.insert(0, prefix);
            touched = true;
        }
        return touched;
    }

    static bool ensureEndsWith(string& src, const string& postfix)
    {
        bool touched = false;
        if (src.empty())
        {
            src = postfix;
            touched = true;
        }
        else if (!endsWith(src, postfix))
        {
            src.append(postfix);
            touched = true;
        }
        return touched;
    }



    static string getHead(const string& src, char ch)
    {
        size_t pos = src.find(ch);
        if (pos != string::npos)
        {
            return src.substr(0, pos);
        }
        return src;
    }

    static string getTail(const string& src, char ch)
    {
        size_t pos = src.find(ch);
        if (pos != string::npos)
        {
            return src.substr(pos+1);
        }
        return string();
    }

    static size_t split(const string& src, char ch, string& first, string& second)
    {
        if (src.empty())
        {
            return 0;
        }
        size_t pos = src.find(ch);
        if (pos != string::npos)
        {
            first = src.substr(0, pos);
            second = src.substr(pos+1);
            return 2;
        }
        first = src;
        return 1;
    }

    template<class Container>
    static size_t split(const string& src, char tch, Container& vect)
    {
        if (src.empty())
            return 0;

        size_t count = 0;

        size_t begin = 0;
        size_t pos = src.find(tch, begin);
        while (string::npos != pos)
        {
            string substr;
            if (pos - begin > 0)
            {
                substr = src.substr(begin, pos - begin);
            }
            vect.push_back(substr);
            count++;

            begin = pos + 1;
            pos = src.find(tch, begin);
        }

        if (begin < src.size())
        {
            vect.push_back(src.substr(begin, string::npos));
            count++;
        }
        return count;
    }

    template <class Container>
    static size_t splitWithSeparator(const string& src, const string& sp, Container& cont)
    {
        if (src.empty())
            return 0;

        size_t count = 0;

        size_t begin = 0;
        size_t pos = src.find(sp, begin);
        while (string::npos != pos)
        {
            string substr;
            if (pos - begin > 0)
            {
                substr = src.substr(begin, pos - begin);
            }
            cont.push_back(substr);
            count++;

            begin = pos + sp.size();
            pos = src.find(sp, begin);
        }

        if (begin < src.size())
        {
            cont.push_back(src.substr(begin, string::npos));
            count++;
        }
        return count;
    }

    static string extract(const string& src, size_t start, char chBegin, char chEnd, bool fullMatch)
    {
        string substr;
        size_t posBegin = src.find(chBegin, start);
        if (posBegin == string::npos)
        {
            return substr;
        }

        size_t posEnd = src.find(chEnd, posBegin + 1);
        if (posEnd == string::npos)
        {
            if (!fullMatch)
            {
                substr = src.substr(posBegin+1);
            }
        }
        else
        {
            substr = src.substr(posBegin+1, posEnd - posBegin - 1);
        }
        return substr;
    }


    static string format(const char* lpszFormat, ...)
    {
        const int MAX_FORMAT_CHAR = 512 * 4;
        char buffer[MAX_FORMAT_CHAR] = {0};
        memset(buffer, '\0', MAX_FORMAT_CHAR);

        va_list pars;
        va_start(pars, lpszFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
        vsprintf_s(buffer, MAX_FORMAT_CHAR, lpszFormat, pars);
#else
        vsprintf(buffer, lpszFormat, pars);
#endif
        va_end(pars);

        return buffer;
    }



};

} // end of namespace

#endif //TSTRINGUTIL__
