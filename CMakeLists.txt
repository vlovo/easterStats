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



 

add_executable(${ProjectName}  ${SRCS} )



 

