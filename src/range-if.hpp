#pragma once

#include "iterator-if.hpp"

    template<class Container, class Pred>
struct range_if
{
    Container& container;
    Pred pred;

    constexpr auto begin() {
        return iterator_if{std::begin(container), std::end(container), pred};
    }
    constexpr auto begin() const {
        return iterator_if{std::cbegin(container), std::cend(container), pred};
    }

    constexpr auto end() {
        return iterator_if{std::end(container), std::end(container), pred};
    }
    constexpr auto end() const {
        return iterator_if{std::cend(container), std::cend(container), pred};
    }
};

    template<class Container, class Pred>
range_if(Container& container, Pred pred) -> range_if<Container, Pred>;
