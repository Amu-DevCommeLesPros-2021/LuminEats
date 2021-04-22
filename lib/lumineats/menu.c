#include "lumineats/menu.h"

#include "lumineats/filter.h"
#include "lumineats/predicates.h"
#include "lumineats/search.h"

#include "algorithm/algorithm.h"
#include "db/tables.h"
#include "db/types.h"
#include "logger/logger.h"
#include "vector/vector.h"

#include <string.h>

vector le_liste_items()
{
    vector items = make_vector(sizeof(item), 0, 2.0);

    assign(&items, begin(&table_items), end(&table_items));

    return items;
}


void analyse_ingredients(
    char ingredients[TAILLE_INGREDIENTS][TAILLE_CHAMP_INGREDIENT],
    char const* ingredients_s
    )
{
    memset(ingredients, '\0', TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT);

    // Make a writable copy.
    char i[TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT];
    strcpy(i, ingredients_s);

    char *ingredient = strtok(i, ";");
    for(int j = 0; ingredient && j != TAILLE_INGREDIENTS; ++j)
    {
        strcpy(ingredients[j], ingredient);
        ingredient = strtok(NULL, ";");
    }
}

item* le_creer_item(
    char const* nom,
    char const* ingredients_s,
    size_t const prix)
{
    size_t const index = size(table_items) ? ((item*)back(&table_items))->index + 1 : 1;

    item i = {
        .index = index,
        .prix = prix
    };
    strcpy(i.nom, nom);
    strcpy(i.ingredients_s, ingredients_s);
    analyse_ingredients(i.ingredients, ingredients_s);

    push_back(&table_items, &i);

    llog("Nouvel 'item' [%s,%s,%s,%zu] créé.", nom, ingredients_s, prix);

    return back(&table_items);
}

void le_enlever_item_menu(
    cle_t const index,
    cle_t const index_restaurant)
{
    // Remove the item from the restaurant's menu.
    restaurant *r = le_cherche_restaurant_i(index_restaurant);

    int i;
    for(i = 0; i != TAILLE_MENU && r->menu[i] != index; ++i)
    {}

    for(++i; i <= TAILLE_MENU; ++i)
    {
        r->menu[i - 1] = r->menu[i];
    }

    memset(r->menu_s, '\0', TAILLE_MENU * 3);
    char *p = r->menu_s;
    for(int i = 0; i != TAILLE_MENU && r->menu[i] != 0; ++i)
    {
        sprintf(p, "%zu", r->menu[i]);
        if(i != TAILLE_MENU && r->menu[i + 1] != 0)
        {
            sprintf(p, ";");
        }
    }

    llog("Item [%zu] supprimé du menu de [%s].\n", index, r->nom);


    // Remove the item from the DB if it is no longer referenced by any restaurants.
    vector restaurants = le_liste_restaurants();
    le_filtrer_restaurants_item(&restaurants, index);

    if(size(restaurants) == 0)
    {
        iterator i = find_if_2(begin(&table_items), end(&table_items), item_a_index, &index);
        erase(&table_items, i);
    }

    llog("Item [%zu] supprimé.\n", index);
}
