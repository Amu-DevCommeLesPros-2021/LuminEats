#include "lumineats/predicates.h"

#include "db/db.h"
#include "db/tables.h"

#include <stdbool.h>
#include <string.h>

bool restaurant_a_index(
    void const* c,
    void const* index)
{
    return ((restaurant*)c)->index == *(size_t*)index;
}

bool restaurant_est_nomme(
    void const* r,
    void const* nom)
{
    return strcmp(((restaurant*)r)->nom, nom) == 0;
}

bool restaurant_a_telephone(
    void const* r,
    void const* telephone)
{
    return strcmp(((restaurant*)r)->telephone, telephone) == 0;
}

bool restaurant_a_nom_ou_telephone(
    void const* r,
    void const* chaine)
{
    return restaurant_est_nomme(r, chaine) || restaurant_a_telephone(r, chaine);
}

bool restaurant_a_type(
    void const* r,
    void const* type)
{
    return strcmp(((restaurant*)r)->type, type) == 0;
}

bool restaurant_peut_livrer(
    void const* r,
    void const* code_postal)
{
    // Il suffit de trouver un livreur qui peut se déplacer chez le restaurant et le client.
    for(iterator i = begin(&table_livreurs), e = end(&table_livreurs); compare(i, e) != 0; increment(&i, 1))
    {
        livreur const* l = (livreur*)value(i);

        if(l->restaurant == ((restaurant*)r)->index)
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
                if(strcmp(l->deplacements[j], ((restaurant*)r)->code_postal) == 0)
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
    void const* c,
    void const* index)
{
    return ((livreur*)c)->index == *(size_t*)index;
}

bool livreur_est_nomme(
    void const* l,
    void const* nom)
{
    return strcmp(((livreur*)l)->nom, nom) == 0;
}

bool livreur_a_telephone(
    void const* l,
    void const* telephone)
{
    return strcmp(((livreur*)l)->telephone, telephone) == 0;
}

bool livreur_a_nom_ou_telephone(
    void const* l,
    void const* chaine)
{
    return livreur_est_nomme(l, chaine) || livreur_a_telephone(l, chaine);
}

bool client_a_index(
    void const* c,
    void const* index)
{
    return ((client*)c)->index == *(size_t*)index;
}

bool client_est_nomme(
    void const* c,
    void const* nom)
{
    return strcmp(((client*)c)->nom, nom) == 0;
}

bool client_a_telephone(
    void const* c,
    void const* telephone)
{
    return strcmp(((client*)c)->telephone, telephone) == 0;
}

bool client_a_nom_ou_telephone(
    void const* c,
    void const* chaine)
{
    return client_est_nomme(c, chaine) || client_a_telephone(c, chaine);
}
