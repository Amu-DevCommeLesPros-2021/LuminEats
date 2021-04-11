#include "ecrans.h"

#include "db/tables.h"
#include "lumineats/lumineats.h"

#include <stdio.h>
#if defined(__linux__)
    #include <stdio_ext.h>
#endif
#include <string.h>

#define TAILLE_SAISIE 128

int strcpy_s(
    char *dest,
    int const count,
    const char* src)
{
    if(count <= 1 || !dest)
    {
        if(!dest || !count)
        {
            return 0;
        }
        *dest = '\0';
        return 0;
    }
    
    for(char* d = dest + count - 1; (dest != d || (*dest = '\0')) && (*dest = *src); ++dest, ++src);
    
    return 0;
}

char const* prompt_string(
    char const* line,
    int const count)
{
    static char buffer[TAILLE_SAISIE];
    char input;
    do
    {
        printf("%s", line);
        input = getchar();
    }
    while(input == '\n');
    buffer[0] = input;
    fgets(&buffer[1], count - 1, stdin);
#if defined(__linux__)    
    __fpurge(stdin);
#else
    fpurge(stdin);
#endif

    char* const c = strrchr(buffer, '\n');
    if(c)
    {
        *c = '\0';
    }

    return buffer;
}

char prompt_choice(
    char const* line)
{
    return prompt_string(line, 1)[0];
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

    char const* saisie = prompt_string("Saisissez nom ou téléphone ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_NOM);
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            break;
        case 'q':
            clear(pile);
            break;
        default:
            if(!le_compte_existe(saisie))
            {
                printf("Ce compte n'existe pas.\n");
            }
            else
            {
                if(le_cherche_restaurant(saisie) != NULL)
                {
                    ecran e = restaurateur_principal;
                    push_back(pile, &e);
                }
                else if(le_cherche_livreur(saisie) != NULL)
                {
                    ecran e = livreur_principal;
                    push_back(pile, &e);
                }
                else
                {
                    ecran e = client_principal;
                    push_back(pile, &e);
                }
            }
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

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'p' pour menu précédent) : ");
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

    char const* saisie = prompt_string("Saisissez le nom de votre restaurant ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_NOM);
    char nom[TAILLE_CHAMP_NOM];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            return;
            break;
        case 'q':
            clear(pile);
            return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte de ce nom existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(nom, TAILLE_CHAMP_NOM, saisie);
            }
            break;
    }

    saisie = prompt_string("Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_TELEPHONE);
    char telephone[TAILLE_CHAMP_TELEPHONE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte avec ce numero existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(telephone, TAILLE_CHAMP_TELEPHONE, saisie);
            }
            break;
    }

    saisie = prompt_string("Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_CODEPOSTAL);
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            strcpy_s(code_postal, TAILLE_CHAMP_CODEPOSTAL, saisie);
            break;
    }

    saisie = prompt_string("Saisissez le type de cuisine ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_TYPE);
    char type[TAILLE_CHAMP_TYPE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            strcpy_s(type, TAILLE_CHAMP_TYPE, saisie);
            break;
    }

    // Ajoute le restaurant à la BdD.
    le_creer_compte_restaurateur(nom, code_postal, telephone, type);

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

   char const* saisie = prompt_string("Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_NOM);
    char nom[TAILLE_CHAMP_NOM];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            return;
            break;
        case 'q':
            clear(pile);
            return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte de ce nom existe déjà.\n");
                return;
            }
            else
            {
               strcpy_s(nom, TAILLE_CHAMP_NOM, saisie);
            }
            break;
    }

    saisie = prompt_string("Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_TELEPHONE);
    char telephone[TAILLE_CHAMP_TELEPHONE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte avec ce numero existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(telephone, TAILLE_CHAMP_TELEPHONE, saisie);
            }
            break;
    }

    // Ajoute le livreur à la BdD.
    le_creer_compte_livreur(nom, telephone);

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

    char const* saisie = prompt_string("Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_NOM);
    char nom[TAILLE_CHAMP_NOM];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile);
            return;
            break;
        case 'q':
            clear(pile);
            return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte de ce nom existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(nom, TAILLE_CHAMP_NOM, saisie);
            }
            break;
    }

    saisie = prompt_string("Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_TELEPHONE);
    char telephone[TAILLE_CHAMP_TELEPHONE];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            if(le_compte_existe(saisie))
            {
                printf("Un compte avec ce numero existe déjà.\n");
                return;
            }
            else
            {
                strcpy_s(telephone, TAILLE_CHAMP_TELEPHONE, saisie);
            }
            break;
    }

    saisie = prompt_string("Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ", TAILLE_CHAMP_CODEPOSTAL);
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    switch(saisie[0])
    {
        case 'p':
            pop_back(pile); return;
            break;
        case 'q':
            clear(pile); return;
            break;
        default:
            strcpy_s(code_postal, TAILLE_CHAMP_CODEPOSTAL, saisie);
            break;
    }

    // Ajoute le client à la BdD.
    le_creer_compte_client(nom, code_postal, telephone);

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

void livreur_principal(
    vector* pile)
{
    printf("\n\
* Menu Livreur *\n\
\n\
Vous voulez :\n\
1. Modifier votre profil\n\
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

void client_principal(
    vector* pile)
{
    printf("\n\
* Menu Client *\n\
\n\
Vous voulez :\n\
1. Modifier votre profil\n\
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