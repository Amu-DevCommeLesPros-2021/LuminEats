#include "lumineats/predicates.h"

#include "db/db.h"

#include <stdbool.h>
#include <string.h>

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
