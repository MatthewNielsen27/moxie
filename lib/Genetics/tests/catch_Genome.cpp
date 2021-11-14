#include <catch2/catch_all.hpp>

#include "Genetics/Genome.hpp"


using namespace moxie::Genetics;


TEST_CASE("Genome: can apply a simple mutation") {
    const auto value = 1;

    auto identity = [](auto foo) { return foo; };
    auto add_1    = [](auto foo) { return foo + 1; };

    auto gene = Genome{value};

    // Test identity mutation
    gene.mutate(identity);
    REQUIRE(gene.value() == identity(value));

    // Test simple arithmetic mutation
    gene.mutate(add_1);
    REQUIRE(gene.value() == add_1(value));
}

TEST_CASE("Genome: verify comparison operators") {
    const auto foo = Genome{1.0};
    const auto bar = Genome{2.0};
    REQUIRE(foo == foo);
    REQUIRE(foo != bar);
}
