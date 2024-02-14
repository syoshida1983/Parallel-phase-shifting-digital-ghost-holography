#pragma once

#ifdef SPI_CLIENT_EXPORTS
#define SPI_CLIENT_API __declspec(dllexport)
#else
#define SPI_CLIENT_API __declspec(dllimport)
#endif

extern "C" SPI_CLIENT_API int32_t open();
extern "C" SPI_CLIENT_API void close();
extern "C" SPI_CLIENT_API void write(const int32_t);
