#include "lumineats/accounts.h"

#include "lumineats/predicates.h"
#include "lumineats/search.h"

#include "algorithm/algorithm.h"
#include "db/tables.h"
#include "logger/logger.h"
#include "vector/vector.h"

#include <string.h>

cle_t le_creer_compte_restaurateur(
    char const* nom,
    char const* code_postal,
    char const* telephone,
    char const* type)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return 0;
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
    memset(r.menu_s, '\0', TAILLE_MENU * 3);
    memset(r.menu, 0, sizeof(int) * TAILLE_MENU);

    push_back(&table_restaurants, &r);

    llog("Compte 'restaurateur' [%zu,%s,%s] créé.", index, nom, telephone);

    return index;
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

cle_t le_creer_compte_livreur(
    char const* nom,
    char const* telephone,
    char const* deplacements_s,
    cle_t const index_restaurant)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return 0;
    }

    if(index_restaurant != 0 && le_cherche_restaurant_i(index_restaurant) == NULL)
    {
        return 0;
    }

    size_t const index = size(table_livreurs) ? ((livreur*)back(&table_livreurs))->index + 1 : 1;

    livreur l = {
        .index = index,
        .solde = 0
    };
    strcpy(l.nom, nom);
    strcpy(l.telephone, telephone);
    strcpy(l.deplacements_s, deplacements_s);
    analyse_deplacements(l.deplacements, deplacements_s);
    l.restaurant = index_restaurant;

    push_back(&table_livreurs, &l);

    llog("Compte 'livreur' [%zu,%s,%s,%s,%zu] créé.", index, nom, telephone, deplacements_s, index_restaurant);

    return index;
}

bool le_modifier_profil_livreur(
    cle_t const index,
    char const* deplacements_s,
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
    strcpy(l->deplacements_s, deplacements_s);
    analyse_deplacements(l->deplacements, deplacements_s);
    l->restaurant = index_restaurant;

    llog("Compte 'livreur' [%s] modifié : [%s,%s,%zu].", l->nom, deplacements_s, telephone, index_restaurant);

    return true;
}

cle_t le_creer_compte_client(
    char const* nom,
    char const* code_postal,
    char const* telephone)
{
    if(le_compte_existe(nom) || le_compte_existe(telephone))
    {
        return 0;
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

    llog("Compte 'client' [%zu,%s,%s] créé.", index, nom, telephone);

    return index;
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
