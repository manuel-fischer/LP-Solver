#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <string_view>
using std::operator""sv;
#include <charconv> // -> from_chars
#include <algorithm>
#include <iterator> // -> back_inserter

#include "lp-model.hpp"
#include "parsing-error.hpp"

// TODO Since C++20: member functions
bool starts_with(std::string_view a, std::string_view sv)
{
    return a.substr(0, sv.size()) == sv;
}

bool starts_with(std::string_view a, char c)
{
    return !a.empty() && a.front() == c;
}

bool try_parse(std::string_view& input, char what)
{
    if(starts_with(input, what))
    {
        input.remove_prefix(1);
        return true;
    }
    return false;
}

bool try_parse(std::string_view& input, std::string_view what)
{
    if(starts_with(input, what))
    {
        input.remove_prefix(what.size());
        return true;
    }
    return false;
}

bool try_parse_letter(std::string_view& input)
{
    if(input.empty()) return false;

    if(std::isalpha(input[0]))
    {
        input.remove_prefix(1);
        return true;
    }

    return false;
}

bool try_parse_letter_or_digit(std::string_view& input)
{
    if(input.empty()) return false;

    if(std::isalnum(input[0]))
    {
        input.remove_prefix(1);
        return true;
    }

    return false;
}

// all these functions take the variable `input` of type std::string_view by reference,
// these are changed to the function, so that the rest of of the input is what couldn't
// be parsed
// most of the functions return std::optional<T>, the return value contains an object
// if a whole object could be parsed completely


void parse_space(std::string_view& input)
{
    input.remove_prefix(std::min(input.find_first_not_of(" \f\n\r\t\v"sv), input.size()));
}


bool parse_eol(std::string_view& input)
{
    parse_space(input);
    if(input.empty())
    {
        return true;
    }
    if(starts_with(input, '#')) // Line Comment
    {
        input.remove_prefix(input.size());
        return true;
    }
    return false;
}


// return -1 if "-" was found, return 1 if "+" was found
std::optional<int> parse_sign(std::string_view& input)
{
    parse_space(input);
    if(try_parse(input, '+')) return { 1};
    if(try_parse(input, '-')) return {-1};
    return std::nullopt;
}

std::optional<number_t> parse_number(std::string_view& input)
{
    auto l_input = input;
    int sign = parse_sign(l_input).value_or(1);
    /*double number = 1; // number_t
    auto [ptr, errc] = std::from_chars(l_input.data(), l_input.data()+l_input.size(), number);
    if(errc == std::errc())
    {
        input.remove_prefix(ptr-input.data());
        return {sign*number};
    }*/
    {
        bool digit_parsed = false;
        double number = 0;
        while(!l_input.empty() && std::isdigit(l_input[0])) {
            number = number*10. + double(l_input[0]-'0');
            digit_parsed = true;
            l_input.remove_prefix(1);
        }

        if(!l_input.empty() && l_input[0] == '.')
        {
            l_input.remove_prefix(1);

            double mul = 0.1;
            while(!l_input.empty() && std::isdigit(l_input[0])) {
                number = number + double(l_input[0]-'0')*mul;
                mul *= 0.1;
                digit_parsed = true;
                l_input.remove_prefix(1);
            }
        }

        if(!digit_parsed) return std::nullopt;

        input = l_input;
        return sign*number;
    }

    //return std::nullopt;
}

std::optional<number_t> parse_const(std::string_view& input)
{
    return parse_number(input);
}

std::optional<var_t> parse_var(std::string_view& input, var_registry_t& var_registry, var_type_t type)
{
    parse_space(input);
    auto start = input;

    if(!try_parse_letter(input)) return std::nullopt;

    while(try_parse_letter_or_digit(input));

    start.remove_suffix(input.size());
    return {var_registry.put(start, type)};
}

std::optional<linear_var_t> parse_linear_var(std::string_view& input, var_registry_t& var_registry, var_type_t type)
{
    number_t num = parse_const(input).value_or((number_t)1);
    auto var = parse_var(input, var_registry, type);
    if(!var.has_value()) return std::nullopt;
    return {{num, *var}};
}

std::optional<linear_t> parse_linear(std::string_view& input, var_registry_t& var_registry, var_type_t type)
{
    linear_t ret;

    {
        int sign = parse_sign(input).value_or(1);
        auto var = parse_linear_var(input, var_registry, type);
        if(!var.has_value()) return std::nullopt;
        ret.terms.push_back({sign*var->coefficient, var->variable});
    }

    while(true)
    {
        auto p_sign = parse_sign(input);
        if(!p_sign.has_value()) break;
        int sign = *p_sign;

        auto var = parse_linear_var(input, var_registry, type);
        if(!var.has_value()) return std::nullopt;
        ret.terms.push_back({sign*var->coefficient, var->variable});
    }
    return {std::move(ret)};
}

std::optional<objective_t> parse_objective(std::string_view& input)
{
    parse_space(input);
    if(try_parse(input, "min"sv)) return {MIN};
    if(try_parse(input, "max"sv)) return {MAX};
    return std::nullopt;
}

std::optional<objective_function_t> parse_objective_function(std::string_view& input, var_registry_t& var_registry)
{
    auto objective = parse_objective(input);
    if(!objective.has_value()) return std::nullopt;

    auto linear_var = parse_linear_var(input, var_registry, OBJECTIVE);
    if(!linear_var.has_value()) return std::nullopt;

    parse_space(input);
    if(!try_parse(input, '=')) return std::nullopt;

    auto linear = parse_linear(input, var_registry, NORMAL);
    if(!linear.has_value()) return std::nullopt;

    if(!parse_eol(input)) return std::nullopt;

    return {{*objective, *linear_var, std::move(*linear)}};
}

std::optional<comp_t> parse_comp(std::string_view& input)
{
    parse_space(input);
    if(try_parse(input, "<="sv)) return {LEQ};
    if(try_parse(input, ">="sv)) return {GEQ};
    if(try_parse(input, '='))    return {EQ};
    return std::nullopt;
}

std::optional<std::vector<limit_t>> parse_limits(std::string_view& input, var_registry_t& var_registry)
{
    std::vector<linear_t> linears;

    parse_space(input);
    try_parse(input, "s.d.");

    auto linear = parse_linear(input, var_registry, NORMAL);
    if(!linear.has_value()) return std::nullopt;
    linears.push_back(std::move(*linear));

    while((parse_space(input), try_parse(input, ',')))
    {
        auto linear = parse_linear(input, var_registry, NORMAL);
        if(!linear.has_value()) return std::nullopt;
        linears.push_back(std::move(*linear));
    }

    auto comp = parse_comp(input);
    if(!comp.has_value()) return std::nullopt;

    auto limit = parse_const(input);
    if(!limit.has_value()) return std::nullopt;

    if(!parse_eol(input)) return std::nullopt;

    std::vector<limit_t> ret(linears.size());
    std::transform(linears.begin(), linears.end(), ret.begin(),
                   [&](linear_t& linear) -> limit_t { return {std::move(linear), *comp, *limit}; });

    return { std::move(ret) };
}


lp_model_t fully_parse_model(std::ifstream input)
{
    lp_model_t ret;
    bool has_objective_function = false;

    int line_number = 0;
    std::string line;
    while(std::getline(input, line))
    {
        line_number++; // first line is 1

        std::string_view input_line = line;
        parse_space(input_line);
        std::string_view input_line_mod = input_line;

        auto get_column = [&]() { return line.size()-input_line_mod.size() + 1; };

        try
        {
            if(parse_eol(input_line)) continue;

            input_line_mod = input_line;
            auto objective_function = parse_objective_function(input_line_mod, ret.vars);
            if(objective_function.has_value())
            {
                if(has_objective_function)
                {
                    throw parsing_error_t("invalid objective function", line_number, get_column());
                }
                else
                {
                    ret.objective_function = std::move(*objective_function);
                    has_objective_function = true;
                    continue;
                }
            }
            else if(input_line_mod != input_line)
            {
                throw parsing_error_t("invalid objective function", line_number, get_column());
            }


            input_line_mod = input_line;
            auto limits = parse_limits(input_line_mod, ret.vars);
            if(limits.has_value())
            {
                ret.limits.reserve(ret.limits.size()+limits->size());
                std::move(limits->begin(), limits->end(), std::back_inserter(ret.limits));
                continue;
            }
            else if(input_line_mod != input_line)
            {
                throw parsing_error_t("Error parsing limits", line_number, get_column());
            }

            throw parsing_error_t("unexpected", line_number, 1);
        }
        catch(incomplete_parsing_error_t& err)
        {
            throw parsing_error_t(std::move(err), line_number, get_column());
        }
    }

    if(!has_objective_function) throw parsing_error_t("There is no objective function", line_number, 1);

    return ret; // RVO
}
