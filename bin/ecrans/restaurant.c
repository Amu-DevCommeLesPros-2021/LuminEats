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
            {
                ecran e = restaurateur_modification_menu;
                push_back(pile, &e);
            }
            break;
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

void restaurateur_modification_menu(
    vector* pile)
{
    printf("\n\
* Menu Restaurateur * %s *\n\
\n\
1. Voir votre menu\n\
2. Créer un nouvel item\n\
3. Ajouter un item\n\
4. Supprimer un item\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'p' pour menu précédent) : ");
    switch(choice)
    {
        case '1':
            {
                cle_t const* menu = le_cherche_restaurant(nom_utilisateur)->menu;
                for(size_t c = 0; c != TAILLE_MENU && menu[c] != 0; ++c)
                {
                    item const* const i = le_cherche_item_i(menu[c]);
                    printf("\n- %s (%s) %d", i->nom, i->ingredients_s, i->prix);
                }
                getchar();
            }
            break;
        case '2':
            {
                char nom[TAILLE_CHAMP_NOM] = {'\0'};
                char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Nom de cet item : ");
                strcpy(nom, saisie);

                char ingredients_s[TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT] = {'\0'};
                saisie = prompt_string(TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT, "Ingrédients (séparés par ';') : " );
                strcpy(ingredients_s, saisie);

                char prix[5] = {'\0'};
                saisie = prompt_string(sizeof(prix), "Prix : " );
                strcpy(prix, saisie);

                le_creer_item(nom, ingredients_s, atoll(prix));
            }
            break;
        case '3':
            {
                cle_t const* menu = le_cherche_restaurant(nom_utilisateur)->menu;
                
                vector items = le_liste_items();
                for(iterator i = begin(&items), e = end(&items); compare(i, e) != 0; increment(&i, 1))
                {
                    item const* const p = le_cherche_item_i(*(cle_t*)value(i));

                    // Cherche si l'item est déjà présent dans le menu.
                    bool present = false;
                    for(int j = 0; j != TAILLE_MENU && menu[j] != 0 && !present; ++j)
                    {
                        if(menu[j] == p->index)
                        {
                            present = true;
                        }
                    }

                    if(!present)
                    {
                        printf("\n[%zu] %s (%s)", p->index, p->nom, p->ingredients_s);
                    }
                }

                char const choice = prompt_choice("\n\nVotre choix ('0' pour ne rien ajouter) : ");
                cle_t const c = choice - '0';
                if(c != 0)
                {
                    le_ajouter_item_menu(c, le_cherche_restaurant(nom_utilisateur)->index);
                }
            }
            break;
        case '4':
            {
                cle_t const* menu = le_cherche_restaurant(nom_utilisateur)->menu;
                for(size_t c = 0; c != TAILLE_MENU && menu[c] != 0; ++c)
                {
                    item const* const i = le_cherche_item_i(menu[c]);
                    printf("\n[%zu] %s (%s)", i->index, i->nom, i->ingredients_s);
                }

                char const choice = prompt_choice("\n\nVotre choix ('0' pour ne rien supprimer) : ");
                cle_t const c = choice - '0';
                if(c != 0)
                {
                    le_enlever_item_menu(c, le_cherche_restaurant(nom_utilisateur)->index);
                }
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

void restaurateur_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Restaurateur * %s *\n\
\n", nom_utilisateur);

    restaurant const* const r = le_cherche_restaurant(nom_utilisateur);

    printf("Votre solde courant : €%d\n\n", r->solde);
    getchar();

    pop_back(pile);
}
