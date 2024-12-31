# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\P1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\P1_autogen.dir\\ParseCache.txt"
  "P1_autogen"
  )
endif()
