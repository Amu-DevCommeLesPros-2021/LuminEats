#pragma once

#include "vector/vector_api.h"
#include "vector/vector_types.h"

#include <stdbool.h>
#include <stdio.h>

#define TAILLE_CHAMP_NOM 72

#define TAILLE_CHAMP_CODEPOSTAL 6
#define TAILLE_CHAMP_TELEPHONE 15
#define TAILLE_CHAMP_TYPE 36
#define TAILLE_ITEMS 10

#define TAILLE_INGREDIENTS 5
#define TAILLE_CHAMP_INGREDIENT 36

typedef size_t cle_t;

typedef struct restaurant
{
    cle_t index;   // Clé primaire.
    char nom[TAILLE_CHAMP_NOM];
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    char telephone[TAILLE_CHAMP_TELEPHONE];
    char type[TAILLE_CHAMP_TYPE];
    char items_s[TAILLE_ITEMS * 3];
    cle_t items[TAILLE_ITEMS];
    size_t solde;
} restaurant;

vector lecture_table_restaurants(
    FILE* fichier);

void ecriture_table_restaurants(
    FILE* fichier,
    vector const* db);

typedef struct item
{
    cle_t index;   // Clé primaire.
    char nom[TAILLE_CHAMP_NOM];
    char ingredients_s[TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT];
    char ingredients[TAILLE_INGREDIENTS][TAILLE_CHAMP_INGREDIENT];
    size_t prix;
} item;

vector lecture_table_items(
    FILE* fichier);

void ecriture_table_items(
    FILE* fichier,
    vector const* db);

#define TAILLE_DEPLACEMENTS 10

typedef struct livreur
{
    cle_t index;   // Clé primaire.
    char nom[TAILLE_CHAMP_NOM];
    char telephone[TAILLE_CHAMP_TELEPHONE];
    char deplacements_s[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    char deplacements[TAILLE_DEPLACEMENTS][TAILLE_CHAMP_CODEPOSTAL];
    cle_t restaurant;
    size_t solde;
} livreur;

vector lecture_table_livreurs(
    FILE* fichier);

void ecriture_table_livreurs(
    FILE* fichier,
    vector const* db);

typedef struct client
{
    cle_t index;   // Clé primaire.
    char nom[TAILLE_CHAMP_NOM];
    char telephone[TAILLE_CHAMP_TELEPHONE];
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    size_t solde;
} client;

vector lecture_table_clients(
    FILE* fichier);

void ecriture_table_clients(
    FILE* fichier,
    vector const* db);

// Tables (globales) de la DB.
extern vector table_restaurants, table_items, table_livreurs, table_clients;

// Lit toutes les tables de la DB.
// 'dossier' est le dossier où se trouvent les fichiers '.csv'.
void ouverture_db(
    char const* dossier);

// Écrit toutes les tables de la DB.
// 'dossier' est le dossier où seront écrits les fichiers '.csv'.
void fermeture_db(
    char const* dossier);