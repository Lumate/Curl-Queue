cmake_minimum_required (VERSION 2.8)
project(curlqueue)

include(FindProtobuf)

add_definitions("-Wno-deprecated-declarations")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#enable c++ 11
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${CMAKE_CXX_FLAGS}")

#zeromqness
find_package(ZMQ REQUIRED)
include_directories(${ZMQ_INCLUDE_DIRS})

#boooooooost
find_package(Boost 1.55 COMPONENTS thread system regex REQUIRED)

#Protocol buffers
find_package(Protobuf REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Glog REQUIRED)
include_directories(GLOG_INCLUDE_DIRS)

include_directories(${CMAKE_SOURCE_DIR})

set(pbuf_sources
    url_request.pb.cc
)

set(router_sources
    server.cpp
   )
set(worker_sources
    responder.cpp
    )

add_executable(router ${router_sources})
add_executable(worker ${worker_sources} ${pbuf_sources})

target_link_libraries(router ${ZMQ_LIBRARIES} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY} ${GLOG_LIBRARIES})
target_link_libraries(worker ${ZMQ_LIBRARIES} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY} ${GLOG_LIBRARIES})

add_subdirectory(tests)
