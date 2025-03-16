# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ScreenshotTranslation_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ScreenshotTranslation_autogen.dir\\ParseCache.txt"
  "ScreenshotTranslation_autogen"
  )
endif()
