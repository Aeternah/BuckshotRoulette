# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BuckshotRoulette_autogen"
  "CMakeFiles\\BuckshotRoulette_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BuckshotRoulette_autogen.dir\\ParseCache.txt"
  )
endif()
