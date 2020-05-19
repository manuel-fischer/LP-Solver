#pragma once

// simple iterator over numbers

    template<class Int>
struct iota_iterator
{
    Int value;

    constexpr iota_iterator& operator++()
    {
        ++value;
        return *this;
    }


    constexpr Int operator*() const
    {
        return value;
    }

    constexpr bool operator==(iota_iterator const& other) const
    {
        return value == other.value;
    }
    constexpr bool operator!=(iota_iterator const& other) const
    {
        return value != other.value;
    }
};
