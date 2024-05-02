set(PLANTUML_VERSION 1.2024.4)

file(
  DOWNLOAD 
  https://github.com/plantuml/plantuml/releases/download/v${PLANTUML_VERSION}/plantuml.jar
  ${CMAKE_BINARY_DIR}/_deps/plantuml.jar
)