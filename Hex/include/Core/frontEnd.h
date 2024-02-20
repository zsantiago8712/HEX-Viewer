#pragma once

#include "Utils/types.h"

typedef struct _WindowConfig* WindowConfig;

#define EmptyWindowBuffer \
    { NULL, NULL, 0 }

#define EmptyWindowTreeFile \
    { NULL, "./", NULL, 0, 0 }

#define EmptyCursor \
    { 0, 0 }

typedef struct {
    WindowConfig config;
    char* buffer;
    u32 buffer_size;
} WindowBuffer;

typedef struct {
    WindowConfig config;

    char* current_directory;
    char** files_in_directory;
    u32 num_files;
    u32 largest_file_name_length;

} WindowTreeFile;

typedef struct {
    i32 x_position;
    i32 y_position;
} Cursor;

bool initNcurses(void);

bool createAndConfigureWindowBuffer(WindowBuffer* buffer, const char* title);
bool createAndConfigureWindowTreeFile(WindowTreeFile* tree_file);

void terminateWindowBuffer(WindowBuffer* buffer);

void terminateWindowTreeFile(WindowTreeFile* tree_file);

void terminateNcurses(void);
bool processKey(WindowTreeFile* tree_file,
                WindowBuffer* padding,
                WindowBuffer* hex,
                WindowBuffer* ascii,
                Cursor* cursor);
