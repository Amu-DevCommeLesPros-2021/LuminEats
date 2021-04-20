#pragma once

#include "vector/vector_types.h"

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
