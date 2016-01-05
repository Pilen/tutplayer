#include "init.h"
#include "output.h"

int main(int argc, char *argv[]) {

    PlayerState *state = init_default();
    output_stop(state);
    (void) state;
    return 0;
}
