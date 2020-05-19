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
    // v: n+m
    // n: number of normal & help variables,
    // m: number of slack variables/limits
    size_t v = count_tableau_variables(model.vars);
    size_t m = count_variables_of_type(model.vars, SLACK);
    size_t n = v-m;

    tableau_t tableau {model /* rest 0 */};
    //tableau.cj_row = matrix_create(n+m, 1);
    tableau.variables.resize(n+m);
    tableau.basis_variables.resize(m);
    tableau.inner = matrix_create(n+m+1, m+1);

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
            tableau.inner.at(j, i) = get_coefficient(restriction.func, var);
            j++;
        }

        // fill in rs column
        tableau.inner.at(n+m, i) = restriction.limit;
        i++;
    }

    // fill in delta-z row
    size_t j = 0;
    for(auto& var : tableau.variables)
    {
        tableau.inner.at(j, m) = -get_coefficient(model.objective_function.objective_linear, var);
        j++;
    }

    // fill in result cell
    tableau.inner.at(n+m, m) = 0;

    return tableau; // RVO
}
