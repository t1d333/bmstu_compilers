# MIT License 
# Copyright (c) 2018-Today Michele Adduci <adduci@tutanota.com>
#
# Project-related instructions

# Activate Testing, if given
if(BUILD_TESTING)
  enable_testing()
endif()

#####################################
# Define Targets
#####################################
add_subdirectory(analyzer)
add_subdirectory(grammar)
add_subdirectory(lexer)
add_subdirectory(tree)
add_subdirectory(ast)
#####################################
# Define Install Targets
#####################################
# install(TARGETS
#   lexer
#   analyzer
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib
#   PUBLIC_HEADER DESTINATION include
# )
#
# install(
#   FILES 
#     ${CMAKE_SOURCE_DIR}/project/hellolib/include/hellolib.h 
#     ${CMAKE_BINARY_DIR}/exports/hellolib_export.h
#   DESTINATION 
#     include/hellolib)
#
# install(FILES  
#   ${CMAKE_SOURCE_DIR}/LICENSE
# DESTINATION .)
