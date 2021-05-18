#include "lumineats/commande.h"

#include "lumineats/filter.h"
#include "lumineats/predicates.h"
#include "lumineats/search.h"

#include "algorithm/algorithm.h"
#include "db/types.h"
#include "logger/logger.h"
#include "vector/api.h"
#include "vector/types.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

bool le_valider_commande(
    cle_t const ix,
    vector* items,
    vector* non_livrables,
    vector* depassent_solde)
{
    size_t const nombre_items = size(*items);

    char const* const code_postal = le_cherche_client_i(ix)->code_postal;

    vector restaurants = le_liste_restaurants();

    le_filtrer_restaurants_livraison(&restaurants, code_postal);

    // If an item is not deliverable, remove it from 'items' and add it to 'non_livrables'.
    for(iterator i = begin(items); compare(i, end(items)) != 0;)
    {
        if(item_offert_restaurants(value(i), &restaurants))
        {
            increment(&i, 1);
        }
        else
        {
            if(non_livrables)
            {
                push_back(non_livrables, value(i));
            }
            erase(items, i);
        }
    }

    destroy(&restaurants);

    // Now, remove all items that would overshoot the customer's available funds.

    int available = le_cherche_client_i(ix)->solde;
    iterator i = begin(items);
    while(compare(i, end(items)) != 0 && available > 0)
    {
        item const* const it = le_cherche_item_i(*(cle_t*)value(i));
        if(available >= it->prix)
        {
            increment(&i, 1);
        }
        available -= it->prix;
    }

    while(compare(i, end(items)) != 0)
    {
        if(depassent_solde)
        {
            push_back(depassent_solde, value(i));
        }
        erase(items, i);
    }

    return nombre_items == size(*items);
}

typedef struct livraison
{
    cle_t restaurant;
    cle_t items[TAILLE_MENU];
    cle_t livreur;
} livraison;

bool livraison_par_restaurant(
    void const* l,
    void const* r)
{
    return ((livraison const*)l)->restaurant == *((cle_t const*)r);
}

// Si la commande est valide, débite le client et crédite le livreur et le·s restaurant·s.
void le_passer_commande(
    cle_t const ix,
    vector* items)
{
    assert(le_valider_commande(ix, items, NULL, NULL));

    vector restaurants = le_liste_restaurants();

    // Gather information for each delivery: the restaurant, the items, the deliverer.
    vector livraisons = make_vector(sizeof(livraison), 0);
    for(iterator i = begin(items); compare(i, end(items)) != 0; increment(&i, 1))
    {
        // Trouve le restaurant qui sera la source de cet item.
        iterator const r = find_if_2(begin(&restaurants), end(&restaurants), restaurant_menu_contient, (cle_t*)value(i));
        cle_t const ix_restaurant = ((restaurant const*)value(r))->index;

        iterator const l = find_if_2(begin(&livraisons), end(&livraisons), livraison_par_restaurant, &ix_restaurant);
        
        if(compare(l, end(&livraisons)) == 0)
        {
            // Crée une nouvelle livraison.
            livraison ll;
            memset(&ll.items, 0, TAILLE_MENU * sizeof(cle_t));

            ll.restaurant = ix_restaurant;
            ll.items[0] = *(cle_t*)value(i);
            ll.livreur = le_livreur_commande(ix_restaurant, le_cherche_client_i(ix)->code_postal);

            push_back(&livraisons, &ll);
        }
        else
        {
            livraison *ll = (livraison*)value(l);
            size_t k = 0;
            for(; k != TAILLE_MENU && ll->items[k] != 0; ++k)
            {}

            ll->items[k] = *(cle_t*)value(i);
        }
    }

    // Credit the deliverers and the restaurants.
    for(iterator l = begin(&livraisons), e = end(&livraisons); compare(l, e) != 0; increment(&l, 1))
    {
        livraison *ll = (livraison*)value(l);

        int sous_total = 0;
        for(size_t k = 0; k != TAILLE_MENU && ll->items[k] != 0; ++k)
        {
            sous_total += le_cherche_item_i(ll->items[k])->prix;
        }

        le_cherche_livreur_i(ll->livreur)->solde += 3;
        le_cherche_restaurant_i(ll->restaurant)->solde += sous_total - 3;
    }

    // Log the order.
    char line[1024] = {'\0'};
    sprintf(line, "Client %zu a passé commande {", ix);
    for(iterator l = begin(&livraisons), e = end(&livraisons); compare(l, e) != 0; increment(&l, 1))
    {
        livraison *ll = (livraison*)value(l);

        sprintf(line + strlen(line), "{%zu, [", ll->restaurant);
        for(size_t k = 0; k != TAILLE_MENU && ll->items[k] != 0; ++k)
        {
            if(k != 0)
            {
                sprintf(line + strlen(line), ", ");
            }
            sprintf(line + strlen(line), "%zu", ll->items[k]);
        }
        sprintf(line + strlen(line), "], %zu} ", ll->livreur);
    }
    sprintf(line + strlen(line), "}");
    llog("%s", line);

    destroy(&restaurants);
    destroy(&livraisons);

    le_cherche_client_i(ix)->solde -= le_total_commande(items);
}

int le_total_commande(
    vector const* items)
{
    int total = 0;

    for(iterator i = begin(items); compare(i, end(items)) != 0; increment(&i, 1))
    {
        total += le_cherche_item_i(*(cle_t*)value(i))->prix;
    }

    return total;
}

cle_t le_livreur_commande(
    cle_t const ix_restaurant,
    char const* const code_postal)
{
    // Il suffit de trouver un livreur qui peut se déplacer chez le restaurant et le client.
    for(iterator i = begin(&table_livreurs), e = end(&table_livreurs); compare(i, e) != 0; increment(&i, 1))
    {
        livreur const* l = (livreur*)value(i);
        restaurant const* r = le_cherche_restaurant_i(ix_restaurant);

        if(l->restaurant == r->index)
        {
            // C'est un livreur qui nous est exclusif. Il suffit de trouver le code postal dans ses déplacements possible.
            for(int j = 0; j != TAILLE_DEPLACEMENTS; ++j)
            {
                if(strcmp(l->deplacements[j], code_postal) == 0)
                {
                    return l->index;
                }
            }
        }
        else if(l->restaurant == 0)
        {
            // C'est un livreur freelance. Il faut trouver dans ses déplacements le code postal du restaurant et celui donné.
            bool deplace_restaurant = false;
            for(int j = 0; j != TAILLE_DEPLACEMENTS && !deplace_restaurant; ++j)
            {
                if(strcmp(l->deplacements[j], r->code_postal) == 0)
                {
                    deplace_restaurant = true;
                }
            }

            bool deplace_client = false;
            for(int j = 0; j != TAILLE_DEPLACEMENTS && !deplace_client; ++j)
            {
                if(strcmp(l->deplacements[j], (char*)code_postal) == 0)
                {
                    deplace_client = true;
                }
            }

            if(deplace_restaurant && deplace_client)
            {
                return l->index;
            }
        }
    }

    return 0;
}
