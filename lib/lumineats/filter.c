#include "lumineats/filter.h"

#include "lumineats/predicates.h"

#include "db/db.h"
#include "db/tables.h"
#include "vector/vector.h"

vector le_liste_restaurants()
{
    vector restaurants = make_vector(sizeof(restaurant), 0, 2.0);

    assign(&restaurants, begin(&table_restaurants), end(&table_restaurants));

    return restaurants;
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
