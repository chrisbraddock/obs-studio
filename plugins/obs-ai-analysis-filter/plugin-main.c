#include <obs-module.h>
#include "obs-ai-analysis-filter.h" // To make ai_analysis_filter_info visible

// Define module information (optional but good practice)
OBS_MODULE_AUTHOR("AI Developer & Jules");
OBS_MODULE_NAME("AI Analysis Filter");
OBS_MODULE_DESCRIPTION("Performs AI analysis on video frames.");
// OBS_MODULE_VERSION // Can be set if versioning is desired

// Called when the module is loaded
bool obs_module_load(void)
{
    // Register the filter defined in obs-ai-analysis-filter.c
    obs_register_source(&ai_analysis_filter_info);
    blog(LOG_INFO, "AI Analysis Filter module loaded successfully.");
    return true;
}

// Called when the module is unloaded (optional)
void obs_module_unload(void)
{
    blog(LOG_INFO, "AI Analysis Filter module unloading.");
}

// Called to set the locale (for translations) (optional)
// void obs_module_set_locale(const char *locale)
// {
//     blog(LOG_INFO, "AI Analysis Filter module locale set to: %s", locale);
// }

// Called to free the locale data (optional)
// void obs_module_free_locale(void)
// {
//     blog(LOG_INFO, "AI Analysis Filter module locale freed.");
// }
