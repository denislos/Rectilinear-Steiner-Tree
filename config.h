

#ifndef CONFIG_H
#define CONFIG_H

#include <types.h>

#include <omp.h>

namespace config {

    constexpr static const coord_type MIN_X = 0;
    constexpr static const coord_type MIN_Y = 0;

    constexpr static const coord_type MAX_X = 150;
    constexpr static const coord_type MAX_Y = 150;

    constexpr static const std::size_t MAX_TERMINALS = 100;

    static const std::string OUTPUT_XML_BEGINNING = "<root>\n<grid min_x=\"0\" max_x=\"150\" min_y=\"0\" max_y=\"150\" />\n<net>\n";
    static const std::string OUTPUT_XML_ENDING = "</net>\n</root>";

    class ParallelConfig 
    {
    public:
        static ParallelConfig* get()
        {
            static ParallelConfig config;
            return &config;
        }

        inline void set_num_threads(std::size_t n_threads) noexcept
        {
            omp_set_num_threads(n_threads);
            num_threads = n_threads;
        }

        inline auto get_num_threads() const noexcept { return num_threads; }
    
    private:
        ParallelConfig() { }

        std::size_t num_threads = 1;
    };

} // namespace config

#endif // CONFIG_H