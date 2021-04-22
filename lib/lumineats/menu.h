#pragma once

#include "db/types.h"
#include "vector/vector.h"

// Renvoie tous les items.
vector const* le_liste_items();

// Crée un nouvel item et renvoie l'index de l'item nouvellement créé.
cle_t le_creer_item(
    char const* nom,
    char const* ingredients_s,
    size_t const prix);

// Ajoute un item au menu d'un restaurant s'il n'y est pas déjà.
void le_ajouter_item_menu(
    cle_t const index,
    cle_t const index_restaurant);

// Enlève l'item du menu du restaurant.
// Enlève l'item de la BdD si il n'est plus référencé par aucun restaurants.
void le_enlever_item_menu(
    cle_t const index,
    cle_t const index_restaurant);
