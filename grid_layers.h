

#ifndef LAYERS_H
#define LAYERS_H

#include <unordered_map>
#include <string>


enum class PointLayer {
    #define DEFINE_POINT_LAYER(LAYER, STR_LAYER) \
        LAYER,
    
    #include "point_layers.def"

        MAX_POINT_LAYER

    #undef DEFINE_POINT_LAYER
};


enum class SegmentLayer {
    #define DEFINE_SEGMENT_LAYER(LAYER, STR_LAYER) \
        LAYER,
    
    #include "segment_layers.def"

        MAX_SEGMENT_LAYER

    #undef DEFINE_SEGMENT_LAYER
};


extern const std::unordered_map<PointLayer, std::string> point_layer_strings;
extern const std::unordered_map<SegmentLayer, std::string> segment_layer_strings;

static inline const std::string& get_layer_string(PointLayer layer) { return point_layer_strings.at(layer); }
static inline const std::string& get_layer_string(SegmentLayer layer) { return segment_layer_strings.at(layer); }

#endif // LAYERS_H