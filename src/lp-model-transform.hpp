#pragma once

#include "lp-model.hpp"
#include "lp-model-classify.hpp"
#include <algorithm> // -> std::partition

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

lp_model_t standard_form(lp_model_t model)
{
    // Enshure maximization function
    if(model.objective_function.objective == MIN)
    {
        make_negative(model.objective_function.objective_var);
        model.objective_function.objective = MAX;
        make_negative(model.objective_function.objective_linear);
    }

    return std::move(model);
}

// canonical form == normal form
lp_model_t canonical_form(lp_model_t model)
{
    // TODO: check standard_form



    // Add slack variables
    std::vector<var_t> slack_variables;
    char buffer[sizeof(size_t)*3 + 2]; // *3 could be *log10(256) ~= 2.40824, 2 for 's', '\0'
    for(auto& limit : model.limits)
    {
        if(is_non_negativity_condition(limit)) continue;

        // TODO: handle GEQ and EQ limits
        std::sprintf(buffer, "s%zu", slack_variables.size()+1);
        var_t slack_var = model.vars.put(std::string_view(buffer), SLACK);
        slack_variables.push_back(slack_var);
        limit.func.terms.push_back({1, slack_var});
        limit.comp = EQ;
    }

    std::stable_partition(model.limits.begin(), model.limits.end(), is_non_negativity_condition);

    // Add non-negativity condition for slack variables
    model.limits.reserve(model.limits.size() + slack_variables.size());
    for(auto& var : slack_variables)
    {
        limit_t limit;
        limit.func = {{{1, var}}};
        limit.comp = GEQ;
        limit.limit = 0;
        model.limits.push_back(std::move(limit));
    }


    return std::move(model);
}
