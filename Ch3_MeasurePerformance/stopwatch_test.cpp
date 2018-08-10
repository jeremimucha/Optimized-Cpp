#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <random>
#include <algorithm>
#include "stopwatch.h"


std::uniform_int_distribution<unsigned> ud;
std::default_random_engine re{
    static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())
    };

template<typename Container>
void time_container(Container&& cont) noexcept
{
    
}

int main()
{
    constexpr auto count = 50000u;
    constexpr auto iters = 100u;
    auto vec = std::vector<unsigned>(count);
    // auto vec = std::array<unsigned, count>{};
    auto sw = Stopwatch{"std::sort", false};
    auto total = 0u;
    for (unsigned i = 0; i < iters; ++i) {
        std::generate(vec.begin(), vec.end(), []{return ud(re);});
        sw.start();
        std::sort(vec.begin(), vec.end());
        total += sw.stop();
    }
    std::cout << "Performance of std::sort on a vector of " << count << " elements,\n"
        << "measured over " << iters << " iterations:\n";
    std::cout << "Total time = " << total << "ms\n"
        << "Average = " << static_cast<double>(total) / iters << "ms\n";
}
