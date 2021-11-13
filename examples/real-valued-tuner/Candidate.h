#pragma once

#include <vector>
#include <random>

#include "Core/Genome.hpp"

using namespace moxie::Core;

struct Candidate {
    //! This is the underlying Genome type (more complicated implementations can have multiple different types)
    using GeneType = Genome<double>;

    //! Each candidate solution contains a sequence of genes
    std::vector<GeneType> genes;

    //! This constructor creates a random candidate solution
    using Domain = std::uniform_real_distribution<double>;
    Candidate(std::size_t dimensions, Domain& domain, std::mt19937& rng) {
        genes.reserve(dimensions);
        for (std::size_t i = 0; i < dimensions; ++i) {
            genes.emplace_back(domain(rng));
        }
    }

    Candidate(std::vector<GeneType>&& values) : genes(std::move(values)) {}

    void mutate(const std::function<double(double)>& mutator) {
        for (auto& gene : genes) gene.mutate(mutator);
    }

    // These are the defaulted constructors
    Candidate() = default;
    Candidate(const Candidate& other) = default;
    ~Candidate() = default;
};
