#include <catch2/catch_all.hpp>

#include "Core/Genome.hpp"


using namespace moxie::Core;


TEST_CASE("Genome: mutation") {
    // Test identity mutation
    {
        auto identity = [](auto foo) { return foo; };
        const auto value = 1;

        auto gene = Genome<int>{value, identity};

        REQUIRE(gene.value() == value);
        gene.mutate();
        REQUIRE(gene.value() == value);
    }

    // Test algebraic mutation
    {
        auto add_1 = [](auto foo) { return foo + 1; };
        const auto value = 1;

        auto gene = Genome<int>{value, add_1};

        REQUIRE(gene.value() == value);
        gene.mutate();
        REQUIRE(gene.value() == value + 1);
    }
}

TEST_CASE("Genome: operators") {
    auto identity = [](auto foo) { return foo; };

    const auto foo = Genome<double>{1.0, identity};
    const auto bar = Genome<double>{2.0, identity};

    REQUIRE(foo == foo);
    REQUIRE(foo != bar);
}
