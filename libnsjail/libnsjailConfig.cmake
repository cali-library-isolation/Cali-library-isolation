get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(LIBNSJAIL_INCLUDE_DIRS ${SELF_DIR}/include/)
set(LIBNSJAIL_LIBRARY ${SELF_DIR}/libnsjail.a)