cmake_minimum_required(VERSION 3.28...3.30)

# Define the library for our plugin
add_library(obs-ai-analysis-filter MODULE)

# Add an alias for easier referencing if needed elsewhere (optional)
add_library(OBS::ai-analysis-filter ALIAS obs-ai-analysis-filter)

# Specify the source files for our plugin
target_sources(
  obs-ai-analysis-filter
  PRIVATE
    obs-ai-analysis-filter.c
    plugin-main.c
)

# Link against the main OBS library
target_link_libraries(obs-ai-analysis-filter PRIVATE OBS::libobs)

# OBS-specific properties (folder in IDE, prefix for output file)
set_target_properties_obs(obs-ai-analysis-filter PROPERTIES FOLDER "Plugins/AI Analysis" PREFIX "")

# Windows-specific resource file (optional, but good practice for icons/version info)
# if(OS_WINDOWS)
#   configure_file(cmake/windows/obs-module.rc.in obs-ai-analysis-filter.rc @ONLY)
#   target_sources(obs-ai-analysis-filter PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/obs-ai-analysis-filter.rc)
# endif()
# Note: The .rc part is commented out as we don't have the rc.in template yet.
# We can add it later if needed.

# Include directories if our header is in a subdirectory or for external libs
target_include_directories(obs-ai-analysis-filter
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # Allows #include "obs-ai-analysis-filter.h"
)
