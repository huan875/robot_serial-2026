cmake_minimum_required(VERSION 3.8)
project(robot_serial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

# 串口管理库
add_library(serial_port_manager
  src/serial_port_manager.cpp
)

ament_target_dependencies(serial_port_manager
  rclcpp
  std_msgs
)

# 可执行文件
add_executable(serial_node_exe
  src/serial_node.cpp
  src/main.cpp
)

target_link_libraries(serial_node_exe
  serial_port_manager
)

ament_target_dependencies(serial_node_exe
  rclcpp
  std_msgs
)

install(TARGETS
  serial_node_exe
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  include/
  DESTINATION include
)

install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
ament_package()
