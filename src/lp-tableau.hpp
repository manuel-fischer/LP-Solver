#pragma once

using column_t = std::vector<number_t>;

struct matrix_t
{
    /*private*/ size_t m_width;

    std::vector<big_number_t> numbers;

    size_t width() const { return m_width; }
    size_t height() const { return numbers.size()/m_width; }
    size_t size() const { return numbers.size(); }

    //big_number_t const& at(size_t x, size_t y) const { return numbers[x + m_width*y]; }
    //big_number_t& at(size_t x, size_t y) { return numbers[x + m_width*y]; }
    big_number_t const& _(size_t y, size_t x) const { return numbers[x + m_width*y]; }
    big_number_t& _(size_t y, size_t x) { return numbers[x + m_width*y]; }

};

struct tableau_t
{
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    lp_model_t const* model;
    //matrix_t cj_row;                    // n x 1
    std::vector<var_t> variables;       // n
    std::vector<var_t> basis_variables; // m
    matrix_t inner;                     // n+1 x m+1, contains rs column, delta-z row and z value


    big_number_t const& a_(size_t y, size_t x) const { return inner._(y,x); }
    big_number_t&       a_(size_t y, size_t x)       { return inner._(y,x); }
    big_number_t const& b_(size_t y) const { return inner._(y,variables.size()); }
    big_number_t&       b_(size_t y)       { return inner._(y,variables.size()); }
    // delta-z
    big_number_t const& dz_(size_t x) const { return inner._(basis_variables.size(),x); }
    big_number_t&       dz_(size_t x)       { return inner._(basis_variables.size(),x); }
};
