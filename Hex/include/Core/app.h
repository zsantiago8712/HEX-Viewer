#pragma once

#include "Core/frontEnd.h"

#define EmptyApp                                                        \
    {                                                                   \
        EmptyWindowBuffer, EmptyWindowBuffer, EmptyWindowBuffer, \
            EmptyWindowTreeFile, true,  EmptyCursor                                         \
    }

typedef struct App {
    WindowBuffer window_hex;
    WindowBuffer window_ascii;
    WindowBuffer window_padding;
    WindowTreeFile window_tree_file;
    bool is_running;
    Cursor cursor;
} App;

void initApp(App* app);
void terminateApp(App* app);
void runApp(App* app);
