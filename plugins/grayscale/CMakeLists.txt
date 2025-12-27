add_library(grayscale SHARED Grayscale.cpp)
target_compile_definitions(grayscale PRIVATE BUILDING_PLUGIN)
target_link_libraries(grayscale PRIVATE Qt6::Core Qt6::Gui)
set_target_properties(grayscale PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
