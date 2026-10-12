add_executable(App_Dynamic
    main.cpp
)

# Визначаємо ANOMALY_IMPORTS для явного dllimport під час підключення DLL
target_compile_definitions(App_Dynamic PRIVATE ANOMALY_IMPORTS)

target_link_libraries(App_Dynamic PRIVATE
    AnomalyDetection_Shared
)

target_compile_features(App_Dynamic PUBLIC cxx_std_20)

# Автоматично копіюємо AnomalyDetection.dll у папку з бінарником після збірки
add_custom_command(TARGET App_Dynamic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:AnomalyDetection_Shared>"
        "$<TARGET_FILE_DIR:App_Dynamic>"
)
