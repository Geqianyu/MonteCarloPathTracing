
#include "Math.h"

namespace Monte_carlo_path_tracing
{
    //---------------------------------------------- 常量 -----------------------------------------------
    extern const double infinity = std::numeric_limits<double>::infinity();
    extern const double sigma = 1e-8;
    extern const double Russian_roulette_probability = 0.6;
    extern const double pi = std::asin(1.0) * 2.0;
    //---------------------------------------------------------------------------------------------------
}