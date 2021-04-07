#pragma once

#include "db/db.h"

#include <stdbool.h>

restaurant* le_cherche_restaurant(
    char const* string);

livreur* le_cherche_livreur(
    char const* string);

client* le_cherche_client(
    char const* string);

// Returns true if 'string' can be found as a name or phone number anywhere in the DB.
bool le_compte_existe(
    char const* string);

void le_creer_compte_restaurateur(
    char const* nom,
    char const* code_postal,
    char const* telephone,
    char const* type);

void le_creer_compte_livreur(
    char const* nom,
    char const* telephone);

void le_creer_compte_client(
    char const* nom,
    char const* code_postal,
    char const* telephone);
