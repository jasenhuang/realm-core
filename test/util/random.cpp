/*************************************************************************
 *
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#include <ctime>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "random.hpp"


namespace realm {
namespace test_util {


unsigned long produce_nondeterministic_random_seed()
{
    typedef unsigned long ulong;
    ulong value = ulong(time(0));

#ifdef _WIN32
    value ^= ulong(GetCurrentProcessId());
#else
    value ^= ulong(getpid());
#endif

    return value;
}


} // namespace test_util

namespace _impl {


GlobalRandom& GlobalRandom::get() noexcept
{
    static GlobalRandom r;
    return r;
}


} // namespace _impl
} // namespace realm
