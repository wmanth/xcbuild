/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree.
 */

#ifndef __libutil_Base_h
#define __libutil_Base_h

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <cctype>

namespace libutil {

// trim from start
static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                [](int c) { return !std::isspace(c); }));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                [](int c) { return !std::isspace(c); }).base(),
            s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

template<typename T, typename U>
static inline std::unique_ptr<T>
static_unique_pointer_cast(std::unique_ptr<U> &&p)
{
    return std::unique_ptr<T>(static_cast<T *>(p.release()));
}

}

#endif  // !__libutil_Base_h
