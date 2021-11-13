/**
 *  @author Matthew Nielsen
 *  @date   2021-11-11
 *
 *  Simple example Genetic Algorithm (GA) optimizer using moxie components.
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "Core/Genome.hpp"
#include "Core/Crossover.hpp"

using namespace moxie::Core;


static constexpr size_t dimensions = 2;
static constexpr double domain_upper = 10.0;
static constexpr double domain_lower = -10.0;


using Candidate = std::vector<Genome<double>>;
using Population = std::vector<Candidate>;

// This is a fitness function based on Xin-She Yang N.4
// https://towardsdatascience.com/optimization-eye-pleasure-78-benchmark-test-functions-for-single-objective-optimization-92e7ed1d1f12
double fitness(const Candidate& candidate) {
    double left_a = 0;
    double left_b = 0;
    double right_a = 0;
    for (const auto& gene : candidate) {
        left_a += pow(sin(gene.value()), 2);
        left_b += pow(gene.value(), 2);
        right_a += pow(sin(sqrt(abs(gene.value()))), 2);
    }

    return (left_a - exp(-1 * left_b)) * (exp(-1 * right_a));
}


int main(const int argc, const char** argv) {
    const auto known_optimum = Candidate(dimensions, Genome{0.0});

    constexpr std::size_t population_size = 100;

    Population foo;

    std::random_device rd;
    std::seed_seq seq{rd(), rd()};
    std::mt19937 rng{seq};

    std::uniform_real_distribution<double> variance{-0.01, 0.01};
    std::uniform_real_distribution<double> domain{-10, 10};

    // This is the mutation we are going to apply to each gene in the sequence
    auto random_mutation = [&](double value) { return std::clamp(value + variance(rng), domain.min(), domain.max()); };

    // Creates a random candidate
    auto makeCandidate = [&]() {
        Candidate candidate; candidate.reserve(dimensions);
        for (std::size_t i = 0; i < dimensions; ++i) { candidate.emplace_back(domain(rng)); }
        return candidate;
    };

    // Create the initial population, populating it with random candidates
    Population pop_current;      pop_current.reserve(population_size);
    Population pop_next;            pop_next.reserve(population_size);

    for (std::size_t i = 0; i < population_size; ++i) { pop_current.push_back(makeCandidate()); }

    // This will store the fitness of each member of the population
    std::vector<double> pop_fitness(population_size, 0.0);

    // --
    // We are going to simulate evolution of the population over 100 generations
    for (auto generation_i = 0; generation_i < 2; ++generation_i) {
        // Calculate the average fitness of each member of the population
        pop_fitness.clear();
        for (const auto& candidate : pop_current) { pop_fitness.push_back(fitness(candidate)); }

        // we need to select n/2 individuals from the population
        pop_next.clear();

        const auto avg_fitness = std::reduce(pop_fitness.begin(), pop_fitness.end()) / population_size;


        for (const auto& candidate : pop_current) {

        }

        std::cout << "generation: " << generation_i << "\t avg fitness: " << avg_fitness << std::endl;
    }
}
