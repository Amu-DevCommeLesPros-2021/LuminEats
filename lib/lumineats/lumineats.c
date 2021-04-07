#include "lumineats/lumineats.h"

#include "lumineats/predicates.h"

#include "algorithm/algorithm.h"
#include "db/db.h"
#include "vector/vector.h"

#include <stdbool.h>
#include <string.h>

restaurant* le_cherche_restaurant(
    char const* string)
{
    iterator i = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_a_nom_ou_telephone, string);
    return compare(i, end(&table_restaurants)) != 0 ? i.element : NULL;
}

livreur* le_cherche_livreur(
    char const* string)
{
    iterator i = find_if_2(begin(&table_livreurs), end(&table_livreurs), livreur_a_nom_ou_telephone, string);
    return compare(i, end(&table_livreurs)) != 0 ? i.element : NULL;
}

client* le_cherche_client(
    char const* string)
{
    iterator i = find_if_2(begin(&table_clients), end(&table_clients), client_a_nom_ou_telephone, string);
    return compare(i, end(&table_clients)) != 0 ? i.element : NULL;
}

bool le_compte_existe(
    char const* string)
{
    return le_cherche_restaurant(string) || le_cherche_livreur(string) || le_cherche_client(string);
}

restaurant* le_creer_compte_restaurateur(
    char const* nom,
    char const* code_postal,
    char const* telephone,
    char const* type)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return NULL;
    }

    size_t const index = size(table_restaurants) ? ((restaurant*)back(&table_restaurants))->index + 1 : 1;

    restaurant r = {
        .index = index,
        .solde = 0
    };
    strcpy(r.nom, nom);
    strcpy(r.code_postal, code_postal);
    strcpy(r.telephone, telephone);
    strcpy(r.type, type);
    memset(r.items_s, '\0', TAILLE_ITEMS * 3);
    memset(r.items, 0, sizeof(int) * TAILLE_ITEMS);

    push_back(&table_restaurants, &r);

    return back(&table_restaurants);
}

livreur* le_creer_compte_livreur(
    char const* nom,
    char const* telephone)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return NULL;
    }

    size_t const index = size(table_livreurs) ? ((livreur*)back(&table_livreurs))->index + 1 : 1;

    livreur l = {
        .index = index,
        .solde = 0
    };
    strcpy(l.nom, nom);
    strcpy(l.telephone, telephone);

    push_back(&table_livreurs, &l);

    return back(&table_livreurs);
}

client* le_creer_compte_client(
    char const* nom,
    char const* code_postal,
    char const* telephone)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return NULL;
    }

    size_t const index = size(table_clients) ? ((client*)back(&table_clients))->index + 1 : 1;

    client c = {
        .index = index,
        .solde = 0
    };
    strcpy(c.nom, nom);
    strcpy(c.code_postal, code_postal);
    strcpy(c.telephone, telephone);

    push_back(&table_clients, &c);

    return back(&table_clients);
}
