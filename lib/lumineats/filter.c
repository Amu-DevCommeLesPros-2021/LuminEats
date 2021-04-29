#include "lumineats/filter.h"

#include "lumineats/predicates.h"

#include "db/db.h"
#include "db/tables.h"
#include "vector/vector.h"

vector const* le_liste_restaurants()
{
    return &table_restaurants;
}

void le_filtrer_restaurants_type(
    vector *restaurants,
    char const* type)
{
    keep_if(restaurants, restaurant_a_type, type);
}

void le_filtrer_restaurants_livraison(
    vector *restaurants,
    char const* code_postal)
{
    keep_if(restaurants, restaurant_peut_livrer, code_postal);
}

void le_filtrer_restaurants_item(
    vector *restaurants,
    cle_t const index_item)
{
    keep_if(restaurants, restaurant_menu_contient, &index_item);
}


void le_filtrer_items_type(
    vector *items,
    char const* type)
{
    keep_if(items, item_a_type, type);
}

void le_filtrer_items_livraison(
    vector *items,
    char const* code_postal)
{
    (void)items;
    (void)code_postal;
}

void le_filtrer_items_restaurant(
    vector *items,
    char const* restaurant)
{
    keep_if(items, item_menu_restaurant, restaurant);
}

void le_filtrer_items_prix(
    vector *items,
    size_t const prix)
{
    keep_if(items, item_prix_moindre, &prix);
}
