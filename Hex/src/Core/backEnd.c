#include "Core/backEnd.h"
#include <dirent.h>
#include <string.h>
#include "Utils/clog.h"
#include "Utils/dynamicAllocator.h"

bool getFilesFromDirectory(const char* path,
                           char*** files_in_directory,
                           u32* num_files,
                           u32* largest_file_name_length) {
    DIR* dir;
    struct dirent* ent;
    u64 temp = 0;
    u32 size_array = 0;
    u32 i = 0;

    dir = opendir(path);

    *num_files = 0;

    *largest_file_name_length = 0;

    if (dir == NULL) {
        LOG_FATAL("No se puede abrir el directorio");
        return false;
    }

    // Leer las entradas del directorio
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            temp = strlen(ent->d_name);
            if (temp > *largest_file_name_length) {
                *largest_file_name_length = (u32)temp;
            }
            *num_files += 1;
        }
    }

    if (*num_files == 0) {
        *largest_file_name_length = 0;
        closedir(dir);
        LOG_INFO("No hay archivos en el directorio");
        return true;
    }

    *largest_file_name_length += 1;

    size_array =
        (*largest_file_name_length * *num_files) + (*num_files * sizeof(char*));

    rewinddir(dir);

    *files_in_directory = (char**)allocate(size_array);
    if (*files_in_directory == NULL) {
        LOG_FATAL(
            "No se pudo reservar memoria para los archivos dentro del "
            "directorio");
        closedir(dir);
        return false;
    }

    char* start_location = (char*)(*files_in_directory + *num_files);
    u32 s;

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            s = (u32)strlen(ent->d_name);
            (*files_in_directory)[i] = start_location;
            strcpy((files_in_directory[0][i]), ent->d_name);
            files_in_directory[0][i][s] = '\0';
            start_location += *largest_file_name_length;
            i++;
        }
    }

    // Cerrar el directorio
    closedir(dir);

    return true;
}
