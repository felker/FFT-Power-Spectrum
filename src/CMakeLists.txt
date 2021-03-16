aux_source_directory(./app APP)

aux_source_directory(./fft FFT)

aux_source_directory(./utils UTILS)

add_library(${PROJECT_NAME} SHARED ${APP} ${FFT} ${UTILS})
target_link_libraries(${PROJECT_NAME} PUBLIC "/usr/local/lib/libsfml-audio.dylib;/usr/local/lib/libsfml-graphics.dylib;/usr/local/lib/libsfml-window.dylib;/usr/local/lib/libsfml-system.dylib")
# target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
