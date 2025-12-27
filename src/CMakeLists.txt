add_executable(timeline_editor
  main.cpp
  Bridge.cpp
  PluginManager.cpp
  VideoPlayer.cpp
  Timeline.cpp
  IFeature.h
)

target_link_libraries(timeline_editor PRIVATE
  Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets Qt6::WebChannel Qt6::Multimedia Qt6::Gui
)

# copy UI folder to runtime dir
add_custom_command(TARGET timeline_editor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/ui $<TARGET_FILE_DIR:timeline_editor>/ui
  COMMENT "Copying ui/ to output folder"
)
