#include "../Hex/include/Core/app.h"

int main(void) {
    App app;

    initApp(&app);
    runApp(&app);
    terminateApp(&app);

    return 0;
}
