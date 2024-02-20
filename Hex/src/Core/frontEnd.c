#include "Core/frontEnd.h"
#include <assert.h>
#include <curses.h>
#include "Core/backEnd.h"
#include "Utils/clog.h"
#include "Utils/dynamicAllocator.h"

static void showFilesInDirectory(WindowTreeFile* tree_file);

struct _WindowConfig {
    char* title;
    i32 width;
    i32 height;

    i32 x_position;
    i32 y_position;

    bool is_active;

    WINDOW* window;
    WINDOW* scroller;
};

bool initNcurses(void) {
    initscr();

    keypad(stdscr, TRUE);
    noecho();
    cbreak();

    nodelay(stdscr, TRUE);

    LOG_INFO("Ncurses initialized");

    return true;
}

// TODO: arreglar los tamaños de las venetas, se estan encimando
WindowConfig createAndConfigureWindowBuffer(const char* title) {
    i32 screen_height = 0;
    i32 screen_width = 0;

    WindowConfig config = NULL;

    getmaxyx(stdscr, screen_height, screen_width);
    assert(screen_height > 0 && screen_width > 0);

    config = (WindowConfig)allocate(sizeof(struct _WindowConfig));

    if (config == NULL) {
        LOG_FATAL("Failed to allocate memory for window config");
        return false;
    }

    // TODO: quiero que el tamaño de las 3 panatallas sea el mismo, que se
    // dividan"(tamaño - (tamaño / 6)) / 3"
    struct _WindowConfig temp_config = {
        .title = (char*)title,
        .width = screen_height,
        .height = (screen_width - (screen_width / 6)) / 3,
        .y_position = 0,
        .x_position = screen_width / 6,
        .is_active = false,
        .window = NULL,
        .scroller = NULL};

    *config = temp_config;

    config->window = newwin(config->height, config->width, config->y_position,
                            config->x_position);

    if (config->window == NULL) {
        LOG_FATAL("Failed to create window");
        deallocate(config, sizeof(struct _WindowConfig));
        return false;
    }

    config->scroller =
        derwin(config->window, config->height - 1, config->width - 1, 1, 1);

    if (config->scroller == NULL) {
        LOG_FATAL("Failed to create window");
        delwin(config->window);
        deallocate(config, sizeof(struct _WindowConfig));
        return false;
    }

    keypad(config->scroller, TRUE);
    scrollok(config->scroller, TRUE);

    box(config->window, 0, 0);
    mvwprintw(config->window, 1, 1, config->title);
    mvwhline(config->window, 2, 1, ACS_HLINE, config->width - 2);

    wrefresh(config->window);
    wrefresh(config->scroller);

    LOG_INFO("Window tree file created");
    return config;
}
// TODO: hacer la vemtana aun mas pequeña
bool createAndConfigureWindowTreeFile(WindowTreeFile* tree_file) {
    i32 screen_height = 0;
    i32 screen_width = 0;

    getmaxyx(stdscr, screen_height, screen_width);
    assert(screen_height > 0 && screen_width > 0);

    tree_file->config = (WindowConfig)allocate(sizeof(struct _WindowConfig));

    if (tree_file == NULL) {
        LOG_FATAL("Failed to allocate memory for window config");
        return false;
    }

    struct _WindowConfig temp_config = {.title = "Tree File",
                                        .width = screen_width / 6,
                                        .height = screen_height,
                                        .y_position = 0,
                                        .x_position = 0,
                                        .is_active = true,
                                        .window = NULL,
                                        .scroller = NULL};

    *tree_file->config = temp_config;

    tree_file->config->window =
        newwin(tree_file->config->height, tree_file->config->width,
               tree_file->config->y_position, tree_file->config->x_position);

    if (tree_file->config->window == NULL) {
        LOG_FATAL("Failed to create window");
        deallocate(tree_file, sizeof(struct _WindowConfig));
        return false;
    }

    tree_file->config->scroller =
        derwin(tree_file->config->window, tree_file->config->height - 6,
               tree_file->config->width - 3, 3, 2);

    if (tree_file->config->scroller == NULL) {
        LOG_FATAL("Failed to create window");
        delwin(tree_file->config->window);
        deallocate(tree_file, sizeof(struct _WindowConfig));
        return false;
    }

    keypad(tree_file->config->scroller, TRUE);
    scrollok(tree_file->config->scroller, TRUE);

    box(tree_file->config->window, 0, 0);
    mvwprintw(tree_file->config->window, 1, 1, tree_file->config->title);
    mvwhline(tree_file->config->window, 2, 1, ACS_HLINE,
             tree_file->config->width - 2);

    wrefresh(tree_file->config->window);
    wrefresh(tree_file->config->scroller);

    showFilesInDirectory(tree_file);

    LOG_INFO("Window tree file created");
    return true;
}

void terminateWindowBuffer(WindowBuffer* buffer) {
    delwin(buffer->config->window);
    delwin(buffer->config->scroller);
    deallocate(buffer->config->window, sizeof(struct _WindowConfig));
    // deallocate(buffer->buffer, buffer->buffer_size);

    LOG_INFO("Window buffer file terminated");
}

void terminateWindowTreeFile(WindowTreeFile* tree_file) {
    delwin(tree_file->config->window);
    delwin(tree_file->config->window);

    // TODO: encontrar una manwera mas exacat de desalojar los archivos
    // deallocate(tree_file->files_in_directory,
    //            sizeof(char*) * tree_file->num_files);

    deallocate(tree_file->config->window, sizeof(struct _WindowConfig));
    LOG_INFO("Window buffer file terminated");
}

void terminateNcurses(void) {
    endwin();
    LOG_INFO("Ncurses terminated");
}

static void showFilesInDirectory(WindowTreeFile* tree_file) {
    // TODO: mostrar los archivos

    if (!getFilesFromDirectory(
            tree_file->current_directory, &tree_file->files_in_directory,
            &tree_file->num_files, &tree_file->largest_file_name_length) ||
        tree_file->num_files <= 0) {
        return;
    }

    for (u32 i = 0; i < tree_file->num_files; i++) {
        wprintw(tree_file->config->scroller, "%s\n",
                tree_file->files_in_directory[i]);
    }

    wrefresh(tree_file->config->scroller);
}
