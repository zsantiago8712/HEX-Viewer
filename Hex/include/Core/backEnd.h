#pragma once

#include "Utils/types.h"

char** getFilesFromDirectory(const char* path,
                             u32* num_files,
                             u32* largest_file_name_length);

u32 getLines(char* data);

char* getFileContent(const char* file, u32* num_lines, u32* size_file);
char* setPadding(const u32 num_lines);
char* setHexContent(char* data, const u32 num_lines, const u32 file_size);
