#include "ecrans/ecrans.h"

#include "prompt.h"

#include "lumineats/lumineats.h"

#include <stdio.h>
#include <string.h>

void creation_compte_restaurateur(
    vector* pile)
{
    printf("\n\
* Création d'un compte Restaurateur *\n\n");

    char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez le nom de votre restaurant ('q' pour quitter, 'p' pour menu précédent) : ");
    char nom[TAILLE_CHAMP_NOM];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            return;
            break;
        case 'q':
            clear(pile);
            return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte de ce nom existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(nom, TAILLE_CHAMP_NOM, saisie);
            }
            break;
    }

    saisie = prompt_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
    char telephone[TAILLE_CHAMP_TELEPHONE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte avec ce numero existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(telephone, TAILLE_CHAMP_TELEPHONE, saisie);
            }
            break;
    }

    saisie = prompt_string(TAILLE_CHAMP_CODEPOSTAL, "Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ");
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            strcpy_s(code_postal, TAILLE_CHAMP_CODEPOSTAL, saisie);
            break;
    }

    saisie = prompt_string(TAILLE_CHAMP_TYPE, "Saisissez le type de cuisine ('q' pour quitter, 'p' pour menu précédent) : ");
    char type[TAILLE_CHAMP_TYPE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            strcpy_s(type, TAILLE_CHAMP_TYPE, saisie);
            break;
    }

    // Ajoute le restaurant à la BdD.
    le_creer_compte_restaurateur(nom, code_postal, telephone, type);

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void restaurateur_principal(
    vector* pile)
{
    printf("\n\
* Menu Restaurateur * %s *\n\
\n\
1. Modifier votre menu\n\
2. Confirmer votre solde\n\
3. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            break;
        case '2':
            {
                ecran e = restaurateur_voir_solde;
                push_back(pile, &e);
            }
            break;
        case '3':
            le_supprimer_compte(nom_utilisateur);
            __attribute__((fallthrough));
        case 'd':
            nom_utilisateur[0] = '\n';

            // Revient au menu initial.
            clear(pile);
            ecran e = initial;
            push_back(pile, &e);
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void restaurateur_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Restaurateur * %s *\n\
\n", nom_utilisateur);

    restaurant const* const r = le_cherche_restaurant(nom_utilisateur);

    printf("Votre solde courant : €%zu\n\n", r->solde);
    getchar();

    pop_back(pile);
}
