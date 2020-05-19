#pragma once

#include "lp-tableau.hpp"

#include <iostream>
#include <iomanip>


struct number_out
{
    number_t value;
};

    template<class OStream>
OStream& operator<<(OStream& out, number_out num)
{
    if(num.value == -0.)
        num.value = 0.;

    return out << num.value;
}

struct blank
{
    size_t count;
};

    template<class OStream>
OStream& operator<<(OStream& out, blank b)
{
    auto fill = out.fill();
    for(size_t i = 0; i < b.count; ++i)
        out << fill;

    return out;
}

struct fill
{
    size_t count;
    char fill_char;
};

    template<class OStream>
OStream& operator<<(OStream& out, fill b)
{
    for(size_t i = 0; i < b.count; ++i)
        out << b.fill_char;

    return out;
}




    template<class OStream>
OStream& operator<<(OStream& out, tableau_t const& tableau)
{
    // v: n+m
    // n: number of normal & help variables,
    // m: number of slack variables/limits
    size_t v = count_tableau_variables(tableau.model.vars);
    size_t m = count_variables_of_type(tableau.model.vars, SLACK);
    size_t n = v-m;

    auto w = std::max<size_t>(3, out.width());
    auto sw = std::setw(w);
    out.width(0);

    auto& vars = tableau.model.vars;


    // top row
    out << blank{3+w} << " |";
    for(var_t var : tableau.variables)
    {
        out << ' ' << sw<<vars.name(var);
    }
    out << " | " << sw<<"RS"<<'\n';



    // draw horizontal line
    out << fill{3+w, '-'} << "-+";
    out << fill{tableau.variables.size()*(1+w), '-'};
    out << "-+-" << fill{w, '-'} << '\n';



    // inner rows
    bool first = true;
    for(size_t i = 0; i < m; ++i)
    {
        out << (first ? "BV " : "   ");
        out << sw<<vars.name(tableau.basis_variables[i]);
        out << " |";
        for(size_t j = 0; j < n+m; ++j)
        {
            out << ' ' << sw<<number_out{tableau.inner.at(j, i)};
        }
        out << " | " << sw<<number_out{tableau.inner.at(n+m, i)};
        out << '\n';

        first = false;
    }



    // draw horizontal line
    out << fill{3+w, '-'} << "-+";
    out << fill{tableau.variables.size()*(1+w), '-'};
    out << "-+-" << fill{w, '-'} << '\n';



    // delta-z row
    out << "   ";
    out << sw<<"/\\z";
    out << " |";
    for(size_t j = 0; j < n+m; ++j)
    {
        out << ' ' << sw<<number_out{tableau.inner.at(j, m)};
    }
    out << " | " << sw<<number_out{tableau.inner.at(n+m, m)};
    out << '\n';

    return out;
}
