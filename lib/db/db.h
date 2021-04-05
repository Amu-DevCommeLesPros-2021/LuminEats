#pragma once

#include "vector/vector_api.h"
#include "vector/vector_types.h"

#include <stdbool.h>
#include <stdio.h>

#define TAILLE_CHAMP_NOM 36
#define TAILLE_CHAMP_CODEPOSTAL 6
#define TAILLE_CHAMP_TELEPHONE 15
#define TAILLE_CHAMP_TYPE 36
#define TAILLE_ITEMS 10

typedef size_t cle_t;

typedef struct restaurant
{
    cle_t index;   // Clé primaire.
    char nom[TAILLE_CHAMP_NOM];
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    char telephone[TAILLE_CHAMP_TELEPHONE];
    char type[TAILLE_CHAMP_TYPE];
    char items_s[TAILLE_ITEMS * 2 + TAILLE_ITEMS];
    cle_t items[TAILLE_ITEMS];
    size_t solde;
} restaurant;

vector lecture_table_restaurant(
    FILE* fichier);

void ecriture_table_restaurant(
    FILE* fichier,
    vector const* db);
