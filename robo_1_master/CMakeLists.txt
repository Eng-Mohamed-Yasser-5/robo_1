cmake_minimum_required(VERSION 3.8)
project(robo_1_master)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)



install(
  DIRECTORY launch config rviz src
  DESTINATION share/${PROJECT_NAME}/
)

add_executable(main_brain src/main_brain.cpp)
ament_target_dependencies(main_brain rclcpp std_msgs)
add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher rclcpp std_msgs)


install(TARGETS
  main_brain
  publisher
  DESTINATION lib/${PROJECT_NAME})
ament_package()
