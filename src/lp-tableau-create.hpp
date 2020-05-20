#pragma once

#include "lp-tableau.hpp"
#include "lp-model-util.hpp"

#include "range-if.hpp"

#include <functional> // -> not_fn

matrix_t matrix_create(size_t width, size_t height)
{
    matrix_t ret;
    ret.numbers.resize(width*height);
    ret.m_width = width;
    return ret; // RVO
}

tableau_t tableau_create(lp_model_t const& model)
{
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = count_tableau_variables(model.vars);
    size_t m = count_variables_of_type(model.vars, SLACK);

    tableau_t tableau;
    //tableau.cj_row = matrix_create(n+m, 1);
    tableau.model = &model;
    tableau.variables.resize(n);
    tableau.basis_variables.resize(m);
    tableau.inner = matrix_create(n+1, m+1);

    // fill in the variable names
    std::copy_if(model.vars.begin(), model.vars.end(),
                 tableau.variables.begin(), is_tableau_variable(model.vars));

    std::copy_if(model.vars.begin(), model.vars.end(),
                 tableau.basis_variables.begin(), is_variable_of_type(model.vars, SLACK));

    // fill in the data

    // all limits that are not negativity conditions
    auto restrictions = range_if{model.limits, std::not_fn(is_non_negativity_condition)};

    size_t i = 0; // rows
    // Fill in BV rows
    for(auto& restriction : restrictions)
    {
        size_t j = 0;
        for(auto& var : tableau.variables)
        {
            tableau.inner._(i,j) = get_coefficient(restriction.func, var);
            j++;
        }

        // fill in rs column
        tableau.inner._(i,n) = restriction.limit;
        i++;
    }

    // fill in delta-z row
    size_t j = 0;
    for(auto& var : tableau.variables)
    {
        tableau.inner._(m,j) = -get_coefficient(model.objective_function.objective_linear, var);
        j++;
    }

    // fill in result cell
    tableau.inner._(m,n) = 0;

    return tableau; // RVO
}
