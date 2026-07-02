cmake_minimum_required(VERSION 3.8)
project(robo_1_master)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

#cv_bridge image_transport OpenCV
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)


find_package(geometry_msgs REQUIRED)



install(
  DIRECTORY launch config rviz src
  DESTINATION share/${PROJECT_NAME}/
)

add_executable(main_brain src/main_brain.cpp)
ament_target_dependencies(main_brain rclcpp std_msgs)
add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher rclcpp std_msgs)

add_executable(check_distance src/check_distance.cpp)
ament_target_dependencies(check_distance rclcpp sensor_msgs)

add_executable(minimal_param_node src/minimal_parameter.cpp)
ament_target_dependencies(minimal_param_node rclcpp)

add_executable(camera src/camera.cpp)
ament_target_dependencies(camera cv_bridge rclcpp std_msgs sensor_msgs image_transport OpenCV)

add_executable(controller src/control.cpp)
ament_target_dependencies(controller rclcpp std_msgs geometry_msgs)


install(TARGETS
  main_brain
  publisher
  check_distance
  minimal_param_node
  camera
  controller
  DESTINATION lib/${PROJECT_NAME})
ament_package()
