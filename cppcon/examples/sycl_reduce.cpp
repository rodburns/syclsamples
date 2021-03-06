/*
Copyright 2018 Gordon Brown

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <benchmark.h>
#include <sycl_execution>

constexpr int size = 4194304;
constexpr int iterations = 10;

class reduce;

TEST_CASE("cppcon::reduce(sycl)", "gpu_reduce") {
  int result{0};
  int expected{0};

  {
    auto input = std::vector<int>(size);

    cppcon::init_data(input, [](int &value, unsigned index) { value = index % 16; });

    cppcon::sycl<reduce> syclPolicy;

    cppcon::benchmark(
        [&]() {
          result = cppcon::reduce(syclPolicy, input.begin(), input.end(), 42,
                                  std::plus<>());
        },
        iterations, "cppcon::reduce(sycl)");
  }

  {
    auto input = std::vector<int>(size);

    cppcon::init_data(input, [](int &value, unsigned index) { value = index % 16; });

    cppcon::benchmark(
        [&]() { expected = std::accumulate(input.begin(), input.end(), 42); },
        iterations, "td::accumulate");
  }

  REQUIRE(result == expected);
}
