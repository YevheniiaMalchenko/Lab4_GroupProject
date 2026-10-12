add_executable(App_Static
    main.cpp
)

target_link_libraries(App_Static PRIVATE
    AnomalyDetection_Static
)

target_compile_features(App_Static PUBLIC cxx_std_20)
