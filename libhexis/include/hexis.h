#ifndef HEXIS_H
#define HEXIS_H

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(HEXIS_EXPORTS)
    #define HEXIS_API __declspec(dllexport)
  #else
    #define HEXIS_API __declspec(dllimport)
  #endif
#else
  #define HEXIS_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Hexis library.
 * @return 0 on success, non-zero on error.
 */
HEXIS_API int hexis_init(void);

/**
 * @brief Get the Hexis version string.
 * @return Version string (e.g. "1.0.0").
 */
HEXIS_API const char* hexis_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_H */
