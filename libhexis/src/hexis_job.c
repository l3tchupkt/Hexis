#include "hexis_job.h"
#include <stdlib.h>
#include <string.h>

// A full job system requires OS-specific threading (pthreads/Windows Threads).
// For the architectural baseline, we scaffold the synchronous mock.
// In a full implementation, hexis_job_start would spawn a thread.

static uint32_t g_job_id_counter = 1;

HexisJob* hexis_job_create(const char* name, hexis_job_func_t func, void* ctx) {
    if (!name || !func) return NULL;
    
    HexisJob* job = (HexisJob*)calloc(1, sizeof(HexisJob));
    if (!job) return NULL;
    
    job->id = g_job_id_counter++;
    job->name = strdup(name);
    job->status = HEXIS_JOB_PENDING;
    job->progress_pct = 0;
    job->user_data = ctx;
    
    // Store func internally (requires an internal struct in full impl, omitting for brevity)
    
    return job;
}

int hexis_job_start(HexisJob* job) {
    if (!job) return -1;
    job->status = HEXIS_JOB_RUNNING;
    // PTHREAD_CREATE or similar would go here
    return 0;
}

int hexis_job_cancel(HexisJob* job) {
    if (!job) return -1;
    job->status = HEXIS_JOB_CANCELLED;
    return 0;
}

int hexis_job_suspend(HexisJob* job) {
    if (!job) return -1;
    if (job->status == HEXIS_JOB_RUNNING) {
        job->status = HEXIS_JOB_SUSPENDED;
        return 0;
    }
    return -1;
}

int hexis_job_resume(HexisJob* job) {
    if (!job) return -1;
    if (job->status == HEXIS_JOB_SUSPENDED) {
        job->status = HEXIS_JOB_RUNNING;
        return 0;
    }
    return -1;
}

void hexis_job_update_progress(HexisJob* job, uint8_t pct) {
    if (!job) return;
    job->progress_pct = pct > 100 ? 100 : pct;
}

void hexis_job_free(HexisJob* job) {
    if (!job) return;
    if (job->name) free((void*)job->name);
    free(job);
}
