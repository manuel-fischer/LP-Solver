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
    // n: number of columns = number of normal & help variables
    // m: number of rows    = number of normal, help & slack variables/limits
    size_t n = count_tableau_variables(tableau.model->vars);
    size_t m = count_variables_of_type(tableau.model->vars, SLACK);

    auto w = std::max<size_t>(3, out.width());
    auto sw = std::setw(w);
    out.width(0);

    auto& vars = tableau.model->vars;


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
        for(size_t j = 0; j < n; ++j)
        {
            out << ' ' << sw<<number_out{tableau.inner._(i,j)};
        }
        out << " | " << sw<<number_out{tableau.inner._(i,n)};
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
    for(size_t j = 0; j < n; ++j)
    {
        out << ' ' << sw<<number_out{tableau.inner._(m,j)};
    }
    out << " | " << sw<<number_out{tableau.inner._(m,n)};
    out << '\n';

    return out;
}
