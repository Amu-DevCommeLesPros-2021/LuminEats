#include "lumineats/search.h"

#include "lumineats/predicates.h"

#include "algorithm/algorithm.h"
#include "db/tables.h"

bool le_compte_existe(
    char const* string)
{
    return le_cherche_restaurant(string) || le_cherche_livreur(string) || le_cherche_client(string);
}


restaurant* le_cherche_restaurant(
    char const* string)
{
    iterator i = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_a_nom_ou_telephone, string);
    return compare(i, end(&table_restaurants)) != 0 ? i.element : NULL;
}

restaurant* le_cherche_restaurant_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_a_index, &index);
    return compare(i, end(&table_restaurants)) != 0 ? i.element : NULL;
}


livreur* le_cherche_livreur(
    char const* string)
{
    iterator i = find_if_2(begin(&table_livreurs), end(&table_livreurs), livreur_a_nom_ou_telephone, string);
    return compare(i, end(&table_livreurs)) != 0 ? i.element : NULL;
}

livreur* le_cherche_livreur_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_livreurs), end(&table_livreurs), livreur_a_index, &index);
    return compare(i, end(&table_livreurs)) != 0 ? i.element : NULL;
}


client* le_cherche_client(
    char const* string)
{
    iterator i = find_if_2(begin(&table_clients), end(&table_clients), client_a_nom_ou_telephone, string);
    return compare(i, end(&table_clients)) != 0 ? i.element : NULL;
}

client* le_cherche_client_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_clients), end(&table_clients), client_a_index, &index);
    return compare(i, end(&table_clients)) != 0 ? i.element : NULL;
}


item* le_cherche_item(
    char const* string)
{
    iterator i = find_if_2(begin(&table_items), end(&table_items), item_est_nomme, string);
    return compare(i, end(&table_items)) != 0 ? i.element : NULL;
}

item* le_cherche_item_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_items), end(&table_items), item_a_index, &index);
    return compare(i, end(&table_items)) != 0 ? i.element : NULL;
}
