set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(hpla OBJECT
  point.hpp
  signal.hpp
  hpla.hpp hpla.cpp
  index_block.hpp index_block.cpp
)


add_executable(hpla-util
  hpla-util.cpp
)

target_link_libraries(hpla-util PUBLIC hpla)
