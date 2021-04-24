#include "ecrans/ecrans.h"

#include "db/db.h"
#include "logger/logger.h"
#include "vector/vector.h"

#if defined(__linux__)
    #include <linux/limits.h>
#else
    #include <sys/syslimits.h>
#endif
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char working_folder[PATH_MAX] = "./";
    bool reset = false;

    int c;
    while((c = getopt(argc, argv, "rb:")) != -1)
    {
        switch(c)
        {
            case 'b':
                strcpy(working_folder, optarg);
                break;
            case 'r':
                reset = true;
                break;
            default:
                abort();
                break;
        }
    }

    char *log_file;
    asprintf(&log_file, "%s/%s", working_folder, "log.txt");
    lopen(log_file);
    free(log_file);

    if(reset)
    {
        efface_db(working_folder);
    }

    ouverture_db(working_folder);

    printf("\n*** Bienvenu sur LuminEats, la livraison à vitesse luminique ***\n");

    vector ecrans = make_vector(sizeof(ecran), 0);

    ecran e = initial;
    push_back(&ecrans, &e);

    while(size(ecrans))
    {
        system("clear");
        ecran e = *(ecran*)back(&ecrans);
        e(&ecrans);
    }

    destroy(&ecrans);

    printf("\n*** À très bientôt ! ***\n");

    fermeture_db(working_folder);

    lclose();

    return 0;
}
