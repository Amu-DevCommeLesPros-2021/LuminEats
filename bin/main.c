#include "ecrans.h"

#include "db/db.h"
#include "vector/vector.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char *db_folder = NULL;

    int c;
    while((c = getopt (argc, argv, "b:")) != -1)
    {
        switch(c)
        {
            case 'b':
                db_folder = optarg;
                break;
            default:
                abort();
                break;
        }
    }

    ouverture_db(db_folder);

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

    fermeture_db(db_folder);

    return 0;
}