#include <iostream>

#include "lp-model-parser.hpp"
#include "lp-model-out.hpp"
#include "lp-model-classify.hpp"
#include "lp-model-transform.hpp"

int main(int argc, char** argv)
{
    if(argc != 2) { std::cerr << "lp-solve <input>\n"; return 1; }

    lp_model_t model = fully_parse_model(std::ifstream(argv[1]));


    std::cout << model;
    std::cout << is_canonical_form(model) << '\n';

    model = canonical_form(std::move(model));

    std::cout << model;

    return 0;
}
