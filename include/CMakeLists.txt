INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/contrib/mongoose"
                   DESTINATION include/hyperose
                   FILES_MATCHING
                   PATTERN "*.h"
                   PATTERN "*.hpp")
