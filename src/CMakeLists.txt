project(cam_model)

#############################
# OpenCV ####################
#############################
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

###### libMesaSR ###### 
link_directories("../mesalib")

set(srcs cam_model.cpp cam_cov.cpp)

add_library(cam_model ${srcs})
target_link_libraries(cam_model ${catkin_LIBRARIES} ${OpenCV_LIBS}  mesasr)

add_library(sr4k_io SR_reader.cpp SR_reader_cv.cpp)
target_link_libraries(sr4k_io mesasr)

add_library(rs2h_io rs_r200_wrapper.cpp)
