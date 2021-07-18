#include <stdio.h>

void
gamemaster_abort_option(void) {
    char answer[16];

    /* Give the gamemaster a chance to abort. */
    printf("*** Gamemaster safe-abort option ... type q or Q to quit: ");
    fflush(stdout);
    fgets(answer, 16, stdin);
    if (answer[0] == 'q' || answer[0] == 'Q') {
        exit(0);
    }
}
