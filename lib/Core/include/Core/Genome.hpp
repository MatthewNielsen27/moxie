/**
 *  @author Matthew Nielsen
 *  @date   2021-11-11
 *
 *  A generic Genome component.
 */
#pragma once

#include <functional>


namespace moxie::Core {

//! This class represents a single gene
template <typename T>
class Genome {
public:
    using MutationFunction = std::function<T(T)>;

    //! These are the constructors for the gene
    Genome(T value, MutationFunction mutator) : m_Value(value), m_MutationFn(mutator) {}
    Genome(const Genome<T>& other) = default;
    Genome(Genome<T>&& other) noexcept = default;
    ~Genome() = default;

    //! This is the value accessor for the gene
    [[nodiscard]] virtual const T& value() const { return m_Value; }

    //! This function is called to perform a mutation on the gene
    virtual void mutate() { m_Value = m_MutationFn(m_Value); }

    [[nodiscard]] virtual bool operator==(const Genome<T>& other) const { return m_Value == other.value(); }
    [[nodiscard]] virtual bool operator!=(const Genome<T>& other) const { return m_Value != other.value(); }

private:
    // This is the function used to mutate the gene
    MutationFunction m_MutationFn;
    T m_Value;
};

}
