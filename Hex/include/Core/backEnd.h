#pragma once

#include "Utils/types.h"

char** getFilesFromDirectory(const char* path,
                             u32* num_files,
                             u32* largest_file_name_length);
