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
    memset(r.items_s, '\0', TAILLE_ITEMS * 2 + TAILLE_ITEMS);
    memset(r.items, 0, TAILLE_ITEMS);

    push_back(&table_restaurants, &r);
}