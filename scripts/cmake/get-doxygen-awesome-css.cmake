set(DOXYGEN_AWESOME_CSS_VERSION 2.3.2)

# Directory to download the doxygen-awesome-css zip archive.
set(DOXYGEN_AWESOME_CSS_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/_deps)
# Directory to use for renaming the extracted doxygen-awesome-css folder.
set(DOXYGEN_AWESOME_CSS_FOLDER_DIR ${DOXYGEN_AWESOME_CSS_DOWNLOAD_DIR}/doxygen-awesome-css)
# Directory to use for the downloaded doxygen-awesome-css zip archive.
set(DOXYGEN_AWESOME_CSS_ARCHIVE_DIR ${DOXYGEN_AWESOME_CSS_FOLDER_DIR}.zip)
# Directory of the extracted doxygen-awesome-css folder.
set(DOXYGEN_AWESOME_CSS_EXTRACTED_DIR ${DOXYGEN_AWESOME_CSS_FOLDER_DIR}-${DOXYGEN_AWESOME_CSS_VERSION})

file(
  DOWNLOAD 
  https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v${DOXYGEN_AWESOME_CSS_VERSION}.zip
  ${DOXYGEN_AWESOME_CSS_ARCHIVE_DIR}
)

file(
  ARCHIVE_EXTRACT INPUT ${DOXYGEN_AWESOME_CSS_ARCHIVE_DIR}
  DESTINATION ${DOXYGEN_AWESOME_CSS_DOWNLOAD_DIR}
)

file(RENAME ${DOXYGEN_AWESOME_CSS_EXTRACTED_DIR} ${DOXYGEN_AWESOME_CSS_FOLDER_DIR})