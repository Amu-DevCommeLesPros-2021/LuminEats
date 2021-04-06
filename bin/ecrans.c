#include "ecrans.h"

#include <stdio.h>

char prompt_choice(
    char const* line)
{
    char input;
    do
    {
        printf("%s", line);
        input = getchar();
    }
    while(input == '\n');
    getchar();

    return input;
}

void prompt_string(
    char const* line,
    char* buffer)
{
    char input;
    do
    {
        printf("%s", line);
        input = getchar();
    }
    while(input == '\n');
    buffer[0] = input;
    gets(&buffer[1]);
}

void initial(
    vector* pile)
{
    printf("\n\
* Menu Principal *\n\
\n\
1. Vous connecter à votre compte\n\
2. Créer un nouveau compte\n\
\n");

    char const choice = prompt_choice("Votre choix ('q' pour quitter) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = connexion_compte;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = creation_compte;
                push_back(pile, &e);
            }
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void connexion_compte(
    vector* pile)
{
    printf("\n\
* Connexion à votre compte*\n");

    char saisie[128];
    prompt_string("Saisissez nom ou téléphone ('q' pour quitter, 'p' pour menu précédent) : ", saisie);
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom ou le numéro de téléphone dans la DB.
            // Si existant, présente l'écran correspondant au type de compte (p. ex. restaurateur_principal).
            // Si inexistant, réessaie n fois.
            break;
    }
}

void creation_compte(
    vector* pile)
{
    printf("\n\
* Création de votre compte *\n\
\n\
Vous êtes :\n\
1. Restaurateur·trice\n\
2. Livreur·se\n\
3. Client·e\n");

    char const choice = prompt_choice("Votre choix ('q' pour quitter) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = creation_compte_restaurateur;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = creation_compte_livreur;
                push_back(pile, &e);
            }
            break;
        case '3':
            {
                ecran e = creation_compte_client;
                push_back(pile, &e);
            }
            break;
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void creation_compte_restaurateur(
    vector* pile)
{
    printf("\n\
* Création d'un compte Restaurateur *\n");

    char nom[128];
    prompt_string("Saisissez le nom de votre restaurant ('q' pour quitter, 'p' pour menu précédent) : ", nom);

    switch(nom[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    char code_postal[128];
    prompt_string("Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ", code_postal);

    switch(code_postal[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            // Validation ?
            break;
    }

    char telephone[128];
    prompt_string("Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ", telephone);

    switch(telephone[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            // Validation ?
            break;
    }

    char type[128];
    prompt_string("Saisissez le type de cuisine ('q' pour quitter, 'p' pour menu précédent) : ", type);

    switch(type[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            // Validation ?
            break;
    }

    // Ajoute le restaurant à la DB.

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void creation_compte_livreur(
    vector* pile)
{
    printf("\n\
* Création d'un compte Livreur *\n");

    char nom[128];
    prompt_string("Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ", nom);

    switch(nom[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    char telephone[128];
    prompt_string("Saisissez votre telephone ('q' pour quitter, 'p' pour menu précédent) : ", telephone);

    switch(telephone[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void creation_compte_client(
    vector* pile)
{
    printf("\n\
* Création d'un compte Client *\n");

    char nom[128];
    prompt_string("Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ", nom);

    switch(nom[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    char code_postal[128];
    prompt_string("Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ", code_postal);

    switch(code_postal[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    char telephone[128];
    prompt_string("Saisissez votre telephone ('q' pour quitter, 'p' pour menu précédent) : ", telephone);

    switch(telephone[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            // Cherche le nom dans la DB.
            // Si existant, signifié et réessaie n fois.
            break;
    }

    // Revient au menu initial.
    clear(pile);
    ecran e = initial;
    push_back(pile, &e);
}

void restaurateur_principal(
    vector* pile)
{
    printf("\n\
* Menu Restaurateur *\n\
\n\
Vous voulez :\n\
1. Modifier votre menu (ajouter/modifier/supprimer)\n\
2. Confirmer votre solde\n\
3. Supprimer votre compte\n\
\n");
    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'p' pour menu précédent) : ");
    switch(choice)
    {
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
    }
}