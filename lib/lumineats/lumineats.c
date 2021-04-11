#include "lumineats/lumineats.h"

#include "lumineats/predicates.h"

#include "algorithm/algorithm.h"
#include "db/db.h"
#include "db/tables.h"
#include "logger/logger.h"
#include "vector/vector.h"

#include <stdbool.h>
#include <string.h>

void le_dump_tables(
    FILE* f)
{
    ecriture_table_restaurants(f, &table_restaurants);
    ecriture_table_items(f, &table_items);
    ecriture_table_livreurs(f, &table_livreurs);
    ecriture_table_clients(f, &table_clients);
}

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

    llog("Compte 'restaurateur' [%s,%s] créé.", nom, telephone);

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

    llog("Compte 'livreur' [%s,%s] créé.", nom, telephone);

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

    llog("Compte 'client' [%s,%s] créé.", nom, telephone);

    return back(&table_clients);
}

void le_supprimer_compte(
    char const* string)
{
    iterator i = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_a_nom_ou_telephone, string);
    if(compare(i, end(&table_restaurants)) != 0)
    {
        erase(&table_restaurants, i);

        llog("Compte 'restaurateur' [%s] supprimé.", string);
    }

    i = find_if_2(begin(&table_livreurs), end(&table_livreurs), livreur_a_nom_ou_telephone, string);
    if(compare(i, end(&table_livreurs)) != 0)
    {
        erase(&table_livreurs, i);

        llog("Compte 'livreur' [%s] supprimé.", string);
    }

    i = find_if_2(begin(&table_clients), end(&table_clients), client_a_nom_ou_telephone, string);
    if(compare(i, end(&table_clients)) != 0)
    {
        erase(&table_clients, i);

        llog("Compte 'client' [%s] supprimé.", string);
    }
}
