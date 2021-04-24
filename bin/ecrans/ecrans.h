#pragma once

#include "db/types.h"
#include "vector/vector.h"

typedef void (*ecran)(vector*);    // Un fonction qui va afficher un écran manipule la pile d'écrans.

// Currently logged in user.
extern char nom_utilisateur[TAILLE_CHAMP_NOM];

// Écran initial.
void initial(
    vector* pile);


// Écrans connexion, création et suppression de compte.
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


// Écrans Restaurateur.
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


// Écrans Menu.
void menu_creer_item(
    vector* pile);

void menu_ajouter_item(
    vector* pile);

void menu_supprimer_item(
    vector* pile);


// Écrans Livreur.
void livreur_principal(
    vector* pile);

void livreur_modifier_profil(
    vector* pile);

void livreur_voir_solde(
    vector* pile);

// Écrans Client.
void client_principal(
    vector* pile);

void client_modifier_profil(
    vector* pile);

void client_crediter_solde(
    vector* pile);

void client_voir_solde(
    vector* pile);

void client_lister_restaurants(
    vector* pile);
