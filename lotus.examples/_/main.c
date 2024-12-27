#define LOTUS_APPLICATION
#define LOTUS_ECS
#define LOTUS_MATH
#define LOTUS_DATASTRUCTS
#include "../../include/lotus.h"

int main() {
    f32* vertices = lotus_make_array(sizeof(ubyte4), 2);
    lotus_print_array("Vertices", vertices);

    lotus_insert_array(vertices, 0, LOTUS_LITERAL(f32, 666.0));
    lotus_push_array(vertices, LOTUS_LITERAL(f32, 420.0));
    lotus_insert_array(vertices, 2, LOTUS_LITERAL(f32, 69.0));

    for (int i = 0; i < 3; i++) {
        f32 vertex;
        lotus_pop_array(vertices, &vertex);
        printf("Vertex: %0.1f\n", vertex);
    }
    
    lotus_print_array("Vertices", vertices);

    lotus_destroy_array(vertices);

    return 0;
}
