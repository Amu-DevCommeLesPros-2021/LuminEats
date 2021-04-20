#pragma once

#include "vector/types.h"

#include <stdio.h>

// Tables (globales) de la BdD.
extern vector table_restaurants, table_items, table_livreurs, table_clients;


// Renvoie un vecteur ayant le contenu de la table de restaurants 'fichier'.
vector lecture_table_restaurants(
    FILE* fichier);

// Écrit le contenu du vecteur 'restaurants' dans 'fichier'.
void ecriture_table_restaurants(
    FILE* fichier,
    vector const* restaurants);


// Renvoie un vecteur ayant le contenu de la table d'items 'fichier'.
vector lecture_table_items(
    FILE* fichier);

// Écrit le contenu du vecteur 'items' dans 'fichier'.
void ecriture_table_items(
    FILE* fichier,
    vector const* items);


// Renvoie un vecteur ayant le contenu de la table de livreurs 'fichier'.
vector lecture_table_livreurs(
    FILE* fichier);

// Écrit le contenu du vecteur 'livreurs' dans 'fichier'.
void ecriture_table_livreurs(
    FILE* fichier,
    vector const* livreurs);


// Renvoie un vecteur ayant le contenu de la table de clients 'fichier'.
vector lecture_table_clients(
    FILE* fichier);

// Écrit le contenu du vecteur 'clients' dans 'fichier'.
void ecriture_table_clients(
    FILE* fichier,
    vector const* clients);
