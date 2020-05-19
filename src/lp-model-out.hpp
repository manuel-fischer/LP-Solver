#pragma once
#include <cmath> // -> abs
#include "lp-model-classify.hpp" // -> is_non_negativity_condition

    template<class OStream>
void print(OStream& out, linear_var_t const& var, var_registry_t const& reg)
{
    if(var.coefficient ==  1) { out <<        reg.name(var.variable); return; }
    if(var.coefficient == -1) { out << '-' << reg.name(var.variable); return; }
    out << var.coefficient << reg.name(var.variable);
}

    template<class OStream>
void print(OStream& out, linear_t const& linear, var_registry_t const& reg)
{
    bool first = true;
    for(auto& linvar : linear.terms)
    {
        if(first)
        {
            print(out, linvar, reg);
            first = false;
        }
        else
        {
            out << (linvar.coefficient < 0 ? " - " : " + ");
            print(out, linear_var_t{std::abs(linvar.coefficient), linvar.variable}, reg);
        }
    }
}

    template<class OStream>
OStream& operator<<(OStream& out, comp_t comp)
{
    return out << "<=\0\0>=\0\0="+((int)comp * 4);
}


    template<class OStream>
void print(OStream& out, limit_t const& limit, var_registry_t const& reg, bool first)
{
    out << (first ? "s.d. " : "     ");
    print(out, limit.func, reg);
    out << ' ' << limit.comp << ' ';
    out << limit.limit;
}


    template<class OStream>
OStream& operator<<(OStream& out, objective_t objective)
{
    return out << "min\0max"+((int)objective * 4);
}


    template<class OStream>
void print(OStream& out, objective_function_t const& objective_function,
           var_registry_t const& reg)
{
    out << objective_function.objective << "  ";
    print(out, objective_function.objective_var, reg);
    out << " = ";
    print(out, objective_function.objective_linear, reg);
}


    template<class OStream>
OStream& operator<<(OStream& out, lp_model_t const& model)
{
    out << "# variables:";
    for(auto& var : model.vars.vars2str)
    {
        out << ' ' << std::string_view{var.data(), var.size()};
    }
    out << "\n\n";

    print(out, model.objective_function, model.vars);
    out << "\n\n";

    bool first = true;
    // first print conditions, that are not non-negativity conditions
    for(auto& limit : model.limits)
    {
        if(is_non_negativity_condition(limit)) continue;
        print(out, limit, model.vars, first);
        out << '\n';

        first = false;
    }
    // then print non-negativity conditions compacted
    bool first_var = true;
    for(auto& limit : model.limits)
    {
        if(!is_non_negativity_condition(limit)) continue;

        out << (first_var ? (first ? "s.d. " : "     ") : ", ");
        print(out, limit.func.terms[0], model.vars);

        first_var = false;
    }
    // if there was a variable, with a non-negativity condition, print >= 0
    if(!first_var) out << " >= 0\n";

    return out;
}
