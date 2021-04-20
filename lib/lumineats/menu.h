#pragma once

#include "db/types.h"
#include "vector/vector.h"

// Renvoie tous les items.
vector le_liste_items();

// Crée un nouvel item et renvoie un pointeur à l'item nouvellement créé.
item* le_creer_item(
    char const* nom,
    char const* ingredients_s,
    size_t const prix);

// Enlève l'item du menu du restaurant.
// Enlève l'item de la BdD si il n'est plus référencé par aucun restaurants.
void le_enlever_item_menu(
    cle_t const index,
    cle_t const index_restaurant);
