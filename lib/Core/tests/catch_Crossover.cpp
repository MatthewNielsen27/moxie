#include <catch2/catch_all.hpp>

#include <algorithm>

#include "Core/Crossover.hpp"
#include "Core/Genome.hpp"

using namespace moxie::Core;


TEST_CASE("binary_crossover: throws exception for sequences of differing size") {
    const auto sequence_a = std::vector<Genome<int>>{};
    const auto sequence_b = std::vector<Genome<int>>{ Genome{1}, Genome{2} };

    Crossover::Splicer splicer{};

    // This should throw because the sequences are not of equal sizes
    REQUIRE_THROWS(splicer.binary_crossover(sequence_a, sequence_b));

    // This shouldn't throw because the sequences are the same size
    REQUIRE_NOTHROW(splicer.binary_crossover(sequence_b, sequence_b));
}

TEST_CASE("binary_crossover: throws exception when splice_point is out of bounds") {
    const auto sequence = std::vector<Genome<int>>{ Genome{1}, Genome{2} };

    Crossover::Splicer splicer{};

    // This should throw because the splice_index is outside the bounds of the sequence
    REQUIRE_THROWS(splicer.binary_crossover(sequence, sequence, 4));

    // This shouldn't throw because the splice_index is within the bounds of the sequence
    REQUIRE_NOTHROW(splicer.binary_crossover(sequence, sequence, 0));
    REQUIRE_NOTHROW(splicer.binary_crossover(sequence, sequence, 1));
}

TEST_CASE("binary_crossover: splicing parents in half works") {
    const auto sequence_a = std::vector<Genome<int>>{10, Genome{0}};
    const auto sequence_b = std::vector<Genome<int>>{10,  Genome{1}};

    // Choose a splice point of in the middle of the sequence
    const auto splice_point = static_cast<std::size_t>(sequence_a.size() / 2);

    // Create children by splicing the parents
    const auto& [child_a, child_b] = Crossover::Splicer::binary_crossover(sequence_a, sequence_b, splice_point);

    // --
    // Check that the first half of child_a comes from parent_b and the latter half comes from parent_a
    REQUIRE(std::all_of(child_a.begin(), child_a.begin() + splice_point, [&](auto e) { return e == *sequence_b.begin(); }));
    REQUIRE(std::all_of(child_a.begin() + splice_point, child_a.end(),   [&](auto e) { return e == *sequence_a.begin(); }));

    // --
    // Check that the first half of child_b comes from parent_a and the latter half comes from parent_b
    REQUIRE(std::all_of(child_b.begin(), child_b.begin() + splice_point, [&](auto e) { return e == *sequence_a.begin(); }));
    REQUIRE(std::all_of(child_b.begin() + splice_point, child_b.end(),   [&](auto e) { return e == *sequence_b.begin(); }));
}
