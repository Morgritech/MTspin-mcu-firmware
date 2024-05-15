add_custom_target(sloc ALL)
#add_custom_target(sloc)

add_custom_command(
    TARGET sloc
    COMMAND ${CMAKE_COMMAND} -E echo "SLOC This Repo:"
    COMMAND cloc ${CMAKE_SOURCE_DIR}/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Source lines of code (SLOC)"
    VERBATIM
)
