/**
 *  @author Matthew Nielsen
 *  @date   2021-11-11
 *
 *  Genome splicing utility class.
 */
#pragma once

#include <random>


namespace moxie::Core::Crossover {

/**
 *  @short  This class provides an interface to splice sequences of DNA.
 */
class Splicer {
private:
    std::mt19937 m_rng;
public:
    Splicer();
    ~Splicer() = default;


    /**
     *  @short  Generates child DNA by performing a binary crossover at a random splice point between
     *          the parent DNA sequences.
     */
    template <typename T>
    [[nodiscard]] std::pair<T,T> binary_crossover(const T& parent_a, const T& parent_b);

    /**
     *  @short  Generates child DNA by performing a binary crossover at a specific splice point between
     *          the parent DNA sequences.
     */
    template <typename T>
    [[nodiscard]] static std::pair<T,T> binary_crossover(const T& parent_a,
                                                         const T& parent_b, std::size_t splice_point);

    /**
     *  @short  Generates child DNA by performing a uniform crossover with a given probability p.
     */
    template <typename T>
    [[nodiscard]] std::pair<T,T> uniform_crossover(const T& parent_a, const T& parent_b, const double p);

};


// --
// Implementations
template <typename T>
std::pair<T, T> Splicer::binary_crossover(const T& parent_a, const T& parent_b) {
    if (parent_a.size() != parent_b.size()) {
        throw std::range_error("cannot crossover sequences of different sizes");
    }

    std::uniform_int_distribution<std::size_t> distrib{0, parent_b.size()};

    return binary_crossover(parent_a, parent_b, distrib(m_rng));
}

template <typename Container>
std::pair<Container, Container> Splicer::binary_crossover(const Container& parent_a,
                                                          const Container& parent_b, std::size_t splice_point) {
    if (parent_a.size() != parent_b.size()) {
        throw std::range_error("cannot crossover sequences of different sizes");
    } else if (splice_point > parent_a.size()) {
        throw std::range_error("splice point not within bounds of parent");
    }

    Container child_a, child_b;
    child_a.reserve(parent_a.size());
    child_b.reserve(parent_a.size());

    std::copy(parent_a.begin(), parent_a.begin() + splice_point, std::back_inserter(child_b));
    std::copy(parent_b.begin(), parent_b.begin() + splice_point, std::back_inserter(child_a));

    std::copy(parent_a.begin() + splice_point, parent_a.end(),   std::back_inserter(child_a));
    std::copy(parent_b.begin() + splice_point, parent_b.end(),   std::back_inserter(child_b));

    return std::make_pair(std::move(child_a), std::move(child_b));
}


template <typename T>
std::pair<T,T> Splicer::uniform_crossover(const T &parent_a, const T &parent_b, const double p) {
    if (parent_a.size() != parent_b.size()) {
        throw std::range_error("cannot crossover sequences of different sizes");
    }

    std::bernoulli_distribution distrib{p};

    T child_a, child_b;
    child_a.reserve(parent_a.size());
    child_b.reserve(parent_a.size());

    for (std::size_t i = 0; i < parent_a.size(); ++i) {
        if (distrib(m_rng)) {
            // Swap this element
            child_a.push_back(parent_b[i]);
            child_b.push_back(parent_a[i]);
        } else {
            // Keep same element
            child_a.push_back(parent_a[i]);
            child_b.push_back(parent_b[i]);
        }
    }

    return std::make_pair(std::move(child_a), std::move(child_b));
}

} // namespace moxie::Core::Crossover
