

#include <iostream>
#include <sstream>
#include <fstream>


#include <layered_grid.h>
#include <config.h>
#include <support.h>


#include <boost/sort/sort.hpp>


LayeredGrid::LayeredGrid(const RectSteinerTree& rst)
    : terminal_points(rst.get_terminal_points())
{
    auto rst_points = rst.get_rst_points();

    for (const auto& point : rst_points)
        layered_points[point] = LayeredGridPoint(point);

    for (const auto& point : terminal_points)
        layered_points[point].enable_pins_connection();

    for (const auto& [begin_point, end_point] : rst)
    {
        if (is_same_horizontal(begin_point, end_point)) {
            const auto [source, target] = get_edges(begin_point.get_x(), end_point.get_x());
            add_segment(horizontal_segments_edge_array[begin_point.get_y()], 
                        source,
                        target);
            
            layered_points[begin_point].enable_horizontal_connection();
            layered_points[end_point].enable_horizontal_connection();
        }
        else if (is_same_vertical(begin_point, end_point)) {
            const auto [source, target] = get_edges(begin_point.get_y(), end_point.get_y());
            add_segment(vertical_segments_edge_array[begin_point.get_x()],
                        source,
                        target);

            layered_points[begin_point].enable_vertical_connection();
            layered_points[end_point].enable_vertical_connection();
        }
        else
            add_segments_diagonal(begin_point, end_point);
    }

    create_segments(horizontal_segments_edge_array, create_horizontal_segment_points);
    create_segments(vertical_segments_edge_array, create_vertical_segment_points);
}


void LayeredGrid::add_segment(LayeredGrid::segment_edges_set_t& segment_edges_set, 
                              coord_type source,
                              coord_type target)
{
    auto& [source_set, target_set] = segment_edges_set;
    const auto [can_prepend, can_append] = can_extend_segment(segment_edges_set, source, target);

    if (!can_prepend && !can_append)
    {
        source_set.emplace(source);
        target_set.emplace(target);
    }
    else
    {
        if (can_prepend)
        {
            source_set.erase(target);

            if (!can_append)
                source_set.emplace(source);
        }

        if (can_append)
        {
            target_set.erase(source);

            if (!can_prepend)
                target_set.emplace(target);
        }
    }
}


void LayeredGrid::add_segments_diagonal(const GridPoint& begin_point,
                                        const GridPoint& end_point)
{
    const auto [source_x, target_x] = get_edges(begin_point.get_x(), end_point.get_x());
    const auto [source_y, target_y] = get_edges(begin_point.get_y(), end_point.get_y());

    // horizontal-vertical pair
    const auto h_v_horizontal = std::make_tuple(begin_point.get_y(), source_x, target_x);
    const auto h_v_vertical   = std::make_tuple(end_point.get_x(), source_y, target_y);
    const auto h_v_corner_point = GridPoint(end_point.get_x(), begin_point.get_y());

    // vertical-horizontal pair
    const auto v_h_vertical   = std::make_tuple(begin_point.get_x(), source_y, target_y);
    const auto v_h_horizontal = std::make_tuple(end_point.get_y(), source_x, target_x);
    const auto v_h_corner_point = GridPoint(begin_point.get_x(), end_point.get_y());

    const auto h_v_extend_score = get_extend_score(h_v_horizontal, h_v_vertical);
    const auto v_h_extend_score = get_extend_score(v_h_horizontal, v_h_vertical);

    if (h_v_extend_score >= v_h_extend_score) {
        add_segment_horizontal(h_v_horizontal);
        add_segment_vertical(h_v_vertical);

        layered_points[begin_point].enable_horizontal_connection();
        layered_points[end_point].enable_vertical_connection();

        add_corner_point(h_v_corner_point);
    }
    else {
        add_segment_horizontal(v_h_horizontal);
        add_segment_vertical(v_h_vertical);

        layered_points[begin_point].enable_vertical_connection();
        layered_points[end_point].enable_horizontal_connection();

        add_corner_point(v_h_corner_point);
    }
}


LayeredGrid::extend_score_type LayeredGrid::get_extend_score(const LayeredGrid::segment_coord_t& horizontal_segment,
                                                             const LayeredGrid::segment_coord_t& vertical_segment)
{
    const auto [horizontal_y, source_x, target_x] = horizontal_segment;
    const auto [horizontal_can_prepend, horizontal_can_append] = can_extend_segment(horizontal_segments_edge_array[horizontal_y],
                                                                                    source_x,
                                                                                    target_x);
    
    const auto [vertical_x, source_y, target_y] = vertical_segment; 
    const auto [vertical_can_prepend, vertical_can_append] = can_extend_segment(vertical_segments_edge_array[vertical_x],
                                                                                source_y,
                                                                                target_y);
    
    return static_cast<extend_score_type>(horizontal_can_prepend + horizontal_can_append + 
                                          vertical_can_prepend + vertical_can_append);
}


void LayeredGrid::create_segments(const std::unordered_map<coord_type, LayeredGrid::segment_edges_set_t>& segments_edge_array,
                                  LayeredGrid::create_segment_points_func_t points_creation_func)
{
    for (const auto& [common_coord, edges_sets] : segments_edge_array)
    {
        const auto& [source_edges_set, target_edges_set] = edges_sets;

        std::vector<coord_type> source_edges_array(source_edges_set.begin(), source_edges_set.end());
        std::vector<coord_type> target_edges_array(target_edges_set.begin(), target_edges_set.end());

        boost::sort::block_indirect_sort(source_edges_array.begin(), 
                                         source_edges_array.end(),
                                         config::ParallelConfig::get()->get_num_threads());

        boost::sort::block_indirect_sort(target_edges_array.begin(), 
                                         target_edges_array.end(),
                                         config::ParallelConfig::get()->get_num_threads());
                  ;

        for (auto source_it = source_edges_array.begin(), target_it = target_edges_array.begin();
             source_it != source_edges_array.end();
             ++source_it, ++target_it)
        {
            segments.push_back(points_creation_func(common_coord, *source_it, *target_it));
        }
    }
}


void LayeredGrid::output_xml_file(const std::string& output_file) const
{
    std::ostringstream oss;

    oss << config::OUTPUT_XML_BEGINNING;
    
    for (const auto& terminal_point : terminal_points)
        oss << output_point(terminal_point.get_x(), terminal_point.get_y(), PointLayer::PINS);
    
    for (const auto& layered_point_pair : layered_points)
        oss << output_layered_point(layered_point_pair.second);

    for (const auto& segment : segments)
        oss << output_segment(segment);

    oss << config::OUTPUT_XML_ENDING;

    write_output_file(output_file, oss.str());
}


std::string LayeredGrid::output_segment(const LayeredGrid::segment_t& segment) const
{
    std::ostringstream oss;

    const auto& [source, target, layer] = segment;
    oss << "<segment x1=\"" << source.get_x() << "\" y1=\"" << source.get_y() << "\"" 
                   " x2=\"" << target.get_x() << "\" y2=\"" << target.get_y() << "\"" 
                " layer=\"" << get_layer_string(layer) << "\" />\n";
    
    return oss.str();
}


std::string LayeredGrid::output_point(coord_type x, coord_type y, PointLayer layer) const
{
    std::ostringstream oss;
    oss << "<point x=\"" << x << "\" y=\"" << y << "\" layer=\"" << get_layer_string(layer) << "\" />\n";
    return oss.str();
}


std::string LayeredGrid::output_layered_point(const LayeredGridPoint& point) const
{
    std::ostringstream oss;
    
    auto layers = point.get_layers();
    for (const auto& layer : layers)
        oss << output_point(point.get_x(), point.get_y(), layer);
    
    return oss.str();
}


void LayeredGrid::write_output_file(const std::string& output_file, const std::string& text) const
{
    std::ofstream out(output_file);

    if (out)
    {
        out << text;
        out.close();
    }
    else
        support::raise_error(support::ErrorType::BAD_OPEN_OUTPUT_FILE);
}