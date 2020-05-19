#pragma once

    template<class Iter, class Pred>
struct iterator_if
{
    Iter iter;
    Iter end;
    Pred pred;

    constexpr iterator_if(iterator_if const&) = default;
    constexpr iterator_if(iterator_if&&) = default;

    iterator_if(Iter iter, Iter end, Pred pred)
      : iter(iter),
        end(end),
        pred(pred)
    {
        advance_to_true();
    }

    void advance_to_true()
    {
        iter = std::find_if(iter, end, pred);
    }

    iterator_if& operator++()
    {
        iter++;
        advance_to_true();

        return *this;
    }

    auto operator*() -> decltype(auto)
    {
        return *iter;
    }

    auto operator*() const -> decltype(auto)
    {
        return *iter;
    }

    constexpr bool operator==(iterator_if const& other) const
    {
        return iter==other.iter;
    }

    constexpr bool operator!=(iterator_if const& other) const
    {
        return iter!=other.iter;
    }
};

    template<class Iter, class Pred>
iterator_if(Iter iter, Iter end, Pred pred) -> iterator_if<Iter, Pred>;
