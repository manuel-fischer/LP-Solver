#pragma once

#include "lp-tableau-classify.hpp"

#include "lp-tableau.hpp"
#include "range-if.hpp"


// return k,l (row,column)
std::pair<size_t, size_t> select_pivot(tableau_t const& tableau)
{
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = tableau.variables.size();
    size_t m = tableau.basis_variables.size();


    auto is_basis_variable = [&](var_t var) {
        return std::find(tableau.basis_variables.begin(), tableau.basis_variables.end(), var) != tableau.basis_variables.end();
    };
    //auto non_basis_variables = range_if{tableau.variables, std::not_fn(is_basis_variable)};


    // k: pivot row
    // l: pivot column
    // Acceptance criterium
    // Find pivot column
    size_t l = -1u;
    big_number_t delta_z_l = 0;
    for(size_t j = 0; j < n; ++j)
    {
        if(is_basis_variable(tableau.variables[j])) continue;

        auto delta_z_j = tableau.dz_(j);
        if(/*delta_z_j < 0 &&*/ delta_z_j < delta_z_l)
        {
            l = j;
            delta_z_l = delta_z_j;
        }
    }
    //if(l == -1u) return {-1u, -1u};
    assert(l != -1u); // Precondition, !is_dually_valid

    // Elimination criterum
    // Find pivot row
    size_t k = -1u;
    big_number_t theta_k = 0;
    for(size_t i = 0; i < m; ++i)
    {
        if(tableau.a_(i,l) > 0)
        {
            big_number_t theta_i = tableau.b_(i)/tableau.a_(i,l);
            if(k == -1u || theta_i < theta_k)
            {
                k = i;
                theta_k = theta_i;
            }
        }
    }

    return {k,l};
}




// k, l: row, column of pivot element
tableau_t basis_swap(tableau_t& tableau, size_t k, size_t l)
{
    tableau_t tableau_new = tableau; // Copy

    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = tableau.variables.size();
    size_t m = tableau.basis_variables.size();

    big_number_t pivot_element = tableau.a_(k,l);

    // including delta-z row, b column(rs) column and result value
    for(size_t i = 0; i < m+1; ++i)
    {
        for(size_t j = 0; j < n+1; ++j)
        {
            // pivotcolumn
            if(j==l)
            {
                tableau_new.inner._(i,l) = i==k ? 1 : 0; // also sets delta-z_i to 0
            }
            // pivotrow
            else if(i==k)
            {
                tableau_new.inner._(k,j) = tableau.inner._(k,j) / pivot_element; // also calculates b-column
            }
            // circle rule
            else
            {
                // also calculates delta-z row, b-column and objective-result
                tableau_new.inner._(i,j) = tableau.inner._(i,j) - (tableau.inner._(k,j)*tableau.inner._(i,l)) / pivot_element;
            }
        }
    }

    tableau_new.basis_variables[k] = tableau.variables[l];

    return tableau_new; // RVO
}




    template<class CB>
void simplex_solve_primal(tableau_t& tableau, CB yield_step)
{
    while(!is_dually_valid(tableau))
    {
        auto[k,l] = select_pivot(tableau);
        std::cout << k << ", " << l << "\n";
        if(k == -1u || l == -1u) break /* unbounded convex polyhedron */;

        yield_step(tableau);

        tableau = basis_swap(tableau, k, l);

    }
    yield_step(tableau);
}
