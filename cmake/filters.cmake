include_guard(GLOBAL)

function(create_filter filter folder)
    source_group("Header Files\\${filter}" REGULAR_EXPRESSION "include/${folder}/.*\.(hpp|h)")
    source_group("Source Files\\${filter}" REGULAR_EXPRESSION "src/${folder}/.*\.(cpp|cc|m|mm)")
endfunction()

function(create_filter_lib filter folder lib)
    source_group("Header Files\\${filter}" REGULAR_EXPRESSION "include/${lib}/${folder}/.*\.(hpp|h)")
    source_group("Source Files\\${filter}" REGULAR_EXPRESSION "src/${folder}/.*\.(cpp|cc|m|mm)")
endfunction()

# Put this after you included the file that defines create_filter*()

function(auto_create_filters)
  cmake_parse_arguments(A " " "LIB" "" ${ARGN})
  set(INC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")

  set(REL_INC_DIRS)
  if(EXISTS "${INC_ROOT}")
    file(GLOB_RECURSE INC_DIRS LIST_DIRECTORIES true
         CONFIGURE_DEPENDS
         "${INC_ROOT}/*")
    foreach(p IN LISTS INC_DIRS)
      if(IS_DIRECTORY "${p}")
        file(RELATIVE_PATH rel "${INC_ROOT}" "${p}")
        if(NOT rel STREQUAL "" AND NOT rel MATCHES "^\\.")
          list(APPEND REL_INC_DIRS "${rel}")
        endif()
      endif()
    endforeach()
  endif()

  if(EXISTS "${SRC_ROOT}")
    file(GLOB_RECURSE SRC_DIRS LIST_DIRECTORIES true
         CONFIGURE_DEPENDS
         "${SRC_ROOT}/*")
    foreach(p IN LISTS SRC_DIRS)
      if(IS_DIRECTORY "${p}")
        file(RELATIVE_PATH rel "${SRC_ROOT}" "${p}")
        if(NOT rel STREQUAL "" AND NOT rel MATCHES "^\\.")
          list(APPEND REL_INC_DIRS "${rel}")
        endif()
      endif()
    endforeach()
  endif()

  list(REMOVE_DUPLICATES REL_INC_DIRS)

  foreach(folder IN LISTS REL_INC_DIRS)
    string(REPLACE "/" "\\" filter_name "${folder}")

    if(A_LIB)
      create_filter_lib("${filter_name}" "${folder}" "${A_LIB}")
    else()
      create_filter("${filter_name}" "${folder}")
    endif()
  endforeach()
endfunction()
