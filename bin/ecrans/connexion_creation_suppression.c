#include "ecrans/ecrans.h"

#include "prompt.h"

#include "lumineats/lumineats.h"

#include <stdio.h>
#include <string.h>

void connexion_compte(
    vector* pile)
{
    printf("\n* Connexion à votre compte*\n\n");

    char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez nom ou téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            if(!le_compte_existe(saisie))
            {
                printf("\nErreur : ce compte n'existe pas. ('enter' pour ré-essayer)");
                getchar();
            }
            else
            {
                void *c;
                if((c = le_cherche_restaurant(saisie)) != NULL)
                {
                    strcpy(nom_utilisateur, ((restaurant*)c)->nom);

                    ecran e = restaurateur_principal;
                    push_back(pile, &e);
                }
                else if((c = le_cherche_livreur(saisie)) != NULL)
                {
                    strcpy(nom_utilisateur, ((livreur*)c)->nom);

                    ecran e = livreur_principal;
                    push_back(pile, &e);
                }
                else
                {
                    c = le_cherche_client(saisie);

                    strcpy(nom_utilisateur, ((client*)c)->nom);

                    ecran e = client_principal;
                    push_back(pile, &e);
                }
            }
            break;
    }
}

void creation_compte(
    vector* pile)
{
    printf("\n\
* Création de votre compte *\n\
\n\
Vous êtes :\n\
1. Restaurateur·trice\n\
2. Livreur·se\n\
3. Client·e\n\n");

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'p' pour menu précédent) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = creation_compte_restaurateur;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = creation_compte_livreur;
                push_back(pile, &e);
            }
            break;
        case '3':
            {
                ecran e = creation_compte_client;
                push_back(pile, &e);
            }
            break;
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
    }
}
