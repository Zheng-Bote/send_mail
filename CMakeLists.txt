cmake_minimum_required(VERSION 3.24)

# cmake -S . -B build
# cmake --build build -j$(nproc)

# Projektname und Sprache
project(SendMail
    VERSION 0.1.1
    DESCRIPTION "SendMail based on Input"
    HOMEPAGE_URL "https://github.com/Zheng-Bote/send-mail"
    LANGUAGES C CXX)

# C++23 Standard erzwingen
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt MOC for Q_OBJECT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 finden
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
# OpenSSL finden
find_package(OpenSSL REQUIRED)

# --- Configure (für rz_config.hpp) ---
set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(PROG_LONGNAME "Send eMails based on input")
set(PROG_CREATED "2026")
set(PROG_AUTHOR "ZHENG Bote")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")

if(Qt6_FOUND)
    set(CMAKE_QTV "${Qt6_VERSION}")
else()
    set(CMAKE_QTV "Unknown")
endif()

# Generiert include/rz_config.hpp im Binary-Tree
add_subdirectory(configure)

include(FetchContent)

FetchContent_Declare(
    dotenv-cpp
    GIT_REPOSITORY https://github.com/laserpants/dotenv-cpp.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(dotenv-cpp)


FetchContent_Declare(
    SimpleMail
    GIT_REPOSITORY https://github.com/cutelyst/simple-mail.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(SimpleMail)

# Quelldateien definieren (inklusive MVC Struktur)
set(SOURCES
    src/main.cpp
    src/ConfigModel.cpp
    include/ConfigModel.hpp
    src/SmtpService.cpp
    include/SmtpService.hpp
    src/MailController.cpp
    include/MailController.hpp
)

# Executable erstellen
add_executable(${PROJECT_NAME} ${SOURCES})

# Include Directories
# WICHTIG: CMAKE_BINARY_DIR hinzufügen, damit rz_config.hpp gefunden wird
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${ASIO_INCLUDE_DIR}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# Bibliotheken linken
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    SimpleMail::Core
    dotenv
)

# Install targets for AppImage support
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# SendMail.env is not installed (user provided)

# Desktop file and Icon
install(FILES SendMail.desktop DESTINATION share/applications)
install(FILES resources/sendmail.svg DESTINATION share/icons/hicolor/scalable/apps)

# Optimierungen
if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()
