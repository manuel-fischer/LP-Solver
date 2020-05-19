#pragma once

#include "lp-tableau.hpp"
#include "range-if.hpp"


// is optimal
bool is_dually_valid(tableau_t const& tableau)
{

}


std::pair<size_t, size_t> select_pivot(tableau_t const& tableau)
{
    // n: number of columns = number of normal & help variables
    // m: number of row     = number of normal, help & slack variables/limits
    size_t n = tableau.variables.size();
    size_t m = tableau.basis_variables.size();


    auto is_basis_variable = [&](var_t var) {
        return std::find(tableau.basis_variables.begin(), tableau.basis_variables.end(), var) != tableau.basis_variables.end();
    }
    //auto non_basis_variables = range_if{tableau.variables, std::not_fn(is_basis_variable)};


    // k: pivot row
    // l: pivot column
    // Find pivot column
    size_t l = -1u;
    big_number_t delta_z_l = 0;
    for(size_t j = 0; j < n; ++j)
    {
        if(is_basis_variable(tableau.variables[i])) continue;

        auto delta_z_j = tableau.inner.at(j, m);
        if(delta_z_j < 0 && delta_z_j < delta_z_l)
        {
            l = j;
            delta_z_l = delta_z_j;
        }
    }

}
