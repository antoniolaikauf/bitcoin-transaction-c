#include "Global.h"

void init_state(struct Random_num *number)
{
    number->state[0] = number->seed;

    for (size_t id_state = 1; id_state < N; id_state++)
    {
        number->seed = F * (number->seed ^ (number->seed >> (W - 2))) + id_state;
        number->state[id_state] = number->seed;
    }
    number->state_index = 0;
}

uint32_t get_random_number(int *state_index, uint32_t *state_array)
{

    int j = *state_index - (N - 1);
    if (j < 0)
        j += N;

    uint32_t x = (state_array[*state_index] & UMASK) | (state_array[j] & LMASK);

    uint32_t xA = x >> 1;

    if (x & 0x00000001UL)
        xA ^= A;

    j = *state_index - (N - M);

    if (j < 0)
        j += N;

    x = state_array[j] ^ xA;
    state_array[*state_index++] = x;

    if (*state_index >= N)
        *state_index = 0;

    uint32_t y = x ^ (x >> U);
    y = y ^ ((y << S) & B);
    y = y ^ ((y << T) & C);
    uint32_t z = y ^ (y >> L);

    return z;
}

int main()
{
    struct Random_num number;
    number.seed = 1;
    init_state(&number);
    for (size_t i = 0; i < N; i++)
    {
        printf("value --> %d \n", number.state[i]);
    }

    uint32_t numero = get_random_number(&number.state_index, number.state);
    printf(" numero --> %d\n", numero);
    return 0;
}