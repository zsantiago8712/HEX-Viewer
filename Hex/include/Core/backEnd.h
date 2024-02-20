#pragma once

#include "Utils/types.h"

bool getFilesFromDirectory(const char* path,
                           char*** files_in_directory,
                           u32* num_files,
                           u32* largest_file_name_length);
