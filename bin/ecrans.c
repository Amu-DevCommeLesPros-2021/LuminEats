#include "ecrans.h"

#include "db/tables.h"
#include "lumineats/lumineats.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#if defined(__linux__)
    #include <stdio_ext.h>
#endif
#include <string.h>

#define TAILLE_SAISIE 128

// Currently logged in user.
char nom_utilisateur[TAILLE_CHAMP_NOM] = {'\0'};

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

// Prompting function used internally by other prompting functions.
// It manages the capture of a (potentially empty) line and gets rid of the newline caracter.
char const* internal_prompt_string(
    int const count,
    char const* format,
    va_list args)
{
    static char buffer[TAILLE_SAISIE];
    assert(count < TAILLE_SAISIE);

    vprintf(format, args);
    fflush(stdout);

    fgets(buffer, count + 1, stdin);

    #if defined(__linux__)
        __fpurge(stdin);
    #else
        fpurge(stdin);
    #endif

    // Replace the ending '\n' by '\0'.
    char *c = strrchr(buffer, '\n');
    if(c)
    {
        *c = '\0';
    }

    return buffer;
}

// Prints the prompt and re-prompts if the user just pressed 'enter'.
char const* prompt_string(
    int const count,
    char const* format,
    ...)
{
    char const* saisie = NULL;
    do
    {
        va_list args;
        va_start(args, format);
        saisie = internal_prompt_string(count, format, args);
        va_end(args);
    }
    while(strlen(saisie) == 0);

    return saisie;
}

// Prints the proompt and if the user just pressed enter, returns the first argument in the va_list.
// (Fragile? Yes, it is. I agree.)
char const* prompt_optional_string(
    int const count,
    char const* format,
    ...)
{
    static char buffer[TAILLE_SAISIE];
    va_list args;
    
    {
        va_start(args, format);
        strcpy(buffer, internal_prompt_string(count, format, args));
        va_end(args);
    }

    // If the user simply "newlined" the prompt away, we'll return the first argument.
    if(strlen(buffer) == 0)
    {
        va_start(args, format);
        strcpy(buffer, va_arg(args, char*));
        va_end(args);
    }

    return buffer;
}

// Uses prompt_string but returns only the first character.
char prompt_choice(
    char const* format,
    ...)
{
    va_list args;
    va_start(args, format);
    char const c = prompt_string(1, format, args)[0];
    va_end(args);

    return c;
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
        case 'z':
            le_dump_tables(stdout);
            break;
    }
}

void connexion_compte(
    vector* pile)
{
    printf("\n\
* Connexion à votre compte*\n");

    char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez nom ou téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
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
                void *c;
                if((c = le_cherche_restaurant(saisie)) != NULL)
                {
                    strcpy(nom_utilisateur, ((restaurant*)c)->nom);

                    ecran e = restaurateur_principal;
                    push_back(pile, &e);
                }
                else if((c = le_cherche_livreur(saisie)) != NULL)
                {
                    strcpy(nom_utilisateur, ((livreur*)c)->nom);

                    ecran e = livreur_principal;
                    push_back(pile, &e);
                }
                else
                {
                    c = le_cherche_client(saisie);

                    strcpy(nom_utilisateur, ((client*)c)->nom);

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

    char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez le nom de votre restaurant ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_CODEPOSTAL, "Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_TYPE, "Saisissez le type de cuisine ('q' pour quitter, 'p' pour menu précédent) : ");
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

   char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_optional_string(TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL, "Saisissez vos code postaux de déplacement ('enter' pour garder vide) : ", "");
    char deplacements[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    strcpy(deplacements, saisie);

    size_t index_restaurant;
    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_NOM, "Saisissez votre restaurant ('enter' pour garder vide) : ");
        if(strlen(saisie) == 0 || strcmp(saisie, "0") == 0)
        {
            index_restaurant = 0;
            saisie_valide = true;
        }
        else
        {
            restaurant *r = le_cherche_restaurant(saisie);
            if(!r)
            {
                printf("Erreur : ce restaurant n'existe pas.\n");
            }
            else
            {
                index_restaurant = r->index;
                saisie_valide = true;
            }
        }
    }


    // Ajoute le livreur à la BdD.
    le_creer_compte_livreur(nom, telephone, deplacements, index_restaurant);

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

    char const* saisie = prompt_string(TAILLE_CHAMP_NOM, "Saisissez votre nom ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre numéro de téléphone ('q' pour quitter, 'p' pour menu précédent) : ");
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

    saisie = prompt_string(TAILLE_CHAMP_CODEPOSTAL, "Saisissez votre code postal ('q' pour quitter, 'p' pour menu précédent) : ");
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
* Menu Restaurateur * %s *\n\
\n\
Vous voulez :\n\
1. Modifier votre menu (ajouter/modifier/supprimer)\n\
2. Confirmer votre solde\n\
3. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, , 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            break;
        case '2':
            {
                ecran e = restaurateur_voir_solde;
                push_back(pile, &e);
            }
            break;
        case '3':
            le_supprimer_compte(nom_utilisateur);
            __attribute__((fallthrough));
        case 'd':
            nom_utilisateur[0] = '\n';

            // Revient au menu initial.
            clear(pile);
            ecran e = initial;
            push_back(pile, &e);
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void restaurateur_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Restaurateur * %s *\n\
\n", nom_utilisateur);

    restaurant const* const r = le_cherche_restaurant(nom_utilisateur);

    printf("Votre solde courant : €%zu\n\n", r->solde);

    pop_back(pile);
}

void livreur_principal(
    vector* pile)
{
    printf("\n\
* Menu Livreur * %s *\n\
\n\
Vous voulez :\n\
1. Modifier votre profil\n\
2. Confirmer votre solde\n\
3. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = livreur_modifier_profil;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = livreur_voir_solde;
                push_back(pile, &e);
            }
            break;
        case '3':
            le_supprimer_compte(nom_utilisateur);
            __attribute__((fallthrough));
        case 'd':
            nom_utilisateur[0] = '\n';

            // Revient au menu initial.
            clear(pile);
            ecran e = initial;
            push_back(pile, &e);
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void livreur_modifier_profil(
    vector* pile)
{
    printf("\n\
* Menu Lvrieur * %s *\n\
\n", nom_utilisateur);

    livreur const* const l = le_cherche_livreur(nom_utilisateur);

    char const* saisie = prompt_optional_string(TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL, "Saisissez vos nouveaux code postaux de déplacement ('enter' pour conserver) [%s] : ", l->deplacements_s);
    char deplacements[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
    strcpy(deplacements, saisie);

    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre nouveau téléphone ('enter' pour conserver) [%s] : ", l->telephone);
        if(le_compte_existe(saisie) && strcmp(l->telephone, saisie) != 0)
        {
            printf("Erreur : ce téléphone existe déjà.\n");
        }
        else
        {
            saisie_valide = true;
        }
    }
    char telephone[TAILLE_CHAMP_TELEPHONE];
    strcpy(telephone, saisie);


    cle_t index_restaurant = l->restaurant;
    char nom_restaurant[TAILLE_CHAMP_NOM] = {'\0'};
    if(index_restaurant != 0)
    {
        restaurant *r = le_cherche_restaurant_i(index_restaurant);
        strcpy(nom_restaurant, r->nom);
    }
    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_NOM, "Saisissez votre restaurant ('enter' pour conserver, '0' pour effacer) [%s] : ", nom_restaurant);
        if(strlen(saisie) == 0 || strcmp(saisie, "0") == 0)
        {
            index_restaurant = 0;
            saisie_valide = true;
        }
        else
        {
            restaurant *r = le_cherche_restaurant(saisie);
            if(!r)
            {
                printf("Erreur : ce restaurant n'existe pas.\n");
            }
            else
            {
                index_restaurant = r->index;
                saisie_valide = true;
            }
        }
    }

    le_modifier_profil_livreur(l->index, deplacements, telephone, index_restaurant);

    pop_back(pile);
}

void livreur_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Livreur * %s *\n\
\n", nom_utilisateur);

    livreur const* const l = le_cherche_livreur(nom_utilisateur);

    printf("Votre solde courant : €%zu\n\n", l->solde);

    pop_back(pile);
}

void client_principal(
    vector* pile)
{
    printf("\n\
* Menu Client * %s *\n\
\n\
Vous voulez :\n\
1. Modifier votre profil\n\
2. Confirmer votre solde\n\
3. Ajoutez du crédit à votre solde\n\
4. Supprimer votre compte\n\
\n", nom_utilisateur);

    char const choice = prompt_choice("Votre choix ('q' pour quitter, 'd' pour deconnexion) : ");
    switch(choice)
    {
        case '1':
            {
                ecran e = client_modifier_profil;
                push_back(pile, &e);
            }
            break;
        case '2':
            {
                ecran e = client_voir_solde;
                push_back(pile, &e);
            }
            break;
        case '3':
            {
                ecran e = client_crediter_solde;
                push_back(pile, &e);
            }
            break;
        case '4':
            le_supprimer_compte(nom_utilisateur);
            __attribute__((fallthrough));
        case 'd':
            nom_utilisateur[0] = '\n';

            // Revient au menu initial.
            {
                clear(pile);
                ecran e = initial;
                push_back(pile, &e);
            }
            break;
        case 'q':
            clear(pile);
            break;
    }
}

void client_modifier_profil(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    char const* saisie = prompt_optional_string(TAILLE_CHAMP_CODEPOSTAL, "Saisissez votre nouveau code postal ('enter' pour conserver) [%s] : ", c->code_postal);
    char code_postal[TAILLE_CHAMP_CODEPOSTAL];
    strcpy(code_postal, saisie);

    for(bool saisie_valide = false; !saisie_valide;)
    {
        saisie = prompt_optional_string(TAILLE_CHAMP_TELEPHONE, "Saisissez votre nouveau téléphone ('enter' pour conserver) [%s] : ", c->telephone);
        if(le_compte_existe(saisie) && strcmp(c->telephone, saisie) != 0)
        {
            printf("Erreur : ce téléphone existe déjà.\n");
        }
        else
        {
            saisie_valide = true;
        }
    }
    char telephone[TAILLE_CHAMP_TELEPHONE];
    strcpy(telephone, saisie);

    le_modifier_profil_client(c->index, code_postal, telephone);

    pop_back(pile);
}

void client_crediter_solde(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    char const* saisie = prompt_string(sizeof(size_t), "Saisissez le montant à ajouter ('0' pour ne rien ajouter) : ");
    le_crediter_solde_client(c->index, atoll(saisie));

    pop_back(pile);
}

void client_voir_solde(
    vector* pile)
{
printf("\n\
* Menu Client * %s *\n\
\n", nom_utilisateur);

    client const* const c = le_cherche_client(nom_utilisateur);

    printf("Votre solde courant : €%zu\n\n", c->solde);

    pop_back(pile);
}
