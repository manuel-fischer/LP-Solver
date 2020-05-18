#pragma once

#include "lp-model.hpp"
#include "lp-model-classify.hpp"

void make_negative(linear_var_t& linear_var)
{
    linear_var.coefficient *= -1;
}

void make_negative(linear_t& linear)
{
    for(auto& t : linear.terms)
    {
        make_negative(t);
    }
}

lp_model_t canonical_form(lp_model_t model)
{
    // Enshure maximization function
    if(model.objective_function.objective == MIN)
    {
        make_negative(model.objective_function.objective_var);
        model.objective_function.objective = MAX;
        make_negative(model.objective_function.objective_linear);
    }

    // TODO: handle GEQ and EQ limits

    return std::move(model);
}
