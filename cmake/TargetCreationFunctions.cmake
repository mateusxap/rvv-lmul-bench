function(create_project_lib TARGET )
    file(GLOB TARGET_SRC "*.c*")
    file(GLOB TARGET_HD "*.h*")
    add_library(${TARGET} STATIC ${TARGET_SRC} ${TARGET_HD})
    
    target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")
  
    get_property ( LIB_LIST GLOBAL PROPERTY LIBS_P)
    list(APPEND LIB_LIST ${TARGET})
    set_property ( GLOBAL PROPERTY LIBS_P ${LIB_LIST})
endfunction()

function(create_executable_project TARGET M_CONSTANT)
    file(GLOB TARGET_SRC "*.c*")
    file(GLOB TARGET_HD "*.h*")
    add_executable(${TARGET} ${TARGET_SRC} ${TARGET_HD})
	if(${M_CONSTANT})
		target_compile_options(${TARGET} PUBLIC -DRVV_MLEN=${M_CONSTANT})
		if(HAVE_RVV_0P7)
		    target_compile_options(${TARGET} PUBLIC -DRVVMF_RVV_0P7)
		endif()
		if(HAVE_RVV_1P0)
			target_compile_options(${TARGET} PUBLIC -DRVVMF_RVV_1P0)
		endif()
	endif()
    target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")
    
    get_property ( LIB_LIST GLOBAL PROPERTY LIBS_P)
    target_link_libraries(${TARGET} ${LIB_LIST})
endfunction()
