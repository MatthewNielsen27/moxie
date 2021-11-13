/**
 *  @author Matthew Nielsen
 *  @date   2021-11-12
 *
 *  An Alias Table implementation for efficient sampling from a cdf.
 */
#pragma once

#include <vector>
#include <random>
#include <set>


namespace moxie::Core {

/**
 *  @short  An alias table can be used to efficiently sample vales from a cdf (cumulative distribution function)
 *          This object takes O(n logn) time to build, and constant O(1) time to sample.
 *
 *  @cite   https://en.wikipedia.org/wiki/Alias_method
 */
class AliasTable {
public:
    explicit AliasTable(const std::vector<double>& probabilities);

    //! @short  Sample an element from the collection
    [[nodiscard]] std::size_t sample(std::mt19937& rng) const;

    //! @short Sample n distinct elements from the collection
    [[nodiscard]] std::set<std::size_t> sampleDistinct(std::mt19937& rng, size_t n) const;

private:
    std::vector<std::size_t> m_alias;
    std::vector<double>      m_weights;
};

} // namespace moxie::Core
