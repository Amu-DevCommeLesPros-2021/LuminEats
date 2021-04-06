#include "lumineats/lumineats.h"

#include "db/db.h"
#include "vector/vector.h"

#include <string.h>

void le_creer_compte_restaurateur(
    char const* nom,
    char const* code_postal,
    char const* telephone,
    char const* type)
{
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
}

void le_creer_compte_livreur(
    char const* nom,
    char const* telephone)
{
    size_t const index = size(table_livreurs) ? ((livreur*)back(&table_livreurs))->index + 1 : 1;

    livreur l = {
        .index = index,
        .solde = 0
    };
    strcpy(l.nom, nom);
    strcpy(l.telephone, telephone);

    push_back(&table_livreurs, &l);
}

void le_creer_compte_client(
    char const* nom,
    char const* code_postal,
    char const* telephone)
{
    size_t const index = size(table_clients) ? ((client*)back(&table_clients))->index + 1 : 1;

    client c = {
        .index = index,
        .solde = 0
    };
    strcpy(c.nom, nom);
    strcpy(c.code_postal, code_postal);
    strcpy(c.telephone, telephone);

    push_back(&table_clients, &c);
}
