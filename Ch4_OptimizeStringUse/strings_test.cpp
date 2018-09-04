#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <algorithm>
#include "stopwatch.h"


// original, naive remove_ctrl()
std::string remove_ctrl(std::string s) {
    std::string result;
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result = result + s[i];
    }
    return result;
}

// remove_ctrl() with operator replaced by mutating assignment
std::string remove_ctrl_mutating(std::string s) {
    std::string result;
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
std::string remove_ctrl_mutating_it(std::string s) {
    std::string result;
    for (auto it=s.begin(); it != s.end(); ++it) {
        if (*it >= 0x20)
            result += *it;
    }
    return result;
}

std::string remove_ctrl_mutating_it_end(std::string s) {
    std::string result;
    for (auto it=s.begin(),end=s.end();it != end; ++it) {
        if (*it >= 0x20)
            result += *it;
    }
    return result;
}

// remove_ctrl_mutating() with space reserved in result
std::string remove_ctrl_reserve(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
std::string remove_ctrl_reserve_it(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (auto it=s.begin(),end=s.end();it != end; ++it) {
        if (*it >= 0x20)
            result += *it;
    }
    return result;
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs_it(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (auto it=s.begin(),end=s.end();it != end; ++it) {
        if (*it >= 0x20)
            result += *it;
    }
    return result;
}

// remove_ctrl_block_mutate() with reference arg
std::string remove_ctrl_blocks(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b,i-b);
    }
    return result;
}

std::string remove_ctrl_blocks_it(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (auto b=s.begin(),i=b,e=s.end(); b != e; b = i+1) {
        for (i=b; i!=e; ++i) {
            if (*i < 0x20) break;
        }
        result.append(b, i);
    }
    return result;
}

void remove_ctrl_block_ret_it(std::string& result, std::string const& s) {
    result.clear();
    result.reserve(s.length());
    for (auto b=s.begin(),i=b,e=s.end(); b != e; b = i+1) {
        for (i=b; i != e; ++i) {
            if (*i < 0x20) break;
        }
        result.append(b, i);
    }
}

std::string remove_ctrl_remove_if(std::string s)
{
    const auto new_end = std::remove_if(begin(s), end(s),
                                        [](char c)noexcept{return c >= 20;});
    s.erase(new_end, end(s));
    return s;
}

template<typename Func, typename... Args>
inline auto run_benchmark(Func&& func, const char* name, unsigned iters, Args&&... args) noexcept
{
    auto sw = Stopwatch{name};
    for (auto i = 0u; i < iters; ++i)
    {
        // auto res = std::forward<Func>(func(std::forward<Args>(args)...);
        if constexpr(std::is_void_v<std::invoke_result_t<Func, Args...>>) {
            std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        }
        else {
            const auto res = std::invoke(func, std::forward<Args>(args)...);
        }
    }
    sw.stop();
    const auto total = sw.lap_get();
    std::cout << name << ", " << iters << " iterations:\n"
        << "-   total   = " << total << " ms\n"
        << "-   average = " << static_cast<double>(total) / iters << " ms\n";
}

int main()
{
    const auto str_src =
        "This is\07 a test string \07containing some\r\ncontrol\b characters\07 to be removed. \07";
    const auto res_src =
        "This is a test string containing some control characters to be removed. ";
    const auto test_str = std::string{str_src} + str_src + str_src;
    const auto res_str  = std::string{res_src} + res_src + res_src;
    constexpr auto iterations = 100000u;

    // run_benchmark(remove_ctrl, "remove_ctr()", iterations, test_str);
    run_benchmark(remove_ctrl_mutating, "remove_ctrl_mutating()", iterations, test_str);
    run_benchmark(remove_ctrl_mutating_it, "remove_ctr_mutating_it()", iterations, test_str);
    run_benchmark(remove_ctrl_mutating_it_end, "remove_ctr_mutating_it_end", iterations, test_str);
    run_benchmark(remove_ctrl_reserve, "remove_ctrl_reserve()", iterations, test_str);
    run_benchmark(remove_ctrl_reserve_it, "remove_ctrl_reserve_it()", iterations, test_str);
    run_benchmark(remove_ctrl_refs, "remove_ctrl_refs()", iterations, test_str);
    run_benchmark(remove_ctrl_refs_it, "remove_ctrl_refs_it()", iterations, test_str);
    run_benchmark(remove_ctrl_remove_if, "remove_ctrl_remove_if()", iterations, test_str);
    run_benchmark(remove_ctrl_blocks, "remove_ctrl_blocks()", iterations, test_str);
    run_benchmark(remove_ctrl_blocks_it, "remove_ctrl_blocks_it()", iterations, test_str);
    auto temp = std::string{};
    run_benchmark(remove_ctrl_block_ret_it, "remove_ctrl_blocks_ret_it()", iterations,
                  temp, test_str);
}
