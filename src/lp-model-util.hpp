#pragma once

#include "lp-model.hpp"
#include <algorithm>

auto is_tableau_variable(var_registry_t const& vars)
{
    return [&vars](var_t var) { return vars.vars2type[var] != OBJECTIVE; };
}

auto is_variable_of_type(var_registry_t const& vars, var_type_t type)
{
    return [&vars, type](var_t var) { return vars.vars2type[var] == type; };
}

size_t count_tableau_variables(var_registry_t const& vars)
{
    return vars.size() - 1; // assuming one objective variable, to be substracted
}

size_t count_variables_of_type(var_registry_t const& vars, var_type_t type)
{
    return std::count(vars.vars2type.begin(), vars.vars2type.end(), type);
}


big_number_t get_coefficient(linear_t const& linear, var_t var)
{
    big_number_t coefficient = 0;
    for(auto& v : linear.terms)
    {
        if(v.variable == var)
            coefficient += v.coefficient;
    }
    return coefficient;
}
