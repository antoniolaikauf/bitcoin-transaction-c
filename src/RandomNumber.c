#include "Global.h"

void init_state(struct Random_num *number)
{
    number->state[0] = number->seed;

    for (size_t id_state = 0; id_state < N; id_state++)
    {
        number->seed = F * (number->seed ^ (number->seed >> (W - 2))) + id_state;
        number->state[id_state] = number->seed;
    }
    number->state_index = 0;
}

int main()
{
    struct Random_num number;
    number.seed = 0;
    init_state(&number);

    printf("hello word %d \n", number.state_index);
    return 0;
}