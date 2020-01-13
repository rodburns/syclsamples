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
#include <std_execution>

constexpr int size = 2097152;
constexpr int iterations = 10;

TEST_CASE("cppcon::transform(seq)", "sequential_transform") {
  std::vector<int> result(size);
  std::vector<int> expected(size);

  {
    auto input = std::vector<int>(size);

    cppcon::init_data(input,
                      [](int &value, unsigned index) { value = index % 16; });

    cppcon::benchmark(
        [&]() {
          cppcon::transform(cppcon::seq, input.begin(), input.end(),
                            result.begin(), [](int &in) { return in * 2; });
        },
        iterations, "cppcon::transform(seq)");
  }

  {
    auto input = std::vector<int>(size);

    cppcon::init_data(input,
                      [](int &value, unsigned index) { value = index % 16; });

    cppcon::benchmark(
        [&]() {
          std::transform(input.begin(), input.end(), expected.begin(),
                         [](int &in) { return in * 2; });
        },
        iterations, "std::transform");
  }

  for (int i = 0; i < size; i++) {
    REQUIRE(result[i] == expected[i]);
  }
}
