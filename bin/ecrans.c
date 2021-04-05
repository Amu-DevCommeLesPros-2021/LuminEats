#include "ecrans.h"

#include <stdio.h>

void principal(vector* pile)
{
    printf("\n\
*** Bienvenu sur LuminEats, la livraison à vitesse luminique ***\n\
\n\
* Menu Principal *\n\
\n\
Vous êtes :\n\
1. Un·e restaurateur·trice\n\
2. Un·e livreur·se\n\
3. Un·e client·e\n\
\n\
Votre choix ('q' pour quitter) : ");

    char c;
    scanf("%c%*c", &c);

    switch(c)
    {
        case '1':
            {
                ecran e = connexion_restaurateur;
                push_back(pile, &e);
            }
            break;
        case '2':
            break;
        case '3':
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void connexion_restaurateur(vector* pile)
{
    printf("\n\
* Menu Restaurateur *\n\
\n\
Vous voulez :\n\
1. Vous connecter à votre compte\n\
2. Créer un nouveau compte\n\
\n\
Votre choix ('q' pour quitter, 'p' pour menu précédent) : \n");

    char c;
    scanf("%c%*c", &c);

    switch(c)
    {
        case '1':
            {
                ecran e = restaurateur;
                push_back(pile, &e);
            }
            break;
        case '2':
            break;
        case '3':
            break;
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void restaurateur(vector* pile)
{
    printf("\n\
* Menu Restaurateur *\n\
\n\
Vous voulez :\n\
1. Supprimer votre compte\n\
2. Modifier votre menu (ajouter/modifier/supprimer)\n\
3. Confirmer votre solde\n\
\n\
Votre choix ('q' pour quitter, 'p' pour menu précédent) : ");

    char c;
    scanf("%c%*c", &c);

    switch(c)
    {
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
    }
}