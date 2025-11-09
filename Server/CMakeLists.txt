cmake_minimum_required(VERSION 3.16)
project(Server LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Server
main.cpp
BaseOld.cpp
BotServer.cpp
FeedServer.cpp
FoodServer.cpp
GameEngine.cpp
HeroServer.cpp
ObjectsServer.cpp
PieceServer.cpp
ThornServer.cpp
MessageServer.cpp
 "MoveObjectServer.cpp" "Colors.h" "UniqueIdGenerator.h" "UniqueIdGenerator.cpp"  "MapSegment.h" "Camera.h" "Camera.cpp" "Map.h" "Map.cpp" "QuadTree.cpp" "Generator.h" "ClippingAlgorithm.cpp"    "Rtree.h" "Rtree.cpp" "Constants.h" "PiecesList.h" "PiecesList.cpp" "IRegistrator.h" "Respawn.cpp"  "JsonSerialize.h" "JsonSerialize.cpp")
target_link_libraries(Server PRIVATE BaseLib)
target_compile_features(Server PRIVATE cxx_std_17)
target_include_directories(Server  PUBLIC ..\\jsonLib\\include)

install(TARGETS Server)

if(WIN32)
    add_custom_command(
        TARGET Server
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Server>
        VERBATIM)
endif()