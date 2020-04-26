

#ifndef LAYERED_GRID_H
#define LAYERED_GRID_H

#include <utility>
#include <unordered_set>
#include <unordered_map>


#include <grid_point.h>
#include <rect_steiner_tree.h>
#include <grid_layers.h>



class LayeredGridPoint : public GridPoint
{
public:
    using GridPoint::GridPoint;

    LayeredGridPoint(const GridPoint& point)
        : GridPoint(point)
    { }

    void enable_pins_connection() { is_pin = true; }
    
    void enable_vertical_connection() {
        if (has_vertical_connection())
            return;

        if (has_pins_connection())
        {
            if (has_horizontal_connection())
                layers.insert(PointLayer::M2_M3);
            else
                layers.insert({PointLayer::PINS_M2, PointLayer::M2, PointLayer::M2_M3});
        }
        else
            layers.insert(PointLayer::M2_M3);
        
        is_vertical_connection_enabled = true;
    }

    void enable_horizontal_connection() {
        if (has_horizontal_connection())
            return;

        if (has_pins_connection())
        {
            if (has_vertical_connection())
                layers.erase(PointLayer::M2);
            else
                layers.insert(PointLayer::PINS_M2);

            is_horizontal_connection_enabled = true;
        }
    }

    inline bool has_pins_connection() const { return is_pin; }
    inline bool has_horizontal_connection() const { return is_horizontal_connection_enabled; }
    inline bool has_vertical_connection() const { return is_vertical_connection_enabled; }

    const auto& get_layers() const { return layers; }

private:
    std::unordered_set<PointLayer> layers;

    PointSet terminal_points;

    bool is_pin = false;
    bool is_horizontal_connection_enabled = false;
    bool is_vertical_connection_enabled = false;
};



class LayeredGrid
{
public:
    LayeredGrid(const RectSteinerTree& rst);

    void output_xml_file(const std::string& output_file) const;

private:
    using segment_edges_set_t = std::pair<std::unordered_set<coord_type>,
                                          std::unordered_set<coord_type>>;
    
    using segment_t = std::tuple<GridPoint, GridPoint, SegmentLayer>;

    using segment_coord_t = std::tuple<coord_type, coord_type, coord_type>;

    std::unordered_map<coord_type, segment_edges_set_t> horizontal_segments_edge_array, vertical_segments_edge_array;
    std::unordered_map<GridPoint, LayeredGridPoint, GridPointHasher> layered_points;

    PointSet terminal_points;

    std::vector<segment_t> segments;

    using create_segment_points_func_t = segment_t (*)(coord_type common, coord_type source, coord_type target);

    static segment_t create_horizontal_segment_points(coord_type common, coord_type source, coord_type target) {
        return std::make_tuple(GridPoint(source, common), GridPoint(target, common), SegmentLayer::M2);
    }

    static segment_t create_vertical_segment_points(coord_type common, coord_type source, coord_type target) {
        return std::make_tuple(GridPoint(common, source), GridPoint(common, target), SegmentLayer::M3);
    }

    void create_segments(const std::unordered_map<coord_type, segment_edges_set_t>& segments_edge_array,
                         create_segment_points_func_t points_creation_func);

    auto get_edges(coord_type begin_point, coord_type end_point) const {
        return std::make_pair(std::min(begin_point, end_point),
                              std::max(begin_point, end_point));
    }

    auto can_extend_segment(segment_edges_set_t& segment_edges_set, coord_type source, coord_type target) const {
        const auto& [source_set, target_set] = segment_edges_set;

        return std::make_pair(source_set.find(target) != source_set.end(),
                              target_set.find(source) != target_set.end());
    }

    using extend_score_type = unsigned int;

    extend_score_type get_extend_score(const segment_coord_t& horizontal_segment,
                                       const segment_coord_t& vertical_segment);

    void add_segment(segment_edges_set_t& segment_edges_set, coord_type source, coord_type target);
    void add_segments_diagonal(const GridPoint& begin_point, const GridPoint& end_point);

    void add_segment_horizontal(const segment_coord_t& segment) {
        const auto [horizontal_y, source_x, target_x] = segment;
        add_segment(horizontal_segments_edge_array[horizontal_y], source_x, target_x);
    }

    void add_segment_vertical(const segment_coord_t& segment) {
        const auto [vertical_x, source_y, target_y] = segment;
        add_segment(vertical_segments_edge_array[vertical_x], source_y, target_y);
    }

    void add_corner_point(const GridPoint& point) {
        auto corner_point = LayeredGridPoint(point);
        corner_point.enable_vertical_connection();
        
        layered_points[point] = corner_point;
    }

    std::string output_segment(const segment_t& segment) const;

    std::string output_point(coord_type x, coord_type y, PointLayer layer) const;
    std::string output_layered_point(const LayeredGridPoint& point) const;

    void write_output_file(const std::string& output_file, const std::string& text) const;
};


#endif // LAYERED_GRID_H