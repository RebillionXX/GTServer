#pragma once
#include <include/randutils.h>
#include <include/pcg/pcg_random.h>

namespace randutils {    
    using pcg_rng = random_generator<pcg32>;
}

namespace utils {
    namespace random {
        inline randutils::pcg_rng get_generator_static() {
            static randutils::pcg_rng pcg_rng{};
            return pcg_rng;
        }

        inline randutils::pcg_rng get_generator_thread_local() {
            thread_local randutils::pcg_rng pcg_rng{};
            return pcg_rng;
        }

        inline randutils::pcg_rng get_generator_local() {
            randutils::pcg_rng pcg_rng{};
            return pcg_rng;
        }
    }
}