/**
 *  @author Matthew Nielsen
 *  @date   2021-11-12
 *
 *  Selection algorithms.
 */
#pragma once

#include <algorithm>
#include <random>


namespace moxie::Genetics::Selection {

//! @short  Convert an array of objective values (lower is better) to relative fitness values (higher is better)
[[nodiscard]] std::vector<double> objective_value_fitness(const std::vector<double>& values);

//! @short  Normalize the vector of fitness so their sum totals 1.
[[nodiscard]] std::vector<double> normalize_fitness(const std::vector<double>& fitness);


/**
 *  @short  Returns n distinct members of the population, sampled by
 *          performing tournament selection in groups of size k with
 *          probability p.
 */
template <typename T>
[[nodiscard]] std::vector<T>
        tournament_selection(const std::vector<T>& population,
                             const std::vector<double>& fitness,
                             const std::size_t& n,
                             const std::size_t& k,
                             double p,
                             std::mt19937& rng);
/**
 *  @short  Returns the indices of n distinct members of the population,
 *          sampled by performing tournament selection in groups of size
 *          k with probability p.
 */
[[nodiscard]] std::vector<std::size_t>
        tournament_selection(const std::vector<double>& fitness,
                             const std::size_t& n,
                             const std::size_t& k,
                             double p,
                             std::mt19937& rng);

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



// --
// Function definitions to follow

template <typename T>
std::vector<T> universal_sampling(const std::vector<T>& population,
                                  const std::size_t& n,
                                  std::mt19937& rng) {
    // --
    // Error check our inputs
    if (n > population.size()) { throw std::invalid_argument("n cannot be larger than population size"); }

    // Sample the population uniformly without replacement
    std::vector<T> out{}; out.reserve(n);
    std::sample(population.begin(), population.end(), std::back_inserter(out), n, rng);

    return out;
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

template <typename T>
std::vector<T> tournament_selection(const std::vector<T>& population,
                                    const std::vector<double>& fitness,
                                    const std::size_t& n,
                                    const std::size_t& k,
                                    double p,
                                    std::mt19937& rng) {
    // Perform the selection
    const auto selection = tournament_selection(fitness, n, k, p, rng);

    // Copy the selected members of the population to the output container
    std::vector<T> out; out.reserve(n);
    std::for_each(selection.begin(), selection.end(), [&](auto i) { out.push_back(population[i]); });

    return out;
}


} // namespace moxie::Genetics::Selection
