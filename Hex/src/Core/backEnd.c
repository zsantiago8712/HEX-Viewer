#include "Core/backEnd.h"
#include <dirent.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Utils/clog.h"
#include "Utils/dynamicAllocator.h"

static u32 getLinesFromSize(const u32 size_data);

char** getFilesFromDirectory(const char* path,
                             u32* num_files,
                             u32* largest_file_name_length) {
    DIR* dir;
    struct dirent* ent;
    u64 temp = 0;
    u32 size_array = 0;
    u32 i = 0;
    char** files_in_directory;

    dir = opendir(path);

    *num_files = 0;

    *largest_file_name_length = 0;

    if (dir == NULL) {
        LOG_FATAL("No se puede abrir el directorio");
        return NULL;
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
        return NULL;
    }

    *largest_file_name_length += 1;

    size_array =
        (*largest_file_name_length * *num_files) + (*num_files * sizeof(char*));

    rewinddir(dir);

    files_in_directory = (char**)allocate(size_array);
    if (files_in_directory == NULL) {
        LOG_FATAL(
            "No se pudo reservar memoria para los archivos dentro del "
            "directorio");
        closedir(dir);
        return false;
    }

    char* start_location = (char*)(files_in_directory + *num_files);
    u32 s;

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            s = (u32)strlen(ent->d_name);
            files_in_directory[i] = start_location;
            strcpy(files_in_directory[i], ent->d_name);
            files_in_directory[i][s] = '\0';
            start_location += *largest_file_name_length;
            i++;
        }
    }

    // Cerrar el directorio
    closedir(dir);
    return files_in_directory;
}

char* getFileContent(const char* file, u32* num_lines, u32* size_file) {
    char* temp_data = NULL;

    char* data = NULL;
    int fd = open(file, O_RDONLY);
    struct stat st;

    if (fd == -1) {
        LOG_ERROR("FILE not found [%s]", file);
        return NULL;
    }

    fstat(fd, &st);
    temp_data = mmap(0, (size)st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (temp_data == MAP_FAILED) {
        close(fd);
        LOG_DEBUG("Error mapeando el archivo");
        return NULL;
    }

    *num_lines = getLinesFromSize((u32)st.st_size);
    *size_file = (u32)st.st_size;

    data = allocate((*num_lines * 17) + 1);
    if (data == NULL) {
        close(fd);
        LOG_ERROR("Failed to allocate memory for file content");
        return NULL;
    }

    memset(data, '.', *num_lines * 17);

    u32 j = 0;
    u32 c = 0;
    for (u32 i = 0; i < (u32)st.st_size; i++, j++) {
        if (temp_data[j] != '\n') {
            if ((i + c) % (16 + c) != 0 || i == 0) {
                data[i] = temp_data[j];
            } else {
                data[i++] = '\n';
                data[i] = temp_data[j];
                c++;
            }
        } else {
            data[i] = '\n';
            LOG_DEBUG("New line detected");
        }
    }

    data[j + 2] = temp_data[(u32)st.st_size - 2];
    data[(*num_lines * 17) - 1] = '\n';
    data[(*num_lines * 17)] = '\0';
    if (munmap(temp_data, (size_t)st.st_size) == -1) {
        LOG_ERROR("Error al desmapear el archivo");
    }
    // Cierra el descriptor del archivo
    close(fd);
    return data;
}

u32 getLines(char* data) {
    u64 size_data = strlen(data);
    float lines = (float)size_data / 16.0f;

    if (lines == floor(lines)) {
        return (u32)lines;
    }

    return ((u32)lines) + 1;
}

static u32 getLinesFromSize(const u32 size_data) {
    float lines = (float)size_data / 16.0f;
    if (lines == floor(lines)) {
        return (u32)lines;
    }
    return ((u32)lines) + 1;
}

char* setPadding(const u32 num_lines) {
    char* padding = allocate((num_lines * 9) + 1);

    if (padding == NULL) {
        LOG_ERROR("Failed to allocate memory for padding");
        return NULL;
    }

    for (u32 i = 0; i < num_lines; i++) {
        sprintf(padding + (i * 9), "%08x\n", i * 16);
    }

    return padding;
}

char* setHexContent(char* data, const u32 num_lines, const u32 file_size) {
    char* hex = allocate((num_lines * (16 * 3 + 1)) + 1);
    if (hex == NULL) {
        LOG_ERROR("Failed to allocate memory for hex content");
        return NULL;
    }

    char* hex_ptr = hex;
    for (u32 line = 0; line < num_lines; line++) {
        for (u32 i = 0; i < 16; i++) {
            u32 index = line * 17 + i;
            if (index <= file_size) {
                if (data[index] == '\n' && index % 17 != 0) {
                    // Si es un salto de línea y no está al final, imprime su
                    // valor hexadecimal
                    sprintf(hex_ptr, "0a ");
                    // Reemplaza el salto de línea en 'data' por un punto
                    data[index] = '.';
                    hex_ptr += 3;
                } else if (data[index] != '\n') {
                    // De lo contrario, imprime el valor hexadecimal del
                    // caracter
                    sprintf(hex_ptr, "%02x ", data[index] & 0xFF);
                    hex_ptr += 3;
                }
            } else {
                // Si no hay más datos, rellena con '00'
                sprintf(hex_ptr, "00 ");
                hex_ptr += 3;
            }
            // Cada byte en hexadecimal ocupa 3 caracteres (2
            // dígitos y un espacio)
        }
        // Al final de cada línea de 16 bytes, coloca un salto de línea
        *hex_ptr++ = '\n';
    }
    // Termina la cadena correctamente
    *hex_ptr = '\0';

    return hex;
}
