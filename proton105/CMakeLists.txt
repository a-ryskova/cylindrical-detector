cmake_minimum_required(VERSION 3.8)
project(KaonTracker)

# Указываем точный путь к установке Geant4
set(Geant4_DIR "/home/anna2003/software/geant4/geant4-v11.2.2-install/lib/cmake/Geant4")

# Находим пакет Geant4
find_package(Geant4 REQUIRED ui_all vis_all)

# Включаем флаги компиляции Geant4
include(${Geant4_USE_FILE})

# Добавляем директории include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Копирование mac-файлов в build директорию
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/init_vis.mac
  ${CMAKE_CURRENT_BINARY_DIR}/init_vis.mac
  COPYONLY
)

# Add copying of vis.mac
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

# Собираем исходные файлы
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Создаем исполняемый файл
add_executable(kaonTracker ${sources})

# Линковка библиотек Geant4
target_link_libraries(kaonTracker ${Geant4_LIBRARIES})

# Вывод информации о конфигурации сборки
message(STATUS "Сборка проекта KaonTracker")
message(STATUS "Используемые компоненты Geant4: ${Geant4_COMPONENTS}")
