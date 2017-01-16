cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CustomCmake/")

 


SET( ProjectName 
 easterStats
)

project(${ProjectName})


SET(SRCS 
	easterStats.cpp
     )
	
SET(HDRS
	 	)
	
 
add_definitions(-DNOMINMAX)


option(USE_NO_OPTIMIZATION_AT_RELWITHDEBINFO
"Shall Code be optimized for RELWITHDEBINFO?"
  ON
  )
if(USE_NO_OPTIMIZATION_AT_RELWITHDEBINFO)
add_definitions(-DUSE_NO_OPTIMIZATION_AT_RELWITHDEBINFO)
endif(USE_NO_OPTIMIZATION_AT_RELWITHDEBINFO)

option(USE_BOOST
"boost ?"
 OFF
  )

if(USE_BOOST)
find_package(BOOST)
include_directories (${Boost_INCLUDE_DIR}) 
link_directories(${Boost_LIBRARY_DIRS})
endif(USE_BOOST)

option(USE_OPENCV
"opencv ?"
 OFF
  )

if(USE_OPENCV)
find_package(openCV)
link_directories(${OpenCV_LIB_DIR_OPT})
# opencv_core2410.lib opencv_highgui2410.lib opencv_imgproc2410.lib
endif(USE_OPENCV)




option(USE_PROTOBUF
"protobuf ?"
off)

if(USE_PROTOBUF)

set(PROTOBUF_SRC_ROOT_FOLDER "x:/protobuf")
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS _.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

 
endif(USE_PROTOBUF)


 
#Build shared library (dll) 
add_executable(${ProjectName}  ${SRCS} )
#TARGET_LINK_LIBRARIES(${NFMsurfControlName} )


 

