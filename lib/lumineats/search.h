#pragma once

#include "db/db.h"

#include <stdbool.h>

// Renvoie 'vrai' si un restaurant, livreur ou client a le nom ou le téléphone donné.
bool le_compte_existe(
    char const* string);

// Renvoie le restaurant avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
restaurant* le_cherche_restaurant(
    char const* string);

// Renvoie le restaurant avec l'index correspondant.
// Renvoie 'NULL' si non-trouvé.
restaurant* le_cherche_restaurant_i(
    cle_t const index);

// Renvoie le livreur avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
livreur* le_cherche_livreur(
    char const* string);

// Renvoie le livreur avec l'index correspondant.
// Renvoie 'NULL' si non-trouvé.
livreur* le_cherche_livreur_i(
    cle_t const index);

// Renvoie le client avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
client* le_cherche_client(
    char const* string);

// Renvoie le client avec l'index correspondant.
// Renvoie 'NULL' si non-trouvé.
client* le_cherche_client_i(
    cle_t const index);


// Renvoie le item avec le nom correspondant.
// Renvoie 'NULL' si non-trouvé.
item* le_cherche_item(
    char const* string);

// Renvoie le item avec l'index correspondant.
// Renvoie 'NULL' si non-trouvé.
item* le_cherche_item_i(
    cle_t const index);
