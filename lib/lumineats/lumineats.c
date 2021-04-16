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
    erase_if(restaurants, restaurant_a_type, type);
}

void le_filtrer_restaurants_livraison(
    vector *restaurants,
    char const* code_postal)
{
    erase_if(restaurants, restaurant_peut_livrer, code_postal);
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

restaurant* le_cherche_restaurant_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_restaurants), end(&table_restaurants), restaurant_a_index, &index);
    return compare(i, end(&table_restaurants)) != 0 ? i.element : NULL;
}

livreur* le_cherche_livreur_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_livreurs), end(&table_livreurs), livreur_a_index, &index);
    return compare(i, end(&table_livreurs)) != 0 ? i.element : NULL;
}

client* le_cherche_client_i(
    cle_t const index)
{
    iterator i = find_if_2(begin(&table_clients), end(&table_clients), client_a_index, &index);
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

void analyse_deplacements(
    char deplacements[TAILLE_DEPLACEMENTS][TAILLE_CHAMP_CODEPOSTAL],
    char const* deplacements_s
    )
{
    memset(deplacements, '\0', TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL);

    // Make a writable copy.
    char d[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    strcpy(d, deplacements_s);

    char *deplacement = strtok(d, ";");
    for(int i = 0; i != TAILLE_DEPLACEMENTS && deplacement; ++i)
    {
        strcpy(deplacements[i], deplacement);
        deplacement = strtok(NULL, ";");
    }
}

livreur* le_creer_compte_livreur(
    char const* nom,
    char const* telephone,
    char const* deplacements,
    cle_t const index_restaurant)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return NULL;
    }

    if(index_restaurant != 0 && le_cherche_restaurant_i(index_restaurant) == NULL)
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
    strcpy(l.deplacements_s, deplacements);
    analyse_deplacements(l.deplacements, deplacements);
    l.restaurant = index_restaurant;

    push_back(&table_livreurs, &l);

    llog("Compte 'livreur' [%s,%s,%s,%zu] créé.", nom, telephone, deplacements, index_restaurant);

    return back(&table_livreurs);
}

bool le_modifier_profil_livreur(
    cle_t const index,
    char const* deplacements,
    char const* telephone,
    cle_t const index_restaurant)
{
    livreur* l = le_cherche_livreur_i(index);
    if(!l)
    {
        return false;
    }

    if(le_compte_existe(telephone) && strcmp(l->telephone, telephone) != 0)
    {
        return false;
    }

    if(index_restaurant != 0 && le_cherche_restaurant_i(index_restaurant) == NULL)
    {
        return false;
    }

    strcpy(l->telephone, telephone);
    strcpy(l->deplacements_s, deplacements);
    analyse_deplacements(l->deplacements, deplacements);
    l->restaurant = index_restaurant;

    llog("Compte 'livreur' [%s] modifié : [%s,%s,%zu].", l->nom, deplacements, telephone, index_restaurant);

    return true;
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

bool le_modifier_profil_client(
    cle_t const index,
    char const* code_postal,
    char const* telephone)
{
    client* c = le_cherche_client_i(index);
    if(!c)
    {
        return false;
    }

    if(le_compte_existe(telephone) && strcmp(c->telephone, telephone) != 0)
    {
        return false;
    }

    strcpy(c->code_postal, code_postal);
    strcpy(c->telephone, telephone);

    llog("Compte 'client' [%s] modifié : [%s,%s,%zu].", c->nom, code_postal, telephone);

    return true;
}

void le_crediter_solde_client(
    cle_t const index,
    size_t montant)
{
    client* c = le_cherche_client_i(index);
    if(c)
    {
        c->solde += montant;
    }

    llog("Compte 'client' [%s] nouveau solde : [%zu].", c->nom, c->solde);
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
