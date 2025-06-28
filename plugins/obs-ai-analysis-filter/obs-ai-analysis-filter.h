#pragma once

#include <obs-module.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration for filter data structure
struct ai_analysis_filter_data;

// Dummy structure for AI processing results
struct ai_processing_result {
    bool detected_something;
    float confidence;
    // For a real model, this would contain bounding boxes, classifications, etc.
    // For placeholder, simple fields are enough.
    int placeholder_value;
};

// Function to register the filter properties (will be defined in .c file)
extern obs_properties_t *ai_analysis_filter_properties(void *data);

// Definition of the filter itself (will be defined in .c file)
extern struct obs_source_info ai_analysis_filter_info;

#ifdef __cplusplus
}
#endif
