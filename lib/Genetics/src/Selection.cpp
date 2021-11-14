#include "Genetics/Selection.hpp"

#include "Util/AliasTable.hpp"

#include <set>
#include <algorithm>



namespace moxie::Genetics::Selection {

std::vector<std::size_t> truncate(const std::vector<double>& fitness,
                                  const std::size_t& n) {
    // --
    // Error check our inputs
    if (n > fitness.size()) { throw std::invalid_argument("n cannot be larger than population size"); }

    // Perform a partial sort on the indexes, comparing the fitness they represent
    std::vector<std::size_t> indices(fitness.size(), 0);
    std::iota(indices.begin(), indices.end(), 0);

    std::partial_sort(indices.begin(),
                      indices.begin() + static_cast<std::vector<double>::difference_type>(n),
                      indices.end(),
                      [&fitness](auto i, auto j) { return fitness[i] > fitness[j]; }
    );

    return {indices.begin(), indices.begin() + static_cast<std::vector<double>::difference_type>(n)};
}

std::vector<std::size_t> proportional_selection(const std::vector<double>& fitness,
                                                const std::size_t& n, std::mt19937& rng) {
    // Use an AliasTable to perform efficient selection using the cdf.
    const auto selection = Util::AliasTable{normalize_fitness(fitness)}.sampleDistinct(rng, n);
    return {selection.begin(), selection.end()};
}

std::vector<std::size_t> tournament_selection(const std::vector<double>& fitness,
                                              const std::size_t& n,
                                              const std::size_t& k,
                                              double p,
                                              std::mt19937& rng) {
    // --
    // Error check our inputs
    if (p < 0 || p > 1) {
        throw std::invalid_argument("probability must be between 0 and 1");
    } else if (k == 0) {
        throw std::invalid_argument("tournament size must be greater than 0");
    } else if (n > fitness.size()) {
        throw std::invalid_argument("n cannot be larger than population size");
    } else if (k > fitness.size()) {
        throw std::invalid_argument("tournament size cannot be larger than population size");
    }

    std::set<std::size_t> selected{};

    std::uniform_int_distribution<std::size_t> indices{0, fitness.size()};

    auto cmp = [&](auto i, auto j) {
        if (fitness[i] == fitness[j]) {
            return i > j;
        }
        return fitness[i] > fitness[j];
    };

    // --
    // This lambda will return a vector of 'k' unselected members sorted by fitness
    auto create_tournament = [&]() -> std::vector<size_t> {
        std::set<std::size_t, decltype(cmp)> tournament_members(cmp);

        while (tournament_members.size() < k) {
            const auto i = indices(rng);
            if (selected.find(i) == selected.end()) {
                tournament_members.emplace(i);
            }
        }

        return {tournament_members.begin(), tournament_members.end()};
    };

    // --
    // Create an AliasTable that we can re-use across the tournaments because
    // the cdf remains constant
    auto cdf = std::vector<double>{p};
    const auto p_prime = 1 - p;
    for (std::size_t i = 1; i < k; ++i) cdf.push_back(cdf.back() * p_prime);

    auto aliasTable = Util::AliasTable(cdf);

    // --
    // Repeat tournaments until we have selected enough people
    for (std::size_t i = 0; i < n; ++i) {
        // Select the tournament members
        const auto tournament_members = create_tournament();

        // Select the winner of the tournament by sampling the AliasTable
        const auto winner = tournament_members[aliasTable.sample(rng)];
        selected.insert(winner);
    }

    return {selected.begin(), selected.end()};
}

std::vector<double> objective_value_fitness(const std::vector<double>& values) {
    // Determine the maximum objective value
    const auto max_value = std::max(values.begin(), values.end());

    // Convert objective value to fitness (relative fitness is measure of distance to max objective value)
    std::vector<double> out{}; out.reserve(values.size());
    std::for_each(values.begin(), values.end(), [&](auto value) { out.push_back(*max_value - value); });

    return out;
}


std::vector<double> normalize_fitness(const std::vector<double>& fitness) {
    // Determine the sum of population fitness
    const auto sum = std::accumulate(fitness.begin(), fitness.end(), 0.0);

    // Normalize population fitness with respect to this sum
    std::vector<double> normalized_fitness{}; normalized_fitness.reserve(fitness.size());
    std::for_each(fitness.begin(), fitness.end(), [&](auto item) { normalized_fitness.push_back(item / sum); });

    return normalized_fitness;
}


} // namespace moxie::Genetics::Selection
