

#include <fstream>

#include <rect_steiner_tree_factory.h>
#include <support.h>
#include <types.h>
#include <config.h>
#include <grid_point.h>
#include <hanan_grid.h>

#include <rapidxml.hpp>


char* read_input_file(const std::string& input_file);

RectSteinerTree* RectSteinerTreeFactory::construct_from_file(const std::string& input_file)
{
    char* input_buffer = read_input_file(input_file);

    constexpr static const int RAPIDXML_PARSE_FLAGS = 0;
    rapidxml::xml_document<> document;
    document.parse<RAPIDXML_PARSE_FLAGS>(input_buffer);

    auto root_node = document.first_node("root");
    if (!root_node)
        support::raise_error(support::ErrorType::BAD_INPUT_FILE_FORMAT);

    auto net_node = root_node->first_node("net");
    if (!net_node)
        support::raise_error(support::ErrorType::BAD_INPUT_FILE_FORMAT);
    
    auto point = net_node->first_node("point");
    if (!point)
        support::raise_error(support::ErrorType::BAD_INPUT_FILE_FORMAT);


    PointSet points;

    coord_type min_x = config::MAX_X, min_y = config::MAX_Y;
    coord_type max_x = config::MIN_X, max_y = config::MIN_Y;


    for (; point; point = point->next_sibling())
    {
        auto x_attribute = point->first_attribute("x");
        auto y_attribute = point->first_attribute("y");

        if (!x_attribute || !y_attribute)
            support::raise_error(support::ErrorType::BAD_INPUT_FILE_FORMAT);
        
        const coord_type x = std::atoi(x_attribute->value());
        const coord_type y = std::atoi(y_attribute->value());

        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);

        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);

        points.push_back(GridPoint(x, y));
    }

    PointSet hanan_grid_points = construct_hanan_grid_points(points, 
                                                             min_x, max_x,
                                                             min_y, max_y);

    auto steiner_tree = new RectSteinerTree(points, hanan_grid_points);

    return steiner_tree;
}



char* read_input_file(const std::string& input_file)
{
    std::ifstream in(input_file);

    char* read_buffer = nullptr;

    if (in)
    {
        in.seekg(0, std::ios::end);
        const std::size_t size = in.tellg();
        in.seekg(0, std::ios::beg);
        
        read_buffer = new(std::nothrow)char[size + 1];
        if (read_buffer == nullptr)
            support::raise_error(support::ErrorType::BAD_ALLOC);
        
        in.read(read_buffer, size);

        in.close();
    }
    else
        support::raise_error(support::ErrorType::BAD_OPEN_INPUT_FILE);
    
    return read_buffer;
}