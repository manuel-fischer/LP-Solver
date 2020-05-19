#pragma once

using column_t = std::vector<number_t>;

struct matrix_t
{
    /*private*/ size_t m_width;

    std::vector<big_number_t> numbers;

    size_t width() const { return m_width; }
    size_t height() const { return numbers.size()/m_width; }
    size_t size() const { return numbers.size(); }

    big_number_t const& at(size_t x, size_t y) const { return numbers[x + m_width*y]; }
    big_number_t& at(size_t x, size_t y) { return numbers[x + m_width*y]; }

};

struct tableau_t
{
    // v: n+m
    // n: number of normal & help variables,
    // m: number of slack variables/limits
    lp_model_t const& model;
    //matrix_t cj_row;                    // n+m x 1
    std::vector<var_t> variables;       // n+m
    std::vector<var_t> basis_variables; // m
    matrix_t inner;                     // n+m+1 x m+1, contains rs column, delta-z row and z value
};
