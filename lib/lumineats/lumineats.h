#pragma once

#include "db/db.h"

#include <stdio.h>
#include <stdbool.h>

// Fonction de débogage.
// Affiche les contenu des tables sur 'f'.
void le_dump_tables(
    FILE* f);

// Renvoie le restaurant avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
restaurant* le_cherche_restaurant(
    char const* string);

// Renvoie le restaurant avec l'index correspondant.
// Renvoie 'NULL' si non-trouvé.
restaurant* le_cherche_restaurant_i(
    cle_t const index);

// Renvoie le livreur avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
livreur* le_cherche_livreur(
    char const* string);

// Renvoie le client avec le nom ou telephone correspondant.
// Renvoie 'NULL' si non-trouvé.
client* le_cherche_client(
    char const* string);


// Renvoie 'vrai' si un restaurant, livreur ou client a le nom ou le téléphone donné.
bool le_compte_existe(
    char const* string);

// Crée un nouveau restaurant et renvoie un pointeur au restaurant nouvellement créé.
// Les arguments 'nom' et 'telephone' ne doivent pas déjà dans la BdD.
// Si 'nom' ou 'telephone' existent déjà dans la BdD, ne crée rien et renvoie 'NULL'.
restaurant* le_creer_compte_restaurateur(
    char const* nom,
    char const* code_postal,
    char const* telephone,
    char const* type);

// Crée un nouveau livreur et renvoie un pointeur au livreur nouvellement créé.
// Les arguments 'nom' et 'telephone' ne doivent pas déjà dans la BdD.
// Si 'nom' ou 'telephone' existent déjà dans la BdD, ne crée rien et renvoie 'NULL'.
livreur* le_creer_compte_livreur(
    char const* nom,
    char const* telephone);

// Modifie les informations d'un livreur avec l'index donné.
// 'telephone' doit etre unique.
// 'index_restaurant' doit être zéro ou valide.
// Renvoie 'vrai' si l'opération réussi.
bool le_modifier_profil_livreur(
    cle_t const index,
    char const* deplacements,
    char const* telephone,
    cle_t const index_restaurant);

// Crée un nouveau client et renvoie un pointeur au client nouvellement créé.
// Les arguments 'nom' et 'telephone' ne doivent pas déjà dans la BdD.
// Si 'nom' ou 'telephone' existent déjà dans la BdD, ne crée rien et renvoie 'NULL'.
client* le_creer_compte_client(
    char const* nom,
    char const* code_postal,
    char const* telephone);

// Modifie les informations du client avec l'index donné.
// 'telephone' doit etre unique.
// Renvoie 'vrai' si l'opération réussi.
bool le_modifier_profil_client(
    cle_t const index,
    char const* code_postal,
    char const* telephone);

// Ajoute un crédit au solde existant.
void le_crediter_solde_client(
    cle_t const index,
    size_t montant);

// Supprime le compte correspondant au nom ou téléphone donné s'il existe.
void le_supprimer_compte(
    char const* string);
