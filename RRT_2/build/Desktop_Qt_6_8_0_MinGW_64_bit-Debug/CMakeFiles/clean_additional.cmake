# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RRT_2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RRT_2_autogen.dir\\ParseCache.txt"
  "RRT_2_autogen"
  )
endif()
