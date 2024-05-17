cmake_minimum_required(VERSION 3.10)
project(steganography)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_library(steganography steganography.h bmp.h bmp.cpp bmpCode.cpp bmpDecode.cpp pngCode.cpp pngDecode.cpp)

add_executable(steganography_test test.cpp)
target_link_libraries(steganography_test PRIVATE steganography)
add_test(NAME steganography_test COMMAND steganography_test)

find_package(Doxygen)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_USE_MATHJAX YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_OUTPUT_LANGUAGE Russian)

doxygen_add_docs(
        doc ALL
        ${PROJECT_SOURCE_DIR}/
        COMMENT "Generate HTML documentation"
)

set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html/
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(TARGETS steganography
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
