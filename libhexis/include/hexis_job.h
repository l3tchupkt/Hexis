#ifndef HEXIS_JOB_H
#define HEXIS_JOB_H

#include "hexis.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HEXIS_JOB_PENDING,
    HEXIS_JOB_RUNNING,
    HEXIS_JOB_SUSPENDED,
    HEXIS_JOB_FINISHED,
    HEXIS_JOB_CANCELLED,
    HEXIS_JOB_ERROR
} hexis_job_status_t;

typedef struct HexisJob {
    uint32_t id;
    const char* name;
    hexis_job_status_t status;
    uint8_t progress_pct; // 0-100
    void* user_data;
} HexisJob;

typedef int (*hexis_job_func_t)(HexisJob* job, void* ctx);

HEXIS_API HexisJob* hexis_job_create(const char* name, hexis_job_func_t func, void* ctx);
HEXIS_API int hexis_job_start(HexisJob* job);
HEXIS_API int hexis_job_cancel(HexisJob* job);
HEXIS_API int hexis_job_suspend(HexisJob* job);
HEXIS_API int hexis_job_resume(HexisJob* job);
HEXIS_API void hexis_job_update_progress(HexisJob* job, uint8_t pct);
HEXIS_API void hexis_job_free(HexisJob* job);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_JOB_H
