#include "ecrans.h"

#include "db/db.h"
#include "vector/vector.h"

#include <sys/syslimits.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char db_folder[PATH_MAX] = "./";
    bool reset = false;

    int c;
    while((c = getopt(argc, argv, "rb:")) != -1)
    {
        switch(c)
        {
            case 'b':
                strcpy(db_folder, optarg);
                break;
            case 'r':
                reset = true;
                break;
            default:
                abort();
                break;
        }
    }

    if(reset)
    {
        efface_db(db_folder);
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