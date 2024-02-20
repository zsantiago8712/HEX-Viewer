#include "Core/app.h"
#include <stdlib.h>
#include "Core/frontEnd.h"
#include "Utils/clog.h"
#include "Utils/dynamicAllocator.h"

static void updateApp(App* app);

void initApp(App* app) {
    App temp = EmptyApp;
    *app = temp;

    initLogger(L_FILE);
    if (!initDynamicAllocator(MEGABYTES_TO_BYTES(500))) {
        LOG_FATAL("Failed to initialize dynamic memory allocator!");
        terminateLogger();
        exit(-1);
    }

    if (!initNcurses()) {
        LOG_FATAL("Failed to initialize ncurses!");
        terminateDynamicAllocator();
        terminateLogger();
        exit(-1);
    }

    if (!createAndConfigureWindowBuffer(&app->window_padding,
                                        "PADDING VIEWER")) {
        LOG_FATAL("Failed to create window buffer 'PADDING VIEWER'!");
        terminateWindowBuffer(&app->window_hex);
        terminateWindowBuffer(&app->window_ascii);
        terminateNcurses();
        terminateDynamicAllocator();
        terminateLogger();
        exit(-1);
    }

    if (!createAndConfigureWindowBuffer(&app->window_hex, "HEX VIEWER")) {
        LOG_FATAL("Failed to create window buffer 'HEX VIEWER'!");
        terminateNcurses();
        terminateDynamicAllocator();
        terminateLogger();
        exit(-1);
    }

    if (!createAndConfigureWindowBuffer(&app->window_ascii, "ASCII VIEWER")) {
        LOG_FATAL("Failed to create window buffer 'ASCII VIEWER'!");
        terminateWindowBuffer(&app->window_hex);
        terminateNcurses();
        terminateDynamicAllocator();
        terminateLogger();
        exit(-1);
    }

    if (!createAndConfigureWindowTreeFile(&app->window_tree_file)) {
        LOG_FATAL("Failed to create window tree file!");
        terminateWindowBuffer(&app->window_hex);
        terminateWindowBuffer(&app->window_ascii);
        terminateWindowBuffer(&app->window_padding);
        terminateNcurses();
        terminateDynamicAllocator();
        terminateLogger();
        exit(-1);
    }

    LOG_INFO("App initialized");
}

void terminateApp(App* app) {
    terminateWindowBuffer(&app->window_hex);
    terminateWindowBuffer(&app->window_ascii);
    terminateWindowBuffer(&app->window_padding);
    terminateWindowTreeFile(&app->window_tree_file);
    terminateNcurses();
    terminateDynamicAllocator();
    terminateLogger();
    LOG_INFO("App terminated");
}

void runApp(App* app) {
    while (app->is_running) {
        // app->is_running = false;
        updateApp(app);
    }
}

static void updateApp(App* app) {
    app->is_running =
        processKey(&app->window_tree_file, &app->window_padding,
                   &app->window_hex, &app->window_ascii, &app->cursor);
}
