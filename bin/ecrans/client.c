#include "ecrans/ecrans.h"

#include "prompt.h"

#include "db/types.h"
#include "lumineats/lumineats.h"

#include <stdio.h>
#include <string.h>

void creation_compte_client(
    vector* pile)
{
    printf("\n\
* Création d'un compte Client *\n\n");

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

    // Ajoute le client à la BdD.
    le_creer_compte_client(nom, code_postal, telephone);

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void client_principal(
    vector* pile)
{
    printf("\n\
* Menu Client * %s *\n\
\n\
1. Modifier votre profil\n\
2. Confirmer votre solde\n\
3. Ajoutez du crédit à votre solde\n\
4. Voir la liste des restaurants\n\
5. Passer une commande\n\
6. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = client_modifier_profil;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = client_voir_solde;
                push_back(pile, &e);
            }
            break;
        case '3':
            {
                ecran e = client_crediter_solde;
                push_back(pile, &e);
            }
            break;
        case '4':
            {
                ecran e = client_lister_restaurants;
                push_back(pile, &e);
            }
            break;
        case '5':
            {
                ecran e = client_commande;
                push_back(pile, &e);
            }
            break;
        case '6':
            le_supprimer_compte(nom_utilisateur);
            __attribute__((fallthrough));
        case 'd':
            nom_utilisateur[0] = '\n';

            // Revient au menu initial.
            {
                clear(pile);
                ecran e = initial;
                push_back(pile, &e);
            }
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void client_modifier_profil(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    char const* saisie = prompt_optional_string(TAILLE_CHAMP_CODEPOSTAL, "Saisissez votre nouveau code postal ('enter' pour conserver) [%s] : ", c->code_postal);
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    strcpy(code_postal, saisie);

    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre nouveau téléphone ('enter' pour conserver) [%s] : ", c->telephone);
        if(le_compte_existe(saisie) && strcmp(c->telephone, saisie) != 0)
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

    le_modifier_profil_client(c->index, code_postal, telephone);

    pop_back(pile);
}

void client_crediter_solde(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    char const* saisie = prompt_string(sizeof(size_t), "Saisissez le montant à ajouter ('0' pour ne rien ajouter) : ");
    le_crediter_solde_client(c->index, atoll(saisie));

    pop_back(pile);
}

void client_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    printf("Votre solde courant : €%d\n\n('enter' pour menu précédent)", c->solde);
    getchar();

    pop_back(pile);
}

void client_lister_restaurants(
    vector* pile)
{
    vector rs = le_liste_restaurants();
    vector restaurants = make_vector(sizeof(cle_t), 0);

    char type[TAILLE_CHAMP_TYPE] = {'\0'};
    char code_postal[TAILLE_CHAMP_CODEPOSTAL] = {'\0'};
    for(bool retour = false; !retour;)
    {
        system("clear");
        printf("\n* Menu Client * %s *\n", nom_utilisateur);

        printf("\nListe des restaurants filtrée par [%c] type (%s) [%c] qui peut me livrer : \n\n", (type[0] ? 'x' : ' '), (type[0] ? type : ""), (code_postal[0] ? 'x' : ' '));

        assign(&restaurants, begin(&rs), end(&rs));

        if(strlen(type))
        {
            le_filtrer_restaurants_type(&restaurants, type);
        }

        if(strlen(code_postal))
        {
            le_filtrer_restaurants_livraison(&restaurants, code_postal);
        }

        for(iterator i = begin(&restaurants), e = end(&restaurants); compare(i, e) != 0; increment(&i, 1))
        {
            printf("- %s\n", le_cherche_restaurant_i(*(cle_t*)value(i))->nom);
        }

        printf("\nFiltrer par :\n1. type de cuisine\n2. qui peut me livrer\n\n");
        char const c = prompt_choice("Votre choix ('q' pour quitter, 'p' pour précedent, 'N' pour alterner les filtres) : ");
        switch(c)
        {
            case '1':
                if(strlen(type))
                {
                    strcpy(type, "");
                }
                else
                {
                    strcpy(type, prompt_string(TAILLE_CHAMP_TYPE, "Type de cuisine : "));
                }
                break;
            case '2':
                if(strlen(code_postal))
                {
                    strcpy(code_postal, "");
                }
                else
                {
                    strcpy(code_postal, le_cherche_client(nom_utilisateur)->code_postal);
                }
                break;
            case 'p':
                pop_back(pile);
                retour = true;
                break;
            case 'q':
                clear(pile);
                retour = true;
                break;
        }
    }

    destroy(&restaurants);
}

typedef struct panier_t
{
    vector items;
    int total;
} panier_t;

void client_commande_ajouter_item(
    panier_t* panier)
{
    vector is = le_liste_items();
    vector items = make_vector(sizeof(cle_t), 0);

    char type[TAILLE_CHAMP_TYPE] = {'\0'};
    char code_postal[TAILLE_CHAMP_CODEPOSTAL] = {'\0'};
    char restaurant[TAILLE_CHAMP_NOM] = {'\0'};
    bool abordable = false;

    for(bool retour = false; !retour;)
    {
        system("clear");
        printf("\n* Menu Client * %s *\n", nom_utilisateur);

        printf("\nListe des items filtrée par [%c] qui peut me livrer [%c] type (%s) [%c] restaurant (%s) [%c] items abordables : \n\n", (code_postal[0] ? 'x' : ' '), (type[0] ? 'x' : ' '), (type[0] ? type : ""), (restaurant[0] ? 'x' : ' '), (restaurant[0] ? restaurant : ""), (abordable ? 'x' : ' '));

        assign(&items, begin(&is), end(&is));

        int n = 1;
        for(iterator i = begin(&items), e = end(&items); compare(i, e) != 0; increment(&i, 1))
        {
            printf("%d. %s\n", n, le_cherche_item_i(*(cle_t*)value(i))->nom);
            ++n;
        }

        char const choice = prompt_choice("\nVotre choix ('p' pour précédent) : ");

        if(choice == 'p')
        {
            retour = true;
        }
        else if(choice - '1' < (int)size(items))
        {
            cle_t ix = *(cle_t*)value(at(&items, choice - '1'));
            push_back(&(panier->items), &ix);
            panier->total += le_cherche_item_i(ix)->prix;

            retour = true;
        }
        else
        {
            printf("Désolé. Cet item ne figure pas sur la liste.\n");
            getchar();
        }
    }
}

void client_commande_enlever_item(
    panier_t* panier)
{
    for(bool retour = false; !retour;)
    {
        system("clear");

        printf("\n* Menu Client * %s *\n\n", nom_utilisateur);

        int n = 1;
        for(iterator i = begin(&panier->items), e = end(&panier->items); compare(i, e) != 0; increment(&i, 1))
        {
            printf("%d. %s\n", n, le_cherche_item_i(*(cle_t*)(value(i)))->nom);
            ++n;
        }

        char const choice = prompt_choice("Votre choix ('p' pour précédent) : ");

        if(choice == 'p')
        {
            retour = true;
        }
        else if(choice - '1' < (int)size(panier->items))
        {
            iterator i = at(&panier->items, choice - '1');
            cle_t const ix = *(cle_t*)value(i);
            erase(&panier->items, i);
            panier->total -= le_cherche_item_i(ix)->prix;

            retour = true;
        }
        else
        {
            printf("Désolé. Cet item ne figure pas sur la liste.\n");
            getchar();
        }
    }
}

void client_commande(
    vector* pile)
{
    panier_t panier;
    panier.items = make_vector(sizeof(cle_t), 0);
    panier.total = 0;

    client* const c = le_cherche_client(nom_utilisateur);

    for(bool retour = false; !retour;)
    {
        system("clear");
        printf("\n* Menu Client * %s *\n\n", nom_utilisateur);

        printf("Panier :\n");
        for(iterator i = begin(&panier.items), e = end(&panier.items); compare(i, e) != 0; increment(&i, 1))
        {
            printf("- %s\n", le_cherche_item_i(*(cle_t*)(value(i)))->nom);
        }
        printf("Total : %d\n\n", panier.total);

        char const choice = prompt_choice("Votre choix ('q' pour quitter, 'p' pour précedent, 'a' pour ajouter un item, 'e' pour enlever un item, 'c' pour passer la commande) : ");
        switch(choice)
        {
            case 'a':
                client_commande_ajouter_item(&panier);
                break;
            case 'e':
                client_commande_enlever_item(&panier);
                break;
            case 'c':
                {
                    vector is = make_vector(sizeof(cle_t), 0);
                    assign(&is, begin(&panier.items), end(&panier.items));
                    vector non_livrables = make_vector(sizeof(cle_t), 0);
                    vector depassent_solde = make_vector(sizeof(cle_t), 0);
                    if(!le_valider_commande(c->index, &is, &non_livrables, &depassent_solde))
                    {
                        printf("Votre commande est invalide! Veuillez la rectifier.\n");
                        if(size(non_livrables))
                        {
                            printf("Les items suivant ne pourront pas vous être livrés :\n");
                            for(iterator i = begin(&non_livrables), e = end(&non_livrables); compare(i, e) != 0; increment(&i, 1))
                            {
                                printf("- %s\n", le_cherche_item_i(*(cle_t*)(value(i)))->nom);
                            }
                        }

                        if(size(depassent_solde))
                        {
                            printf("Les items suivant dépassent votre solde disponible :\n");
                            for(iterator i = begin(&depassent_solde), e = end(&depassent_solde); compare(i, e) != 0; increment(&i, 1))
                            {
                                printf("- %s\n", le_cherche_item_i(*(cle_t*)(value(i)))->nom);
                            }
                        }
                    }
                    else
                    {
                        le_passer_commande(c->index, &panier.items);
                        printf("Votre commande est passée. Merci !\n");
                        pop_back(pile);
                        retour = true;
                    }
                    getchar();
                }
                break;
            case 'p':
                pop_back(pile);
                retour = true;
                break;
            case 'q':
                clear(pile);
                retour = true;
                break;
        }
    }

    destroy(&panier.items);
    panier.total = 0;
}
