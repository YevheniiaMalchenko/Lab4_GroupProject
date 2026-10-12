# Створення статичної бібліотеки (STATIC)
add_library(AnomalyDetection_Static STATIC
    src/AnomalyDetection.cpp
)

target_include_directories(AnomalyDetection_Static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(AnomalyDetection_Static PUBLIC cxx_std_20)

# Створення динамічної бібліотеки (SHARED)
add_library(AnomalyDetection_Shared SHARED
    src/AnomalyDetection.cpp
)

# Встановлюємо вихідне ім'я AnomalyDetection (щоб отрималось AnomalyDetection.dll та AnomalyDetection.lib)
set_target_properties(AnomalyDetection_Shared PROPERTIES
    OUTPUT_NAME "AnomalyDetection"
)

# Визначаємо ANOMALY_EXPORTS для експорту функцій при збірці DLL
target_compile_definitions(AnomalyDetection_Shared PRIVATE ANOMALY_EXPORTS)

target_include_directories(AnomalyDetection_Shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(AnomalyDetection_Shared PUBLIC cxx_std_20)

# Створення SDK-пакета (Stage 2 Delivery SDK Package)
set(SDK_DIR "${CMAKE_SOURCE_DIR}/AnomalyDetection_SDK")

add_custom_command(TARGET AnomalyDetection_Shared POST_BUILD
    COMMENT "Building AnomalyDetection SDK Package..."
    # Створення директорій
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/lib"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/bin"
    # Копіювання заголовочного файлу
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/AnomalyDetection.h"
        "${SDK_DIR}/include/AnomalyDetection.h"
    # Копіювання імпортної бібліотеки .lib
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_LINKER_FILE:AnomalyDetection_Shared>"
        "${SDK_DIR}/lib/"
    # Копіювання .dll бібліотеки
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:AnomalyDetection_Shared>"
        "${SDK_DIR}/bin/"
    # Копіювання README.md (Контракт)
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        "${SDK_DIR}/README.md"
)
