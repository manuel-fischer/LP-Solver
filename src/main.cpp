#include <iostream>

#include "lp-model.hpp"
#include "lp-model-parser.hpp"
#include "lp-model-out.hpp"
#include "lp-model-classify.hpp"
#include "lp-model-transform.hpp"

#include "lp-tableau.hpp"
#include "lp-tableau-create.hpp"
#include "lp-tableau-out.hpp"
#include "lp-tableau-transform.hpp"

#include <cassert>

int main(int argc, char** argv)
{
    size_t cell_width = 10;

    if(argc != 2) { std::cerr << "lp-solve <input>\n"; return 1; }

    lp_model_t model = fully_parse_model(std::ifstream(argv[1]));


    model = standard_form(std::move(model));
    std::cout << "Standard form:\n";
    std::cout << model << "\n\n";
    //assert(is_standard_form(model));
    //assert(!is_canonical_form(model)); // should be false, because of missing slack variables

    model = canonical_form(std::move(model));
    std::cout << "Canonical form:\n";
    std::cout << model << "\n\n";
    //assert(is_canonical_form(model));


    tableau_t tableau = tableau_create(model);

    simplex_solve_primal(/*mut*/tableau,
        [&, i=1](tableau_t const& tableau) mutable
        {
            std::cout << "\n\n" << i << ". Tableau\n";
            std::cout << std::setw(cell_width) << tableau;
            i++;
        }
    );

    //std::cout << z;

    return 0;
}
