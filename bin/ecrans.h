#pragma once

#include "vector/vector.h"

typedef void (*ecran)(vector*);    // Un fonction qui va afficher un écran manipule la pile d'écrans.

void initial(
    vector* pile);

void connexion_compte(
    vector* pile);

void creation_compte(
    vector* pile);

void creation_compte_restaurateur(
    vector* pile);

void creation_compte_livreur(
    vector* pile);

void creation_compte_client(
    vector* pile);

void restaurateur_principal(
    vector* pile);

void restaurateur_creation_compte(
    vector* pile);

void restaurateur_suppression_compte(
    vector* pile);

void restaurateur_modification_menu(
    vector* pile);

void restaurateur_voir_solde(
    vector* pile);

void menu_creer_item(
    vector* pile);

void menu_ajouter_item(
    vector* pile);

void menu_supprimer_item(
    vector* pile);