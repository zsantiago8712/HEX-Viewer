#pragma once

#include "Core/frontEnd.h"

#define EmptyApp                                                        \
    {                                                                   \
        false, EmptyWindowBuffer, EmptyWindowBuffer, EmptyWindowBuffer, \
            EmptyWindowTreeFile                                         \
    }

typedef struct App {
    bool is_running;
    WindowBuffer window_hex;
    WindowBuffer window_ascii;
    WindowBuffer window_padding;
    WindowTreeFile window_tree_file;
} App;

void initApp(App* app);
void terminateApp(App* app);
void runApp(App* app);
