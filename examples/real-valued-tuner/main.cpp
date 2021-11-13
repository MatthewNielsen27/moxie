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

#include "Core/Crossover.hpp"
#include "Core/Selection.hpp"

// This includes specific boilerplate code
#include "Candidate.h"


using namespace moxie::Core;


static constexpr std::size_t dimensions = 2;
static constexpr std::size_t population_size = 100;

// This is a fitness function based on Xin-She Yang N.4
// https://towardsdatascience.com/optimization-eye-pleasure-78-benchmark-test-functions-for-single-objective-optimization-92e7ed1d1f12
double f(const Candidate& candidate) {
    double left_a = 0;
    double left_b = 0;
    double right_a = 0;
    for (const auto& gene : candidate.genes) {
        left_a += pow(sin(gene.value()), 2);
        left_b += pow(gene.value(), 2);
        right_a += pow(sin(sqrt(abs(gene.value()))), 2);
    }

    return (left_a - exp(-1 * left_b)) * (exp(-1 * right_a));
}

// We can view the population as a vector of candidates
using Population = std::vector<Candidate>;


int main(const int argc, const char** argv) {
    // --
    // The known optimum for this problem is at [0,...,0]
    const auto known_optimum = []() {
        Candidate candidate;
        candidate.genes = std::vector<Candidate::GeneType>(dimensions, Genome{0.0});
        return candidate;
    }();

    Population foo;

    std::random_device rd;
    std::seed_seq seq{rd(), rd()};
    std::mt19937 rng{seq};

    std::uniform_real_distribution<double> domain{-10.0, 10.0};
    std::uniform_real_distribution<double> variance{-0.1, 0.1};
    std::bernoulli_distribution willMutate{0.2};

    // --
    // This is the mutation we are going to apply to each gene in the sequence
    auto random_mutation = [&](double value) {
        return !willMutate(rng) ? value : std::clamp(value + variance(rng), domain.min(), domain.max());
    };

    // --
    // Create the initial population, populating it with random candidates
    Population pop_current;      pop_current.reserve(population_size);
    Population pop_next;            pop_next.reserve(population_size);

    for (std::size_t i = 0; i < population_size; ++i) { pop_current.emplace_back(dimensions, domain, rng); }

    // This will store the fitness of each member of the population
    std::vector<double> pop_fitness(population_size, 0.0);

    Crossover::Splicer splicer{};

    // --
    // We are going to simulate evolution of the population over 100 generations
    for (auto generation_i = 0; generation_i < 100; ++generation_i) {
        // Calculate the average fitness of each member of the population
        for (std::size_t i = 0; i < pop_current.size(); ++i) { pop_fitness[i] = f(pop_current[i]); }

        const auto avg_fitness = std::reduce(pop_fitness.begin(), pop_fitness.end()) / population_size;
        std::cout << "generation: "    << generation_i
                  << "\tavg fitness: " << avg_fitness
                  << "\tmin: " << *std::min(pop_fitness.begin(), pop_fitness.end())
                  << "\tmax: " << *std::max(pop_fitness.begin(), pop_fitness.end()) << std::endl;



        // --
        // We need to select N/2 individuals from the population
        const auto num_survivors = population_size / 2;

        pop_next.clear();
        const auto selected_i = Selection::proportional_selection(pop_fitness,
                                                                  num_survivors,
                                                                  rng);

        std::for_each(selected_i.begin(), selected_i.end(), [&](auto i) { pop_next.push_back(pop_current[i]); });
        std::shuffle(pop_next.begin(), pop_next.end(), rng);

        static constexpr double p_entanglement = 0.37;

        // Now we need to generate pairwise members of the population
        for (std::size_t i = 0, j = 1; j < num_survivors; i+=2, j+=2) {
            const auto& parent_a = pop_next[i];
            const auto& parent_b = pop_next[j];

            auto [sequence_a, sequence_b] = splicer.uniform_crossover(parent_a.genes, parent_b.genes, p_entanglement);

            for (auto& gene : sequence_a) gene.mutate(random_mutation);
            for (auto& gene : sequence_b) gene.mutate(random_mutation);

            pop_next.emplace_back(std::move(sequence_a));
            pop_next.emplace_back(std::move(sequence_b));
        }

        pop_current.clear();
        pop_current = pop_next;
    }
}
