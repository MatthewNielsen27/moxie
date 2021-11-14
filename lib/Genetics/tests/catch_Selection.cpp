#include <catch2/catch_all.hpp>

#include <set>

#include "Genetics/Selection.hpp"

using namespace moxie::Genetics;

TEST_CASE("uniform_selection: sanity") {
    const auto population = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd()};
    std::mt19937 rng{seq};

    SECTION("should return n distinct elements") {
        const auto num_elements = 4;
        const auto sample = Selection::universal_sampling(population, num_elements, rng);

        REQUIRE(std::set<int>{sample.begin(), sample.end()}.size() == num_elements);
    }

    SECTION("should raise an error if index is out of range") {
        REQUIRE_THROWS(Selection::universal_sampling(population, 20, rng));
    }
}

TEST_CASE("truncate: sanity") {
    const auto population = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto fitness = std::vector<double>{0.5, 0.1, 1.0, 3.0, 0.001, 0.9, 10.0, 0.7, 0.75, 10.0};

    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd()};
    std::mt19937 rng{seq};

    SECTION("should return n distinct elements") {
        const auto num_elements = 4;
        const auto sample = Selection::truncate(population, fitness, num_elements);

        const auto yielded = std::set<int>{sample.begin(), sample.end()};

        // Check that it returned the correct number of elements
        REQUIRE(yielded.size() == num_elements);

        // Check that it returned the correct elements
        for (const auto expected_value : {3, 4, 7, 10}) {
            auto it = yielded.find(expected_value);
            CHECK(it != yielded.end());
        }
    }

    SECTION("should raise an error if index is out of range") {
        REQUIRE_THROWS(Selection::universal_sampling(population, 20, rng));
    }
}


TEST_CASE("proportional_selection: sanity") {
    const auto population = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto fitness = std::vector<double>{0.5, 0.1, 1.0, 3.0, 0.001, 0.9, 10.0, 0.7, 0.75, 1000000.0};

    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd()};
    std::mt19937 rng{seq};

    SECTION("should return n distinct elements") {
        const auto num_elements = 4;
        const auto sample = Selection::proportional_selection(population, fitness, num_elements, rng);

        const auto yielded = std::set<int>{sample.begin(), sample.end()};

        // Check that it returned the correct number of elements
        REQUIRE(yielded.size() == num_elements);
        CHECK(yielded.find(10) != yielded.end());   // This may not always work
    }

    SECTION("should raise an error if index is out of range") {
        REQUIRE_THROWS(Selection::universal_sampling(population, 20, rng));
    }
}


TEST_CASE("tournament_selection: sanity") {
    const auto population = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto fitness = std::vector<double>{0.5, 0.1, 1.0, 3.0, 0.001, 0.9, 10.0, 0.7, 0.75, 1000000.0};

    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd()};
    std::mt19937 rng{seq};

    SECTION("should return n distinct elements") {
        const auto num_elements = 4;
        const auto sample = Selection::proportional_selection(population, fitness, num_elements, rng);

        const auto yielded = std::set<int>{sample.begin(), sample.end()};

        // Check that it returned the correct number of elements
        REQUIRE(yielded.size() == num_elements);
        CHECK(yielded.find(10) != yielded.end());   // This may not always work
    }

    SECTION("should raise an error if index is out of range") {
        REQUIRE_THROWS(Selection::universal_sampling(population, 20, rng));
    }
}
