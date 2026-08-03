#include "hexis_session.h"
#include "hexis_cas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HexisSession* hexis_session_start_record(const char* filepath) {
    if (!filepath) return NULL;
    
    HexisSession* session = (HexisSession*)calloc(1, sizeof(HexisSession));
    if (!session) return NULL;
    
    session->filename = strdup(filepath);
    session->is_recording = true;
    
    FILE* f = fopen(filepath, "w");
    if (f) {
        fprintf(f, "{\n  \"hexis_session_version\": 1,\n  \"events\": [\n");
        fclose(f);
    } else {
        free((void*)session->filename);
        free(session);
        return NULL;
    }
    
    return session;
}

HexisSession* hexis_session_start_replay(const char* filepath) {
    if (!filepath) return NULL;
    
    HexisSession* session = (HexisSession*)calloc(1, sizeof(HexisSession));
    if (!session) return NULL;
    
    session->filename = strdup(filepath);
    session->is_replaying = true;
    
    return session;
}

void hexis_session_close(HexisSession* session) {
    if (!session) return;
    
    if (session->is_recording) {
        FILE* f = fopen(session->filename, "a");
        if (f) {
            fprintf(f, "\n  ]\n}\n");
            fclose(f);
        }
    }
    
    if (session->filename) free((void*)session->filename);
    free(session);
}

int hexis_session_log_event(HexisSession* session, const hexis_session_event_t* event, const uint8_t* data) {
    if (!session || !session->is_recording || !event) return -1;
    
    FILE* f = fopen(session->filename, "a");
    if (!f) return -1;
    
    const char* cmd_str = "UNKNOWN";
    switch (event->cmd) {
        case SESSION_CMD_CONNECT: cmd_str = "CONNECT"; break;
        case SESSION_CMD_READ: cmd_str = "READ"; break;
        case SESSION_CMD_WRITE: cmd_str = "WRITE"; break;
        case SESSION_CMD_ERASE: cmd_str = "ERASE"; break;
        case SESSION_CMD_VERIFY: cmd_str = "VERIFY"; break;
        case SESSION_CMD_DISCONNECT: cmd_str = "DISCONNECT"; break;
    }
    
    // Use Content-Addressed Storage to deduplicate firmware reads/writes
    char cas_ref[128] = "null";
    if (data && event->length > 0) {
        hexis_cas_hash_t hash;
        if (hexis_cas_add(data, event->length, &hash) == 0) {
            snprintf(cas_ref, sizeof(cas_ref), "\"%s\"", hash.hash);
        }
    }
    
    fprintf(f, "    { \"cmd\": \"%s\", \"address\": %llu, \"length\": %zu, \"result\": %d, \"cas_hash\": %s },\n", 
            cmd_str, (unsigned long long)event->address, event->length, event->result_code, cas_ref);
            
    fclose(f);
    return 0;
}
