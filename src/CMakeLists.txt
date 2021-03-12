aux_source_directory(./app APP)

aux_source_directory(./fft FFT)

aux_source_directory(./utils UTILS)

add_library(${PROJECT_NAME} SHARED ${APP} ${FFT} ${UTILS})

