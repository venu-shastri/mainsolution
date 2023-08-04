#include <execution>
#include <iostream>

int main() {
    //[seq Execution may not be parallelized
    std::vector<int> x{5, 7, 6, 4, 8, 2};
    std::sort(std::execution::seq, x.begin(), x.end());
    std::copy(x.begin(), x.end(), std::ostream_iterator<int>{std::cout, " "});
    std::cout << '\n';
    //]

    //[par Execution may be parallelized
    std::vector<int> y{5, 7, 6, 4, 8, 2};
    std::sort(std::execution::par, y.begin(), y.end());
    std::copy(y.begin(), y.end(), std::ostream_iterator<int>{std::cout, " "});
    std::cout << '\n';
    //]

    //[par_unseq Execution may be parallelized, vectorized, or migrated across threads
    std::vector<int> z{5, 7, 6, 4, 8, 2};
    std::sort(std::execution::par_unseq, z.begin(), z.end());
    std::copy(z.begin(), z.end(), std::ostream_iterator<int>{std::cout, " "});
    std::cout << '\n';
    //]


    return 0;
}
