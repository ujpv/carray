   add_library(carray STATIC CArrayUtil.c CArrayUtil.h)
   target_include_directories(carray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})