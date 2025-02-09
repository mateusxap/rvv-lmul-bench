message ("    check HAVE_RVV_0P7")
try_compile(HAVE_RVV_0P7
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/cmake/checks/compile_rvv_0p7.cpp
            )

message ("    check HAVE_RVV_1P0")
try_compile(HAVE_RVV_1P0
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/cmake/checks/compile_rvv_1p0.cpp
            )
