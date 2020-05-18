#pragma once

#include <vector>

#include "var-registry.hpp"

// TODO change number_t to high precision double
using number_t = double;

#ifdef IMPL_BIGM
struct big_number_t
{
    number_t real, big;
};
#else
using big_number_t = number_t;
#endif


struct linear_var_t
{
    big_number_t coefficient;
    var_t variable;
};

struct linear_t
{
    std::vector<linear_var_t> terms;
};

enum comp_t
{
    LEQ,
    GEQ,
    EQ
};

struct limit_t
{
    linear_t func;
    comp_t comp;
    big_number_t limit;
};

enum objective_t
{
    MIN,
    MAX
};

struct objective_function_t
{
    objective_t objective;
    linear_var_t objective_var;
    linear_t objective_linear;
};

struct lp_model_t
{
    var_registry_t vars;
    objective_function_t objective_function;
    std::vector<limit_t> limits;
};
