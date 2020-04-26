

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>


#include <layered_grid.h>
#include <rect_steiner_tree_factory.h>
#include <support.h>
#include <config.h>

#include <omp.h>


std::string get_input_file(int argc, char** argv);
std::string get_output_file(const std::string& input_file);



int main(int argc, char** argv)
{
    const std::size_t max_threads = omp_get_max_threads();
    config::ParallelConfig::get()->set_num_threads(max_threads);

    const auto input_file = get_input_file(argc, argv);
    
    auto factory = std::make_unique<RectSteinerTreeFactory>();
    auto steiner_tree = std::unique_ptr<RectSteinerTree>(factory->construct_from_file(input_file));

    auto layered_grid = std::make_unique<LayeredGrid>(*steiner_tree);

    const auto output_file = get_output_file(input_file);
    layered_grid->output_xml_file(output_file);

    return EXIT_SUCCESS;
}




std::string get_input_file(int argc, char** argv)
{
    constexpr static const int ARGUMENT_NUMBER = 1;
    support::check_argument_number(argc, ARGUMENT_NUMBER);

    return std::string(argv[ARGUMENT_NUMBER]);
}


std::string get_output_file(const std::string& input_file)
{
    constexpr static const std::size_t PREFIX_SIZE = 4; // ".xml"
    return input_file.substr(0, input_file.size() - PREFIX_SIZE) + "_out.xml";
}