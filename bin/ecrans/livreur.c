#include "ecrans/ecrans.h"

#include "prompt.h"

#include "lumineats/lumineats.h"

#include <stdio.h>
#include <string.h>

void creation_compte_livreur(
    vector* pile)
{
    printf("\n\
* Création d'un compte Livreur *\n\n");

   char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_optional_string(TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL, "Saisissez vos code postaux de déplacement ('enter' pour garder vide) : ", "");
    char deplacements[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    strcpy(deplacements, saisie);

    size_t index_restaurant;
    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_NOM, "Saisissez votre restaurant ('enter' pour garder vide) : ");
        if(strlen(saisie) == 0 || strcmp(saisie, "0") == 0)
        {
            index_restaurant = 0;
            saisie_valide = true;
        }
        else
        {
            restaurant *r = le_cherche_restaurant(saisie);
            if(!r)
            {
                printf("Erreur : ce restaurant n'existe pas.\n");
            }
            else
            {
                index_restaurant = r->index;
                saisie_valide = true;
            }
        }
    }


    // Ajoute le livreur à la BdD.
    le_creer_compte_livreur(nom, telephone, deplacements, index_restaurant);

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void livreur_principal(
    vector* pile)
{
    printf("\n\
* Menu Livreur * %s *\n\
\n\
1. Modifier votre profil\n\
2. Confirmer votre solde\n\
3. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = livreur_modifier_profil;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = livreur_voir_solde;
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

void livreur_modifier_profil(
    vector* pile)
{
    printf("\n\
* Menu Livreur * %s *\n\
\n", nom_utilisateur);

    livreur const* const l = le_cherche_livreur(nom_utilisateur);

    char const* saisie = prompt_optional_string(TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL, "Saisissez vos nouveaux code postaux de déplacement ('enter' pour conserver) [%s] : ", l->deplacements_s);
    char deplacements[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    strcpy(deplacements, saisie);

    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre nouveau téléphone ('enter' pour conserver) [%s] : ", l->telephone);
        if(le_compte_existe(saisie) && strcmp(l->telephone, saisie) != 0)
        {
            printf("Erreur : ce téléphone existe déjà.\n");
        }
        else
        {
            saisie_valide = true;
        }
    }
    char telephone[TAILLE_CHAMP_TELEPHONE];
    strcpy(telephone, saisie);


    cle_t index_restaurant = l->restaurant;
    char nom_restaurant[TAILLE_CHAMP_NOM] = {'\0'};
    if(index_restaurant != 0)
    {
        restaurant *r = le_cherche_restaurant_i(index_restaurant);
        strcpy(nom_restaurant, r->nom);
    }
    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_NOM, "Saisissez votre restaurant ('enter' pour conserver, '0' pour effacer) [%s] : ", nom_restaurant);
        if(strlen(saisie) == 0 || strcmp(saisie, "0") == 0)
        {
            index_restaurant = 0;
            saisie_valide = true;
        }
        else
        {
            restaurant *r = le_cherche_restaurant(saisie);
            if(!r)
            {
                printf("Erreur : ce restaurant n'existe pas.\n");
            }
            else
            {
                index_restaurant = r->index;
                saisie_valide = true;
            }
        }
    }

    le_modifier_profil_livreur(l->index, deplacements, telephone, index_restaurant);

    pop_back(pile);
}

void livreur_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Livreur * %s *\n\
\n", nom_utilisateur);

    livreur const* const l = le_cherche_livreur(nom_utilisateur);

    printf("Votre solde courant : €%zu\n\n", l->solde);
    getchar();

    pop_back(pile);
}
