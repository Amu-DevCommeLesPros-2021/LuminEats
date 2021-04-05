#include <stdio.h>

#include "ecrans.h"

#include "vector/vector.h"

int main()
{
    vector ecrans = make_vector(sizeof(ecran), 0, 2.0);

    ecran e = principal;
    push_back(&ecrans, &e);

    while(size(ecrans))
    {
        ecran e = *(ecran*)back(&ecrans);
        e(&ecrans);
    }

    destroy(&ecrans);

    return 0;
}