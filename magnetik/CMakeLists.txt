cmake_minimum_required(VERSION 3.8)
project(CylindricalDetector)

# Настройка пути к ROOT из snap
set(ROOT_DIR "/snap/root-framework/current/usr/local")
list(APPEND CMAKE_PREFIX_PATH ${ROOT_DIR})
list(APPEND CMAKE_MODULE_PATH 
    ${ROOT_DIR}/cmake/modules
    ${ROOT_DIR}/share/root/cmake
)

# Настройка переменных окружения для ROOT
set(ENV{ROOTSYS} ${ROOT_DIR})
set(ENV{LD_LIBRARY_PATH} "${ROOT_DIR}/lib:$ENV{LD_LIBRARY_PATH}")

# Поиск ROOT с необходимыми компонентами
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree Hist Physics)
if(ROOT_FOUND)
    include(${ROOT_USE_FILE})
    include_directories(${ROOT_INCLUDE_DIRS})
    message(STATUS "Found ROOT version: ${ROOT_VERSION}")
else()
    message(FATAL_ERROR "ROOT not found!")
endif()

# Настройка Geant4
set(Geant4_DIR "/home/anna2003/software/geant4/geant4-v11.2.2-install/lib/cmake/Geant4")
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Добавление директорий с заголовочными файлами
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Исходные файлы проекта
set(SOURCES
    src/DetectorConstruction.cc
    src/PrimaryGeneratorAction.cc
    src/RunAction.cc
    src/EventAction.cc
    src/MySensitiveDetector.cc
    src/main.cc
)

# Создание исполняемого файла
add_executable(cylinder_detector ${SOURCES})

# Подключение библиотек
target_link_libraries(cylinder_detector 
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
)

# Копирование макро-файлов
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vis.mac
    ${CMAKE_CURRENT_BINARY_DIR}/vis.mac
    COPYONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/run.mac
    ${CMAKE_CURRENT_BINARY_DIR}/run.mac
    COPYONLY
)

# Вывод информации о сборке
message(STATUS "ROOT path: ${ROOT_DIR}")
message(STATUS "ROOT libraries: ${ROOT_LIBRARIES}")
message(STATUS "ROOT include dirs: ${ROOT_INCLUDE_DIRS}")
message(STATUS "Geant4 version: ${Geant4_VERSION}")
message(STATUS "Сборка с поддержкой чувствительного детектора")

# Собираем исходные файлы с добавлением MySensitiveDetector
set(SOURCES
    src/DetectorConstruction.cc
    src/PrimaryGeneratorAction.cc
    src/RunAction.cc
    src/EventAction.cc
    src/MySensitiveDetector.cc
    src/main.cc
)

