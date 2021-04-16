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

bool restaurant_a_type(
    void const* r,
    void const* type);

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
