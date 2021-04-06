#pragma once

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
