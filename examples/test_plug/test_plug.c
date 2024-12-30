#define LOTUS_PLUG
#include "../../lotus/include/lotus.h"

LOTUS_API_ENTRY void hello_plug(void);
LOTUS_API_ENTRY void goodbye_plug(void);

void plug_init(void) {
    printf("My Lotus Plug Init!\n");
}

void plug_shutdown(void) {
    printf("My Lotus Plug Shutdown!\n");
}

void hello_plug(void) {
    printf("Hello Lotus Plug!\n");
}

void goodbye_plug(void) {
    printf("Goodbye Lotus Plug!\n");
}
