#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "parsing-error.hpp"
#include "iota-iterator.hpp"

using var_t = size_t;

enum var_type_t
{
    OBJECTIVE,
    NORMAL,
    SLACK,
    HELP,
};

struct var_registry_t {
    // use vector: avoid small string optimization to avoid invalidating strviews
    // in str2vars

    std::vector<std::vector<char>> vars2str;
    std::vector<var_type_t> vars2type;
    std::unordered_map<std::string_view, var_t> str2vars;

    var_t put(std::string_view str, var_type_t type)
    {
        if(auto fnd = str2vars.find(str); fnd != str2vars.end())
        {
            if(vars2type[fnd->second] != type) throw incomplete_parsing_error_t() << "Variable used as a different kind of variable";
            return fnd->second;
        }
        else
        {
            var_t var = vars2str.size();
            vars2str.emplace_back(str.begin(), str.end());
            vars2type.push_back(type);
            str2vars.emplace(
                std::string_view(vars2str.back().data(), vars2str.back().size()), var
            );
            return var;
        }
    }

    std::string_view name(var_t var) const
    {
        return {vars2str[var].data(), vars2str[var].size()};
    }

    var_type_t type(var_t var) const
    {
        return vars2type[var];
    }

    size_t size() const
    {
        return vars2str.size();
    }

    iota_iterator<var_t> begin() const { return {0};      }
    iota_iterator<var_t> end()   const { return {size()}; }

};
