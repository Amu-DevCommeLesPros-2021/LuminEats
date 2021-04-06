#include <stdio.h>

#include "ecrans.h"

#include "vector/vector.h"

int main()
{
    printf("\n*** Bienvenu sur LuminEats, la livraison à vitesse luminique ***\n");

    vector ecrans = make_vector(sizeof(ecran), 0, 2.0);

    ecran e = initial;
    push_back(&ecrans, &e);

    while(size(ecrans))
    {
        ecran e = *(ecran*)back(&ecrans);
        e(&ecrans);
    }

    destroy(&ecrans);

    printf("\n*** À très bientôt ! ***\n");
    
    return 0;
}