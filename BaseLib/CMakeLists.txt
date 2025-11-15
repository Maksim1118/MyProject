set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
   GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_library(BaseLib STATIC src/Map_Geometry.cpp "src/BaseFunctions.cpp")

target_include_directories(BaseLib PUBLIC include ..\\jsonLib\\include)
target_link_libraries(BaseLib sfml-graphics sfml-system sfml-window sfml-network sfml-graphics sfml-audio sfml-main)