#pragma once

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
    void const* r,
    void const* nom)
{
    return strcmp(((livreur*)r)->nom, nom) == 0;
}

bool livreur_a_telephone(
    void const* r,
    void const* telephone)
{
    return strcmp(((livreur*)r)->telephone, telephone) == 0;
}

bool livreur_a_nom_ou_telephone(
    void const* r,
    void const* chaine)
{
    return livreur_est_nomme(r, chaine) || livreur_a_telephone(r, chaine);
}

bool client_est_nomme(
    void const* r,
    void const* nom)
{
    return strcmp(((client*)r)->nom, nom) == 0;
}

bool client_a_telephone(
    void const* r,
    void const* telephone)
{
    return strcmp(((client*)r)->telephone, telephone) == 0;
}

bool client_a_nom_ou_telephone(
    void const* r,
    void const* chaine)
{
    return client_est_nomme(r, chaine) || client_a_telephone(r, chaine);
}
