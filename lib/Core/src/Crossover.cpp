#include "Core/Crossover.hpp"

namespace moxie::Core::Crossover {

Splicer::Splicer() {
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd()};
    m_rng = std::mt19937{seed};
}

}
