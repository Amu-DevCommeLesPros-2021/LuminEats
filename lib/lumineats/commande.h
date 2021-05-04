#pragma once

#include "db/types.h"
#include "vector/types.h"

#include <stdbool.h>

// Renvoie 'vrai' si les items peuvent être livrés au client et que la somme est 
// moindre ou égale à son solde.
// Renvoie 'faux' dan le cas contraire et modifie 'non_livrables' et 'depasse_solde' 
// pour refléter les items qui ne peuvent être livrés ou qui dépasse le solde disponible.
bool le_valider_commande(
    cle_t const client,
    vector* items,          // Items constituant la commande.
    vector* non_livrables,  // Items qui ne peuvent être livrés.
    vector* depassent_solde); // Items qui font dépasser le total de la commande au-delà du solde du client.

// Si la commande est valide, débite le client et crédite le livreur et le·s restaurant·s.
void le_passer_commande(
    cle_t const client,
    vector const* items); // Un vecteur d'index d'items.
