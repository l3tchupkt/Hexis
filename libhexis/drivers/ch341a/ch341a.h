#ifndef HEXIS_CH341A_H
#define HEXIS_CH341A_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Register the CH341A driver into the Hexis framework.
 * @return 0 on success, non-zero on failure.
 */
int hexis_ch341a_register(void);

#ifdef __cplusplus
}
#endif

#endif /* HEXIS_CH341A_H */
