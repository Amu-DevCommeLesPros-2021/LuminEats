#include "ecrans/ecrans.h"

#include "prompt.h"

#include "lumineats/lumineats.h"

#include <stdio.h>
#include <string.h>

void initial(
    vector* pile)
{
    printf("\n\
* Menu Principal *\n\
\n\
1. Vous connecter à votre compte\n\
2. Créer un nouveau compte\n\n");

    char const choice = prompt_choice("Votre choix ('q' pour quitter) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = connexion_compte;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = creation_compte;
                push_back(pile, &e);
            }
            break;
        case 'q':
            clear(pile);
            break;
        case 'z':
            system("clear");
            le_dump_tables(stdout);
            getchar();
            break;
    }
}
