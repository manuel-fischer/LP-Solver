#pragma once
#include "lp-model.hpp"
#include <algorithm> // -> is_partitioned, any_of

bool is_non_negativity_condition(limit_t const& limit)
{
    if(limit.func.terms.size() != 1) return false;
    if(limit.comp != GEQ) return false;
    if(limit.limit != 0) return false;
    return true;
}


bool is_standard_canonical_form_common(lp_model_t const& model, comp_t comp)
{
    if(model.objective_function.objective != MAX) return false;

    // check if all conditions, that aren't non-negativity conditions, are leq-conditions
    std::vector<bool> non_negative(model.vars.size());
    non_negative[model.objective_function.objective_var.variable] = true;
    for(auto& limit : model.limits)
    {
        if(limit.comp == comp) continue;
        if(is_non_negativity_condition(limit))
        {
            non_negative[limit.func.terms[0].variable] = true;
            continue;
        }
        return false;
    }

    // check if all non-negativity conditions are at the end -> assumptions in code
    if(!std::is_partitioned(model.limits.begin(), model.limits.end(),
                            is_non_negativity_condition))
        return false;

    // check if no slack variables are in the objective function
    /*for(auto& var : model.objective_function.objective_linear.terms)
    {
        auto type = model.vars.type(var.variable);
        if(type != NORMAL && type != HELP) return false;
    }*/

    // check if all variables are non-negative
    //if(!std::all_of(non_negative.begin(), non_negative.end())) return false;
    //if(find(non_negative.begin(), non_negative.end(), false) != non_negative.end()) return false;
    if(std::any_of(non_negative.begin(), non_negative.end(), std::logical_not<bool>{})) return false;

    return true;
}


bool is_standard_form(lp_model_t const& model)
{
    //check if every condition is an LEQ condition, that is not a non-negativity condition
    if(!is_standard_canonical_form_common(model, LEQ)) return false;

    // TODO check if there are NO slack variables

    return true;
}


bool is_canonical_form(lp_model_t const& model)
{
    //check if every condition is an EQ condition, that is not a non-negativity condition
    if(!is_standard_canonical_form_common(model, EQ)) return false;

    // TODO: check if there are slack variables

    return true;
}
