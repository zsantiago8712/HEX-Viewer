#include "Core/frontEnd.h"
#include <assert.h>
#include <curses.h>
#include <string.h>
#include "Core/backEnd.h"
#include "Utils/clog.h"
#include "Utils/dynamicAllocator.h"

#define MAX_WINDOW_BUFFER 3

static void showFilesInDirectory(WindowTreeFile* tree_file);

static void showFileContent(WindowBuffer* padding,
                            WindowBuffer* hex,
                            WindowBuffer* ascii,
                            const char* files_selected);

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

bool createAndConfigureWindowBuffer(WindowBuffer* buffer, const char* title) {
    static i32 window_count = 0;

    assert(window_count < MAX_WINDOW_BUFFER);

    i32 screen_height = 0;
    i32 screen_width = 0;

    getmaxyx(stdscr, screen_height, screen_width);
    assert(screen_height > 0 && screen_width > 0);

    buffer->config = (WindowConfig)allocate(sizeof(struct _WindowConfig));

    if (buffer->config == NULL) {
        LOG_FATAL("Failed to allocate memory for window config");
        return false;
    }

    struct _WindowConfig temp_config = {.title = (char*)title,
                                        .width = 0,
                                        .height = screen_height,
                                        .y_position = 0,
                                        .x_position = 0,
                                        .is_active = false,
                                        .window = NULL,
                                        .scroller = NULL};

    i32 first_width = screen_width / 5;
    i32 remaining_width = screen_width - first_width;
    i32 width_for_calculation = remaining_width / 2;

    i32 second_width = width_for_calculation / 2;
    i32 third_width = width_for_calculation;
    i32 fourth_width = remaining_width - (second_width + third_width);

    i32 second_x = first_width;
    i32 third_x = second_x + second_width;
    i32 fourth_x = third_x + third_width;

    switch (window_count) {
        case 0:  // Primera ventana
            temp_config.width = second_width;
            temp_config.x_position = second_x;
            break;
        case 1:  // Segunda ventana
            temp_config.width = third_width;
            temp_config.x_position = third_x;
            break;
        case 2:  // Cuarta ventana
            temp_config.width = fourth_width;
            temp_config.x_position = fourth_x;
            break;
        default:
            return false;
            break;
    }

    *buffer->config = temp_config;

    buffer->config->window =
        newwin(buffer->config->height, buffer->config->width,
               buffer->config->y_position, buffer->config->x_position);

    if (buffer->config->window == NULL) {
        LOG_FATAL("Failed to create window");
        deallocate(buffer->config, sizeof(struct _WindowConfig));
        return false;
    }

    buffer->config->scroller =
        derwin(buffer->config->window, buffer->config->height - 5,
               buffer->config->width - 3, 3, 2);

    if (buffer->config->scroller == NULL) {
        LOG_FATAL("Failed to create window");
        delwin(buffer->config->window);
        deallocate(buffer->config, sizeof(struct _WindowConfig));
        return false;
    }

    keypad(buffer->config->scroller, TRUE);
    scrollok(buffer->config->scroller, TRUE);

    box(buffer->config->window, 0, 0);
    mvwprintw(buffer->config->window, 1, 1, buffer->config->title);
    mvwhline(buffer->config->window, 2, 1, ACS_HLINE,
             buffer->config->width - 2);

    wrefresh(buffer->config->window);
    wrefresh(buffer->config->scroller);

    window_count++;
    LOG_INFO("Window tree file created");
    return true;
}

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
                                        .width = screen_width / 5,
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

    wmove(tree_file->config->scroller, 0, 0);

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

    // TODO: encontrar una manwera mas exacta de desalojar los archivos
    // deallocate(tree_file->files_in_directory,
    //            sizeof(char*) * tree_file->num_files);

    deallocate(tree_file->config->window, sizeof(struct _WindowConfig));
    LOG_INFO("Window buffer file terminated");
}

void terminateNcurses(void) {
    endwin();
    LOG_INFO("Ncurses terminated");
}

bool processKey(WindowTreeFile* tree_file,
                WindowBuffer* padding,
                WindowBuffer* hex,
                WindowBuffer* ascii,
                Cursor* cursor) {
    int key = '\0';
    while ((key = wgetch(tree_file->config->scroller)) != ERR) {
        LOG_DEBUG("key pressed: %d  [%#x]-> '%s'", key, key, keyname(key));
        switch (key) {
            case KEY_UP:
                if (cursor->y_position - 1 < 0) {
                    cursor->y_position = (i32)tree_file->num_files - 1 >= 0
                                             ? (i32)tree_file->num_files - 1
                                             : 0;
                } else {
                    cursor->y_position--;
                }
                wmove(tree_file->config->scroller, cursor->y_position,
                      cursor->x_position);
                break;
            case KEY_DOWN:
                if (cursor->y_position + 1 > (i32)tree_file->num_files - 1) {
                    cursor->y_position = 0;
                } else {
                    cursor->y_position++;
                }
                wmove(tree_file->config->scroller, cursor->y_position,
                      cursor->x_position);
                break;
            case 'q':
                return false;
                break;

            case '\n':
                showFileContent(
                    padding, hex, ascii,
                    tree_file->files_in_directory[cursor->y_position]);
                break;

            default:
                break;
        }
        wrefresh(padding->config->scroller);
        wrefresh(hex->config->scroller);
        wrefresh(ascii->config->scroller);
        wrefresh(tree_file->config->scroller);
    }

    return true;
}

/* -------------------- Static functions ------------------- */
static void showFilesInDirectory(WindowTreeFile* tree_file) {
    tree_file->files_in_directory = getFilesFromDirectory(
        tree_file->current_directory, &tree_file->num_files,
        &tree_file->largest_file_name_length);

    if (tree_file->files_in_directory == NULL || tree_file->num_files <= 0) {
        return;
    }

    for (u32 i = 0; i < tree_file->num_files; i++) {
        wprintw(tree_file->config->scroller, "%s\n",
                tree_file->files_in_directory[i]);
    }

    wrefresh(tree_file->config->scroller);
}

static void showFileContent(WindowBuffer* padding,
                            WindowBuffer* hex,
                            WindowBuffer* ascii,
                            const char* files_selected) {
    // NOTE: podria ser buena idea un multothreading aqui!
    if (ascii->buffer != NULL) {
        deallocate(ascii->buffer, (u32)strlen(ascii->buffer));
        wclear(ascii->config->scroller);
    }

    if (padding->buffer != NULL) {
        deallocate(padding->buffer, (u32)strlen(padding->buffer));
        wclear(padding->config->scroller);
    }

    if (hex->buffer != NULL) {
        deallocate(hex->buffer, (u32)strlen(hex->buffer));
        wclear(hex->config->scroller);
    }

    u32 num_lines = 0;
    u32 size_file = 0;
    ascii->buffer = getFileContent(files_selected, &num_lines, &size_file);
    padding->buffer = setPadding(num_lines);
    hex->buffer = setHexContent(ascii->buffer, num_lines, size_file);

    wprintw(padding->config->scroller, "%s", padding->buffer);
    wprintw(ascii->config->scroller, "%s", ascii->buffer);
    wprintw(hex->config->scroller, "%s", hex->buffer);
    LOG_DEBUG("File content ascii: %s", ascii->buffer);
    LOG_DEBUG("File content padding: %s", padding->buffer);
}
