#include "Util/AliasTable.hpp"


namespace moxie::Util {

AliasTable::AliasTable(const std::vector<double>& probabilities)
        : m_alias(probabilities.size(), 0),
          m_weights(probabilities.size(), 0.0) {

    const auto k = probabilities.size();
    const auto ratio = 1.0 / static_cast<double>(k);

    // --
    // We begin by sorting the entries into 2 bins
    std::vector<size_t> smaller, larger;
    for (std::size_t i = 0; i < k; ++i) {
        m_weights[i] = static_cast<double>(k) * probabilities[i];

        if (m_weights[i] < 1.0) {
            smaller.push_back(i);
        } else {
            larger.push_back(i);
        }
    }

    // --
    // Now we want to interleave smaller and larger elements
    while (!smaller.empty() && !larger.empty()) {
        const auto small = smaller.back(); smaller.pop_back();
        const auto large = larger.back(); larger.pop_back();

        // Place the larger item at the smaller item's index
        m_alias[small] = large;

        // Offset the remaining weight contribution of the larger item with the weight of the element it displaced
        m_weights[large] = m_weights[large] - (1.0 - m_weights[small]);

        if (m_weights[large] < 1.0) {
            smaller.push_back(large);
        } else {
            larger.push_back(large);
        }
    }
}

inline std::size_t AliasTable::sample(std::mt19937& rng) const {
    std::uniform_real_distribution<double>      random_weight{0.0, 1.0};
    std::uniform_int_distribution<std::size_t>  random_index{0, m_alias.size() - 1};

    const auto index = random_index(rng);
    if (random_weight(rng) < m_weights[index]) {
        return index;
    } else {
        return m_alias[index];
    }
}

std::set<std::size_t> AliasTable::sampleDistinct(std::mt19937& rng, std::size_t n) const {
    if (n > m_alias.size()) {
        throw std::invalid_argument("n is larger than number of possible elements");
    } else if (n == 0) {
        return {};
    } else if (n == m_alias.size()) {
        return {m_alias.begin(), m_alias.end()};
    }

    std::set<std::size_t> taken{};
    while (taken.size() < n) {
        taken.insert(sample(rng));
    }

    return taken;
}

} // namespace moxie::Util
