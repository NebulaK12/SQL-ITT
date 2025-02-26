# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\SQL-ITT_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SQL-ITT_autogen.dir\\ParseCache.txt"
  "SQL-ITT_autogen"
  )
endif()
