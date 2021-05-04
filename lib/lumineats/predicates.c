#include "lumineats/predicates.h"

#include "lumineats/search.h"

#include "algorithm/algorithm.h"
#include "db/db.h"
#include "db/tables.h"

#include <stdbool.h>
#include <string.h>

bool restaurant_a_index(
    void const* r,
    void const* index)
{
    return ((restaurant*)r)->index == *(size_t*)index;
}

bool restaurant_est_nomme(
    void const* ix,
    void const* nom)
{
    return strcmp(le_cherche_restaurant_i(*(cle_t const*)ix)->nom, nom) == 0;
}

bool restaurant_a_telephone(
    void const* ix,
    void const* telephone)
{
    return strcmp(le_cherche_restaurant_i(*(cle_t const*)ix)->telephone, telephone) == 0;
}

bool restaurant_a_nom_ou_telephone(
    void const* ix,
    void const* chaine)
{
    return restaurant_est_nomme(ix, chaine) || restaurant_a_telephone(ix, chaine);
}

bool restaurant_menu_contient(
    void const* ix,
    void const* index)
{
    cle_t *m = le_cherche_restaurant_i(*(cle_t const*)ix)->menu;

    for(int j = 0; j != TAILLE_MENU && m[j] != 0; ++j)
    {
        if(m[j] == *(cle_t*)index)
        {
            return true;
        }
    }

    return false;
}

bool restaurant_a_type(
    void const* ix,
    void const* type)
{
    return strcmp(le_cherche_restaurant_i(*(cle_t const*)ix)->type, type) == 0;
}

bool restaurant_peut_livrer(
    void const* ix,
    void const* code_postal)
{
    // Il suffit de trouver un livreur qui peut se déplacer chez le restaurant et le client.
    for(iterator i = begin(&table_livreurs), e = end(&table_livreurs); compare(i, e) != 0; increment(&i, 1))
    {
        livreur const* l = (livreur*)value(i);
        restaurant const* r = le_cherche_restaurant_i(*(cle_t const*)ix);

        if(l->restaurant == r->index)
        {
            // C'est un livreur qui nous est exclusif. Il suffit de trouver le code postal dans ses déplacements possible.
            for(int j = 0; j != TAILLE_DEPLACEMENTS; ++j)
            {
                if(strcmp(l->deplacements[j], (char*)code_postal) == 0)
                {
                    return true;
                }
            }
        }
        else if(l->restaurant == 0)
        {
            // C'est un livreur freelance. Il faut trouver dans ses déplacements le code postal du restaurant et celui donné.
            bool deplace_restaurant = false;
            for(int j = 0; j != TAILLE_DEPLACEMENTS && !deplace_restaurant; ++j)
            {
                if(strcmp(l->deplacements[j], r->code_postal) == 0)
                {
                    deplace_restaurant = true;
                }
            }

            bool deplace_client = false;
            for(int j = 0; j != TAILLE_DEPLACEMENTS && !deplace_client; ++j)
            {
                if(strcmp(l->deplacements[j], (char*)code_postal) == 0)
                {
                    deplace_client = true;
                }
            }

            if(deplace_restaurant && deplace_client)
            {
                return true;
            }
        }
    }

    return false;
}

bool livreur_a_index(
    void const* l,
    void const* index)
{
    return ((livreur*)l)->index == *(size_t*)index;
}

bool livreur_est_nomme(
    void const* ix,
    void const* nom)
{
    return strcmp(le_cherche_livreur_i(*(cle_t const*)ix)->nom, nom) == 0;
}

bool livreur_a_telephone(
    void const* ix,
    void const* telephone)
{
    return strcmp(le_cherche_livreur_i(*(cle_t const*)ix)->telephone, telephone) == 0;
}

bool livreur_a_nom_ou_telephone(
    void const* ix,
    void const* chaine)
{
    return livreur_est_nomme(ix, chaine) || livreur_a_telephone(ix, chaine);
}

bool client_a_index(
    void const* c,
    void const* index)
{
    return ((client*)c)->index == *(size_t*)index;
}

bool client_est_nomme(
    void const* ix,
    void const* nom)
{
    return strcmp(le_cherche_client_i(*(cle_t const*)ix)->nom, nom) == 0;
}

bool client_a_telephone(
    void const* ix,
    void const* telephone)
{
    return strcmp(le_cherche_client_i(*(cle_t const*)ix)->telephone, telephone) == 0;
}

bool client_a_nom_ou_telephone(
    void const* ix,
    void const* chaine)
{
    return client_est_nomme(ix, chaine) || client_a_telephone(ix, chaine);
}

bool item_a_index(
    void const* i,
    void const* index)
{
    return ((item*)i)->index == *(size_t*)index;
}

bool item_est_nomme(
    void const* ix,
    void const* nom)
{
    return strcmp(le_cherche_item_i(*(cle_t const*)ix)->nom, nom) == 0;
}

bool item_a_type(
    void const* ix,
    void const* type) 
{
    for(iterator j = begin(&table_restaurants), e = end(&table_restaurants); compare(j, e) != 0; increment(&j, 1))
    {
        restaurant const* r = (restaurant const*)value(j);
        if(strcmp(r->type, type) == 0)
        {
            for(int k = 0; k != TAILLE_MENU && r->menu[k] != 0; ++k)
            {
                if(r->menu[k] == *(cle_t const*)ix)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool item_menu_restaurant(
    void const* ix,
    void const* nom)
{
    iterator j = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_est_nomme, nom);
    restaurant const* r = (restaurant const*)value(j);

    for(int k = 0; k != TAILLE_MENU && r->menu[k] != 0; ++k)
    {
        if(r->menu[k] == *(cle_t const*)ix)
        {
            return true;
        }
    }

    return false;
}

bool item_prix_moindre(
    void const* ix,
    void const* prix)
{
    return le_cherche_item_i(*(cle_t const*)ix)->prix <= *(int*)prix;
}

bool item_offert_restaurants(
    void const *ix,
    void const *restaurants)
{
    vector const* v = (vector const*)restaurants;

    for(iterator j = begin(v), e = end(v); compare(j, e) != 0; increment(&j, 1))
    {
        restaurant const* r = le_cherche_restaurant_i(*(cle_t*)value(j));
        for(int k = 0; k != TAILLE_MENU && r->menu[k] != 0; ++k)
        {
            if(r->menu[k] == *(cle_t const*)ix)
            {
                return true;
            }
        }
    }

    return false;
}
