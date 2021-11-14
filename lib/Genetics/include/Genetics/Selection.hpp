/**
 *  @author Matthew Nielsen
 *  @date   2021-11-12
 *
 *  Selection algorithms.
 */
#pragma once

#include <algorithm>
#include <random>

#include "Util/AliasTable.hpp"

#include <unordered_map>
#include <set>


namespace moxie::Genetics::Selection {

//! Perform tournament selection
[[nodiscard]] std::vector<std::size_t>
        tournament_selection(const std::vector<double>& fitness,
                             const std::size_t& n,
                             const std::size_t& k,
                             double p,
                             std::mt19937& rng);



//! @short  Sample n elements from the population uniformly at random (without replacement).
template <typename T>
[[nodiscard]] std::vector<T>
        universal_sampling(const std::vector<T>& population,
                           const std::size_t& n,
                           std::mt19937& rng);

/**
 *  @short  Returns the n most fit members of the population.
 */
template <typename T>
[[nodiscard]] std::vector<T>
        truncate(const std::vector<T>& population,
                 const std::vector<double>& fitness,
                 const std::size_t& n);

/**
 *  @short  Returns the indices of the n most fit members of the population.
 */
[[nodiscard]] std::vector<std::size_t>
        truncate(const std::vector<double>& fitness,
                 const std::size_t& n);


/**
 *  @short  Returns n distinct members of the population, sampled with
 *          probabilities proportional to their relative fitness.
 */
template <typename T>
[[nodiscard]] std::vector<T>
        proportional_selection(const std::vector<T>& population,
                               const std::vector<double>& fitness,
                               const std::size_t& n,
                               std::mt19937& rng);

/**
 *  @short  Returns the indices of n distinct members of the population, sampled with
 *          probabilities proportional to their relative fitness.
 */
[[nodiscard]] std::vector<std::size_t>
        proportional_selection(const std::vector<double>& fitness,
                               const std::size_t& n,
                               std::mt19937& rng);


//! @short  Convert an array of objective values (lower is better) to relative fitness values (higher is better)
[[maybe_unused]]
[[nodiscard]] std::vector<double>
objective_value_fitness(const std::vector<double>& values) {
    // Determine the maximum objective value
    const auto max_value = std::max(values.begin(), values.end());

    // Convert objective value to fitness (relative fitness is measure of distance to max objective value)
    std::vector<double> out{}; out.reserve(values.size());
    std::for_each(values.begin(), values.end(), [&](auto value) { out.push_back(*max_value - value); });

    return out;
}

//! @short  Convert an array of objective values (lower is better) to relative fitness values (higher is better)
[[maybe_unused]]
[[nodiscard]] std::vector<double>
normalize_fitness(const std::vector<double>& fitness) {
    // Determine the sum of population fitness
    const auto sum = std::accumulate(fitness.begin(), fitness.end(), 0.0);

    // Normalize population fitness with respect to this sum
    std::vector<double> normalized_fitness{}; normalized_fitness.reserve(fitness.size());
    std::for_each(fitness.begin(), fitness.end(), [&](auto item) { normalized_fitness.push_back(item / sum); });

    return normalized_fitness;
}




// --
// Function definitions to follow

template <typename T>
std::vector<T> universal_sampling(const std::vector<T>& population,
                                  const std::size_t& n, std::mt19937& rng) {
    // --
    // Error check our inputs
    if (n > population.size()) { throw std::invalid_argument("n cannot be larger than population size"); }

    // Sample the population uniformly without replacement
    std::vector<T> out{}; out.reserve(n);
    std::sample(population.begin(), population.end(), std::back_inserter(out), n, rng);

    return out;
}


std::vector<std::size_t> truncate(const std::vector<double>& fitness,
                                  const std::size_t& n) {
    // --
    // Error check our inputs
    if (n > fitness.size()) { throw std::invalid_argument("n cannot be larger than population size"); }

    // Perform a partial sort on the indexes, comparing the fitness they represent
    std::vector<std::size_t> indices(fitness.size(), 0);
    std::iota(indices.begin(), indices.end(), 0);

    std::partial_sort(indices.begin(),
                      indices.begin() + n,
                      indices.end(),
                      [&fitness](auto i, auto j) { return fitness[i] > fitness[j]; }
                      );

    return {indices.begin(), indices.begin() + n};
}


template <typename T>
std::vector<T> truncate(const std::vector<T>& population,
                        const std::vector<double>& fitness,
                        const std::size_t& n) {
    // Perform the selection
    const auto selection = truncate(fitness, n);

    // Copy the selected members of the population to the output container
    std::vector<T> out; out.reserve(n);
    std::for_each(selection.begin(), selection.end(), [&](auto i) { out.push_back(population[i]); });

    return out;
}

std::vector<std::size_t> proportional_selection(const std::vector<double>& fitness,
                                                const std::size_t& n, std::mt19937& rng) {
    // Use an AliasTable to perform efficient selection using the cdf.
    const auto selection = Util::AliasTable{normalize_fitness(fitness)}.sampleDistinct(rng, n);
    return {selection.begin(), selection.end()};
}

template <typename T>
std::vector<T> proportional_selection(const std::vector<T>& population,
                                      const std::vector<double>& fitness,
                                      const std::size_t& n,
                                      std::mt19937& rng) {
    // Perform the selection
    const auto selection = proportional_selection(fitness, n, rng);

    // Copy the selected members of the population to the output container
    std::vector<T> out; out.reserve(n);
    std::for_each(selection.begin(), selection.end(), [&](auto i) { out.push_back(population[i]); });

    return out;
}


std::vector<std::size_t> tournament_selection(const std::vector<double>& fitness,
                                              const std::size_t& n,
                                              const std::size_t& k,
                                              double p,
                                              std::mt19937& rng) {

    std::set<std::size_t> selected{};

    std::uniform_int_distribution<std::size_t> indices{0, fitness.size()};

    auto cmp = [&](auto i, auto j) { return fitness[i] > fitness[j]; };

    // --
    // This lambda will return a vector of 'k' unselected members sorted by fitness
    auto create_tournament = [&]() -> std::vector<size_t> {
        std::set<std::size_t, decltype(cmp)> tournament_members;

        while (tourney.size() < k) {
            const auto i = indices(rng);
            if (selected.find(i) != selected.end()) { tournament_members.insert(i); }
        }

        return {tournament_members.begin(), tournament_members.end()};
    };

    // --
    // Create an AliasTable that we can re-use across the tournaments because
    // the cdf remains constant
    auto cdf = std::vector<double>{p};
    for (std::size_t i = 1; i < k; ++i) cdf.push_back(cdf.back() * p_prime);

    auto aliasTable = Util::AliasTable(cdf);

    // --
    // Repeat tournaments until we have selected enough people
    while (selected.size() < k) {
        // Select the tournament members
        const auto tournament_members = create_tournament();

        // Select the winner of the tournament by sampling the AliasTable
        const auto winner = tournament_members[aliasTable.sample(rng)];
        selected.insert(winner);
    }

    return {selected.begin(), selected.end()};
}

} // namespace moxie::Genetics::Selection
