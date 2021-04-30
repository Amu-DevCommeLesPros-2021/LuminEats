#pragma once

#include <stdbool.h>

bool restaurant_a_index(
    void const* r,
    void const* index);

bool restaurant_est_nomme(
    void const* ix,
    void const* nom);

bool restaurant_a_telephone(
    void const* ix,
    void const* telephone);

bool restaurant_a_nom_ou_telephone(
    void const* ix,
    void const* chaine);

bool restaurant_menu_contient(
    void const* ix,
    void const* index);

bool restaurant_a_type(
    void const* ix,
    void const* type);

// Renvoie 'vrai' si :
// 1. un livreur attitré au restaurant peut se déplacer vers le code postal donné.
// 2. un livreur «agent libre» peut se déplacer vers le code postal du restaurant et le code postal donné.
bool restaurant_peut_livrer(
    void const* ix,
    void const* code_postal);

bool livreur_a_index(
    void const* l,
    void const* index);

bool livreur_est_nomme(
    void const* lix,
    void const* nom);

bool livreur_a_telephone(
    void const* ix,
    void const* telephone);

bool livreur_a_nom_ou_telephone(
    void const* ix,
    void const* chaine);

bool client_a_index(
    void const* ix,
    void const* index);

bool client_est_nomme(
    void const* ix,
    void const* nom);

bool client_a_telephone(
    void const* ix,
    void const* telephone);

bool client_a_nom_ou_telephone(
    void const* ix,
    void const* chaine);

bool item_a_index(
    void const* i,
    void const* index);

bool item_est_nomme(
    void const* ix,
    void const* nom);

// Renvoie 'vrai' si cet item est offer par un restaurant du type donné.
bool item_a_type(
    void const* ix,
    void const* type);

// Renvoie 'vrai' si cet item figure au menu du restaurant donné.
bool item_menu_restaurant(
    void const* ix,
    void const* nom);

// Renvoie 'vrai' si cet item coûte aussi cher ou moins cher que le prix donné.
bool item_prix_moindre(
    void const* ix,
    void const* prix);

// Renvoie 'vrai' si cet item est offert par les restaurants donné.
bool item_offert_restaurants(
    void const *ix,
    void const *restaurants);   // 'restaurants' est un vecteur.

