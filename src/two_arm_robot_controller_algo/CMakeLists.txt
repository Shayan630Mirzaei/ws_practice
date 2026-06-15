cmake_minimum_required(VERSION 3.8)
project(two_arm_robot_controller_algo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(pluginlib REQUIRED)
find_package(controller_interface REQUIRED)
find_package(robot_interfaces REQUIRED)

include_directories(include)

add_library(
  ${PROJECT_NAME}
  SHARED
  src/two_arm_controller.cpp
)

ament_target_dependencies(${PROJECT_NAME} rclcpp rclcpp_lifecycle controller_interface pluginlib robot_interfaces)

pluginlib_export_plugin_description_file(controller_interface first_order_low_pass_filter_plugin.xml)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)

install(
  DIRECTORY include
  DESTINATION include
)

ament_export_libraries(
  ${PROJECT_NAME}
)

ament_export_dependencies(
  controller_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
)


ament_package()
