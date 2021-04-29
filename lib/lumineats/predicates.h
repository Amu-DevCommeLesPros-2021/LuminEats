#pragma once

#include <stdbool.h>

bool restaurant_a_index(
    void const* c,
    void const* index);

bool restaurant_est_nomme(
    void const* r,
    void const* nom);

bool restaurant_a_telephone(
    void const* r,
    void const* telephone);

bool restaurant_a_nom_ou_telephone(
    void const* r,
    void const* chaine);

bool restaurant_menu_contient(
    void const* r,
    void const* index);

bool restaurant_a_type(
    void const* r,
    void const* type);

// Renvoie 'vrai' si :
// 1. un livreur attitré au restaurant peut se déplacer vers le code postal donné.
// 2. un livreur «agent libre» peut se déplacer vers le code postal du restaurant et le code postal donné.
bool restaurant_peut_livrer(
    void const* r,
    void const* code_postal);

bool livreur_a_index(
    void const* c,
    void const* index);

bool livreur_est_nomme(
    void const* l,
    void const* nom);

bool livreur_a_telephone(
    void const* l,
    void const* telephone);

bool livreur_a_nom_ou_telephone(
    void const* l,
    void const* chaine);

bool client_a_index(
    void const* c,
    void const* index);

bool client_est_nomme(
    void const* c,
    void const* nom);

bool client_a_telephone(
    void const* c,
    void const* telephone);

bool client_a_nom_ou_telephone(
    void const* c,
    void const* chaine);

bool item_a_index(
    void const* i,
    void const* index);

bool item_est_nomme(
    void const* i,
    void const* nom);

// Renvoie 'vrai' si cet item est offer par un restaurant du type donné.
bool item_a_type(
    void const* i,
    void const* type);
