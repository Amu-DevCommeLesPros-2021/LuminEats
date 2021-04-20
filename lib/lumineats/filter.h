#pragma once

#include "db/types.h"
#include "vector/types.h"

// Renvoie tous les restaurants.
vector le_liste_restaurants();

// Filtre une liste de restaurants par type.
void le_filtrer_restaurants_type(
    vector *restaurants,
    char const* type);

// Filtre une liste de restaurants par livraison possible au code postal.
void le_filtrer_restaurants_livraison(
    vector *restaurants,
    char const* code_postal);

// Filtre une liste de restaurants par ceux qui ont 'index_item' au menu.
void le_filtrer_restaurants_item(
    vector *restaurants,
    cle_t const index_item);
