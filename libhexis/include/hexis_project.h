#ifndef HEXIS_PROJECT_H
#define HEXIS_PROJECT_H

#include "hexis.h"
#include "hexis_graph.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HexisProject {
    const char* project_dir;
    const char* name;
    
    // Serialized state references
    HexisGraph* active_graph;
} HexisProject;

/**
 * @brief Create a new Hexis project directory (.hexisproj).
 */
HEXIS_API HexisProject* hexis_project_create(const char* dir_path, const char* name);

/**
 * @brief Open an existing Hexis project.
 */
HEXIS_API HexisProject* hexis_project_open(const char* dir_path);

/**
 * @brief Save the current project state (graph, metadata) to JSON.
 */
HEXIS_API int hexis_project_save(HexisProject* project);

/**
 * @brief Close the project.
 */
HEXIS_API void hexis_project_close(HexisProject* project);

/**
 * @brief Export the project to a .hexis ZIP archive.
 */
HEXIS_API int hexis_project_export(HexisProject* project, const char* out_filepath);

/**
 * @brief Import a .hexis ZIP archive into a workspace directory.
 */
HEXIS_API int hexis_project_import(const char* hexis_filepath, const char* out_dir_path);

#ifdef __cplusplus
}
#endif

#endif // HEXIS_PROJECT_H
