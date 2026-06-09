cmake_minimum_required(VERSION 3.15.0)

project(Visual_Odometry)
add_executable(${PROJECT_NAME} src/main.cpp src/vo.cpp src/helper.cpp)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_NAME} PRIVATE 
                        include
                        )

target_link_libraries(${PROJECT_NAME} PRIVATE
                        ${OpenCV_LIBS}
                        )