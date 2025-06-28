#include "obs-ai-analysis-filter.h"
#include <obs-module.h>
#include <obs-data.h> // Required for obs_data_t
#include <media-io/audio-math.h> // For db_to_mul if needed, or general audio
#include <math.h> // For general math functions

// Logging helper macros
#define LOG_PREFIX "[ai-analysis-filter: '%s'] "
#define do_log(level, format, ...) \
    blog(level, LOG_PREFIX format, obs_source_get_name(filter_data->context), ##__VA_ARGS__)
#define error(format, ...) do_log(LOG_ERROR, format, ##__VA_ARGS__)
#define warn(format, ...) do_log(LOG_WARNING, format, ##__VA_ARGS__)
#define info(format, ...) do_log(LOG_INFO, format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG, format, ##__VA_ARGS__)

// Filter data structure
struct ai_analysis_filter_data {
    obs_source_t *context; // OBS source context
    struct ai_processing_result last_ai_result; // Store the last result

    // Throttling settings
    int process_every_n_frames;
    uint64_t current_frame_count;

    // For now, let's add a placeholder setting like the gain filter
    float example_setting_value;
};

// --- Placeholder AI Processing Function ---
static struct ai_processing_result placeholder_ai_process_frame(const struct obs_source_frame *frame)
{
    struct ai_processing_result result = {0}; // Initialize to zero/false

    if (!frame || !frame->data[0]) {
        result.detected_something = false;
        return result;
    }

    // Simulate some processing based on frame properties
    // Example: detect "something" if the frame is reasonably large
    if (frame->width > 100 && frame->height > 100) {
        result.detected_something = true;
        result.confidence = 0.75f + (float)(frame->timestamp % 25) / 100.0f; // Vary confidence a bit
        result.placeholder_value = (frame->timestamp % 100); // Some dummy value
    } else {
        result.detected_something = false;
        result.confidence = 0.1f;
        result.placeholder_value = -1;
    }

    // Simulate processing time (remove for real implementation)
    // os_sleep_ms(5); // 5ms delay

    return result;
}


// --- Mandatory functions for a filter ---

// Called to get the display name of the filter
static const char *ai_analysis_filter_name(void *unused)
{
    UNUSED_PARAMETER(unused);
    return obs_module_text("AI Analysis Filter"); // Needs localization string
}

// Called when the filter is created
static void *ai_analysis_filter_create(obs_data_t *settings, obs_source_t *filter)
{
    struct ai_analysis_filter_data *filter_data = bzalloc(sizeof(*filter_data));
    filter_data->context = filter;
    filter_data->current_frame_count = 0; // Initialize frame count

    // Load initial settings
    ai_analysis_filter_update(filter_data, settings);

    info("filter created");
    return filter_data;
}

// Called when the filter is destroyed
static void ai_analysis_filter_destroy(void *data)
{
    struct ai_analysis_filter_data *filter_data = data;
    if (filter_data) {
        info("filter destroyed");
        // Add any cleanup code here (e.g., free AI models)
        bfree(filter_data);
    }
}

// Called when filter settings are updated
static void ai_analysis_filter_update(void *data, obs_data_t *settings)
{
    struct ai_analysis_filter_data *filter_data = data;
    info("filter settings updated");

    filter_data->process_every_n_frames = (int)obs_data_get_int(settings, S_PROCESS_NTH_FRAME);
    if (filter_data->process_every_n_frames < 1) {
        filter_data->process_every_n_frames = 1; // Ensure it's at least 1
    }
    debug("Process every Nth frame set to: %d", filter_data->process_every_n_frames);

    // Placeholder: Update example setting
    // double val = obs_data_get_double(settings, "example_slider");
    // filter_data->example_setting_value = (float)val;
    // debug("Example setting updated to: %f", filter_data->example_setting_value);

    // UNUSED_PARAMETER(settings); // settings is now used
}

// --- Optional functions, but commonly used ---

// Called to set default settings values
#define S_PROCESS_NTH_FRAME "process_nth_frame"
#define TEXT_PROCESS_NTH_FRAME obs_module_text("Process Every Nth Frame") // Needs localization

static void ai_analysis_filter_defaults(obs_data_t *settings)
{
    info("setting filter defaults");
    obs_data_set_default_int(settings, S_PROCESS_NTH_FRAME, 1); // Default to process every frame
    // Example: obs_data_set_default_double(settings, "example_slider", 0.0);
    // UNUSED_PARAMETER(settings); // Remove if settings are used
}

// Called to create the properties view for the filter in OBS UI
obs_properties_t *ai_analysis_filter_properties(void *data)
{
    obs_properties_t *props = obs_properties_create();
    info("creating filter properties");

    obs_properties_add_int(props,
                           S_PROCESS_NTH_FRAME,
                           TEXT_PROCESS_NTH_FRAME,
                           1,    // Min value
                           600,  // Max value (e.g., process once every 10 seconds at 60fps)
                           1);   // Step value

    // Example: Add a float slider
    // obs_property_t *p = obs_properties_add_float_slider(props,
    //                                     "example_slider",
    //                                     obs_module_text("Example Slider"), // Needs localization
    //                                     0.0, 100.0, 0.1);
    // obs_property_float_set_suffix(p, " units");

    UNUSED_PARAMETER(data);
    return props;
}

// --- Filter specific functions ---

// Called for each video frame (if it's a video filter)
static struct obs_source_frame *ai_analysis_filter_video(void *data, struct obs_source_frame *frame)
{
    struct ai_analysis_filter_data *filter_data = data;
    // This is where frame capture and AI processing will happen.
    // For now, just pass the frame through.

    if (frame) {
        // debug("Processing video frame: ts=%llu, width=%u, height=%u, format=%d, linesize[0]=%u, data[0]=%p",
        //       frame->timestamp,
        //       frame->width,
        //       frame->height,
        //       frame->format,
        //       frame->linesize[0],
        //       frame->data[0]);

        filter_data->current_frame_count++;

        bool should_process_this_frame = (filter_data->current_frame_count % filter_data->process_every_n_frames == 0);

        if (should_process_this_frame) {
            // Call the placeholder AI processing function
            filter_data->last_ai_result = placeholder_ai_process_frame(frame);

            if (filter_data->last_ai_result.detected_something) {
                debug("AI Result (Frame %llu): Detected! Confidence: %.2f, Value: %d (ts: %llu)",
                      filter_data->current_frame_count,
                      filter_data->last_ai_result.confidence,
                      filter_data->last_ai_result.placeholder_value,
                      frame->timestamp);

                // Simulate overlay: Log what would be drawn
                info("OVERLAY_SIM: [Detected Object | Conf: %.2f | Val: %d]",
                     filter_data->last_ai_result.confidence,
                     filter_data->last_ai_result.placeholder_value);

            } else {
                // debug("AI Result (Frame %llu): Nothing detected. (ts: %llu)", filter_data->current_frame_count, frame->timestamp);
                // Simulate overlay: Log that nothing is drawn or clear overlay
                // info("OVERLAY_SIM: [Clear/No Detection]"); // Optional: only log when something *is* detected.
            }
        } else {
            // Not processing this frame due to throttling.
            // If we want the overlay to reflect the *last processed detection*, we do nothing here.
            // If we want the overlay to clear if the current frame isn't processed, we'd do:
            // info("OVERLAY_SIM: [Throttled - No Update]");
            // For now, overlay will show the last *processed* detection.
            // If last_ai_result.detected_something is true from a previous processed frame, that "overlay" would persist.
            if (filter_data->last_ai_result.detected_something) {
                 info("OVERLAY_SIM_PERSIST: [Detected Object | Conf: %.2f | Val: %d] (from last processed frame)",
                     filter_data->last_ai_result.confidence,
                     filter_data->last_ai_result.placeholder_value);
            }
        }

    } else {
        debug("Received null video frame.");
        filter_data->last_ai_result.detected_something = false; // Reset if frame is null
        filter_data->current_frame_count = 0; // Reset frame count on null/error
    }

    // UNUSED_PARAMETER(filter_data); // filter_data is now used
    return frame;
}

// Define the filter structure
struct obs_source_info ai_analysis_filter_info = {
    .id = "ai_analysis_filter",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_ASYNC, // Asynchronous video filter
    .get_name = ai_analysis_filter_name,
    .create = ai_analysis_filter_create,
    .destroy = ai_analysis_filter_destroy,
    .update = ai_analysis_filter_update,
    .get_defaults = ai_analysis_filter_defaults,
    .get_properties = ai_analysis_filter_properties,
    .filter_video = ai_analysis_filter_video,
    // Add .filter_audio if it also processes audio
};
#ifdef __cplusplus
}
#endif
