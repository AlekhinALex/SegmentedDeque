# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Models_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Models_autogen.dir/ParseCache.txt"
  "CMakeFiles/SegmentedDeque_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SegmentedDeque_autogen.dir/ParseCache.txt"
  "Models_autogen"
  "SegmentedDeque_autogen"
  )
endif()
