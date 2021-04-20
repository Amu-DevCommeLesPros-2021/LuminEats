#pragma once

#include "vector/vector.h"

#include <stdio.h>

// Lit toutes les tables de la BdD et rempli les variables 'tables_*'
// 'dossier' est le dossier où se trouvent les fichiers '.csv'.
void ouverture_db(
    char const* dossier);

// Écrit toutes les tables de la BdD avec le contenu des variables 'tables_*.
// 'dossier' est le dossier où seront écrits les fichiers '.csv'.
void fermeture_db(
    char const* dossier);

// Remet la BdD à zéro.
void efface_db(
    char const* dossier);


// Fonction de débogage.
// Affiche les contenu des tables dans 'f'.
void le_dump_tables(
    FILE* f);
