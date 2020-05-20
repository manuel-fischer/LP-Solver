#pragma once

#include "lp-tableau.hpp"

// is valid
bool is_primally_valid(tableau_t const& tableau)
{
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = tableau.variables.size();
    size_t m = tableau.basis_variables.size();

    for(size_t i = 0; i < m; ++i)
    {
        if(tableau.b_(i) < 0.) return false;
    }
    return true;
}

// is optimal
bool is_dually_valid(tableau_t const& tableau)
{
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = tableau.variables.size();
    size_t m = tableau.basis_variables.size();

    for(size_t j = 0; j < n; ++j)
    {
        if(tableau.dz_(j) < 0.) return false;
    }
    return true;

}
