#include "hexis_project.h"
#include "hexis_cas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

HexisProject* hexis_project_create(const char* dir_path, const char* name) {
    if (!dir_path || !name) return NULL;
    
    HexisProject* proj = (HexisProject*)calloc(1, sizeof(HexisProject));
    proj->project_dir = strdup(dir_path);
    proj->name = strdup(name);
    proj->active_graph = hexis_graph_create();
    
    // Create base project dir
    mkdir(dir_path, 0755);
    
    // Create objects CAS dir
    char obj_dir[1024];
    snprintf(obj_dir, sizeof(obj_dir), "%s/objects", dir_path);
    hexis_cas_init(obj_dir);
    
    return proj;
}

HexisProject* hexis_project_open(const char* dir_path) {
    if (!dir_path) return NULL;
    
    // In a full implementation, this reads session.json and reconstructs state
    HexisProject* proj = (HexisProject*)calloc(1, sizeof(HexisProject));
    proj->project_dir = strdup(dir_path);
    proj->name = strdup("Loaded Project");
    proj->active_graph = hexis_graph_create();
    
    char obj_dir[1024];
    snprintf(obj_dir, sizeof(obj_dir), "%s/objects", dir_path);
    hexis_cas_init(obj_dir);
    
    return proj;
}

int hexis_project_save(HexisProject* project) {
    if (!project) return -1;
    
    char session_file[1024];
    snprintf(session_file, sizeof(session_file), "%s/session.json", project->project_dir);
    
    FILE* f = fopen(session_file, "w");
    if (!f) return -2;
    
    fprintf(f, "{\n  \"project\": \"%s\",\n  \"version\": 1\n}\n", project->name);
    fclose(f);
    
    return 0;
}

void hexis_project_close(HexisProject* project) {
    if (!project) return;
    if (project->project_dir) free((void*)project->project_dir);
    if (project->name) free((void*)project->name);
    if (project->active_graph) hexis_graph_free(project->active_graph);
    free(project);
}

int hexis_project_export(HexisProject* project, const char* out_filepath) {
    if (!project || !out_filepath) return -1;
    
    // Mock ZIP export via system call for the SDK architecture
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "zip -r -j -q %s %s/*", out_filepath, project->project_dir);
    return system(cmd);
}

int hexis_project_import(const char* hexis_filepath, const char* out_dir_path) {
    if (!hexis_filepath || !out_dir_path) return -1;
    
    mkdir(out_dir_path, 0755);
    
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "unzip -q %s -d %s", hexis_filepath, out_dir_path);
    return system(cmd);
}
