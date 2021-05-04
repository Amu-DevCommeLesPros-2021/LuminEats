#include "lumineats/commande.h"

#include "lumineats/filter.h"
#include "lumineats/predicates.h"
#include "lumineats/search.h"

#include "db/types.h"
#include "vector/api.h"
#include "vector/types.h"

#include <stdbool.h>

bool le_valider_commande(
    cle_t const ix,
    vector* items,
    vector* non_livrables,
    vector* depassent_solde)
{
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
            push_back(non_livrables, value(i));
            erase(items, i);
        }
    }

    // Now, remove all items that would overshoot the customer's available funds.

    int available = le_cherche_client_i(ix)->solde;
    iterator i = begin(items);
    while(compare(i, end(items)) != 0 && available > 0)
    {
        item const* const it = le_cherche_item_i(*(cle_t*)value(i));
        if(available > it->prix)
        {
            increment(&i, 1);
        }
        available -= it->prix;
    }

    while(compare(i, end(items)) != 0)
    {
        push_back(depassent_solde, value(i));
        erase(items, i);
    }

    return size(*non_livrables) == 0 && size(*depassent_solde) == 0;
}

// Si la commande est valide, débite le client et crédite le livreur et le·s restaurant·s.
void le_passer_commande(
    cle_t const client,
    vector const* items)
{
    (void)client;
    (void)items;
}
