#pragma once

#include "vector/vector.h"

typedef void (*ecran)(vector*);    // Un fonction qui va afficher un écran manipule la pile d'écrans.

void principal(vector* pile);

void connexion_restaurateur(vector* pile);

void restaurateur(vector* pile);
