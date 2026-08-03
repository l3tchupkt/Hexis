#ifndef HEXIS_SESSION_H
#define HEXIS_SESSION_H

#include "hexis.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SESSION_CMD_CONNECT,
    SESSION_CMD_READ,
    SESSION_CMD_WRITE,
    SESSION_CMD_ERASE,
    SESSION_CMD_VERIFY,
    SESSION_CMD_DISCONNECT
} hexis_session_cmd_t;

typedef struct {
    hexis_session_cmd_t cmd;
    uint64_t address;
    size_t length;
    uint32_t checksum; // Data block checksum for verification
    int result_code;
} hexis_session_event_t;

typedef struct {
    const char* filename;
    bool is_recording;
    bool is_replaying;
    // Internal state omitted
    void* internal;
} HexisSession;

HEXIS_API HexisSession* hexis_session_start_record(const char* filepath);
HEXIS_API HexisSession* hexis_session_start_replay(const char* filepath);
HEXIS_API void hexis_session_close(HexisSession* session);

HEXIS_API int hexis_session_log_event(HexisSession* session, const hexis_session_event_t* event, const uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_SESSION_H
