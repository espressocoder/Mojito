cmake_minimum_required(VERSION 3.8)
PROJECT(Mojito)

# --------------- Includes ---------------------
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/contrib")

# ------------------ MVS Libraries --------------------
IF(ENABLE_SHARED_LIBS)
    SET(mongoose_LIBRARY mongoose_shared)
ELSE()
    SET(mongoose_LIBRARY mongoose_static)
ENDIF()

# --------------- Outputs ---------------------
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
FILE(MAKE_DIRECTORY "${LIBRARY_OUTPUT_PATH}")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(contrib)
