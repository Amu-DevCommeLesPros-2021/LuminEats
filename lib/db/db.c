#include "db.h"

#include "vector/vector_api.h"
#include "vector/vector_types.h"

#if defined(__linux__)
    #include <linux/limits.h>
#else
    #include <sys/syslimits.h>
#endif
#if defined(__linux__)
    #define  _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>
#include <string.h>


vector table_restaurants = {0}, table_items = {0}, table_livreurs = {0}, table_clients = {0};

vector lecture_table_restaurants(
    FILE* fichier)
{
    vector restaurants = make_vector(sizeof(restaurant), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        restaurant r = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%zu\n", &r.index, r.nom, r.code_postal, r.telephone, r.type, r.items_s, &r.solde);
        
        // Parse string of semicolon separated items.
        memset(r.items, 0, sizeof(r.items));
        int i = 0;
        for(char *item = strtok(r.items_s, ";"); item; item = strtok(NULL, ";"))
        {
            r.items[i++] = atoi(item);
        }

        push_back(&restaurants, &r);
    }
    free(buffer);

    return restaurants;
}

void ecriture_table_restaurants(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,code postal,telephone,type,menu,solde\n"); // Header line.

    for(iterator i = begin(db); compare(i, end(db)) != 0; increment(&i, 1))
    {
        restaurant *r = (restaurant*)value(i);

        fprintf(fichier, "%zu,%s,%s,%s,%s,", r->index, r->nom, r->code_postal, r->telephone, r->type);

        for(int i = 0; i != TAILLE_ITEMS && r->items[i] != 0; ++i)
        {
            fprintf(fichier, "%zu", r->items[i]);
            if(i != TAILLE_ITEMS && r->items[i + 1] != 0)
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu\n", r->solde);
    }
}

vector lecture_table_items(
    FILE* fichier)
{
    vector items = make_vector(sizeof(item), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        item i = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%zu\n", &i.index, i.nom, i.ingredients_s, &i.prix);
        
        // Parse string of semicolon separated ingredients.
        memset(i.ingredients, 0, TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT);
        int j = 0;
        for(char *ingredient = strtok(i.ingredients_s, ";"); ingredient; ingredient = strtok(NULL, ";"))
        {
            strcpy(i.ingredients[j++], ingredient);
        }

        push_back(&items, &i);
    }
    free(buffer);

    return items;
}

void ecriture_table_items(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,ingredients,prix\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        item *i = (item*)value(j);

        fprintf(fichier, "%zu,%s,", i->index, i->nom);

        for(int k = 0; k != TAILLE_INGREDIENTS && i->ingredients[k][0] != '\0'; ++k)
        {
            fprintf(fichier, "%s", i->ingredients[k]);
            if(k != TAILLE_INGREDIENTS && i->ingredients[k + 1][0] != '\0')
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu\n", i->prix);
    }
}

vector lecture_table_livreurs(
    FILE* fichier)
{
    vector livreurs = make_vector(sizeof(livreur), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        livreur l = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%zu,%zu\n", &l.index, l.nom, l.telephone, l.deplacements_s, &l.restaurant, &l.solde);
        
        // Parse string of semicolon separated postal codes.
        memset(l.deplacements, 0, TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL);
        int j = 0;
        for(char *deplacement = strtok(l.deplacements_s, ";"); deplacement; deplacement = strtok(NULL, ";"))
        {
            strcpy(l.deplacements[j++], deplacement);
        }

        push_back(&livreurs, &l);
    }
    free(buffer);

    return livreurs;
}

void ecriture_table_livreurs(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,telephone,deplacements,restaurant,solde\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        livreur *l = (livreur*)value(j);

        fprintf(fichier, "%zu,%s,%s,", l->index, l->nom, l->telephone);

        for(int k = 0; k != TAILLE_DEPLACEMENTS && l->deplacements[k][0] != '\0'; ++k)
        {
            fprintf(fichier, "%s", l->deplacements[k]);
            if(k != TAILLE_DEPLACEMENTS && l->deplacements[k + 1][0] != '\0')
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu,%zu\n", l->restaurant, l->solde);
    }
}

vector lecture_table_clients(
    FILE* fichier)
{
    vector clients = make_vector(sizeof(client), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        client c = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%zu\n", &c.index, c.nom, c.code_postal, c.telephone, &c.solde);
        push_back(&clients, &c);
    }
    free(buffer);

    return clients;
}

void ecriture_table_clients(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,code postal,telephone,solde\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        client *c = (client*)value(j);

        fprintf(fichier, "%zu,%s,%s,%s,%zu\n", c->index, c->nom, c->code_postal, c->telephone, c->solde);
    }
}

void ouverture_db(
    char const* dossier)
{
    char chemin[PATH_MAX];
    
    // To help testing and allow to re-open the DB without closing (and thus writing) it first, destroy the current tables.
    destroy(&table_restaurants);
    destroy(&table_items);
    destroy(&table_livreurs);
    destroy(&table_clients);

    // Read and copy restaurants.
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "r");
    if(test_db_restaurants)
    {
        table_restaurants = lecture_table_restaurants(test_db_restaurants);
        fclose(test_db_restaurants);
    }
    else
    {
        table_restaurants = make_vector(sizeof(restaurant), 0, 2.0);
    }

    // Read and copy items.
    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "r");
    if(test_db_items)
    {
        table_items = lecture_table_items(test_db_items);
        fclose(test_db_items);
    }
    else
    {
        table_items = make_vector(sizeof(item), 0, 2.0);
    }

    // Read and copy deliverers.
    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "r");
    if(test_db_livreurs)
    {
        table_livreurs = lecture_table_livreurs(test_db_livreurs);
        fclose(test_db_livreurs);
    }
    else
    {
        table_livreurs = make_vector(sizeof(livreur), 0, 2.0);
    }

    // Read and copy customers.
    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "r");
    if(test_db_clients)
    {
        table_clients = lecture_table_clients(test_db_clients);
        fclose(test_db_clients);
    }
    else
    {
        table_clients = make_vector(sizeof(client), 0, 2.0);
    }
}

void fermeture_db(
    char const* dossier)
{
    char chemin[PATH_MAX];
    
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "w");
    ecriture_table_restaurants(test_db_restaurants, &table_restaurants);
    fclose(test_db_restaurants);
    destroy(&table_restaurants);

    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "w");
    ecriture_table_items(test_db_items, &table_items);
    fclose(test_db_items);
    destroy(&table_items);

    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "w");
    ecriture_table_livreurs(test_db_livreurs, &table_livreurs);
    fclose(test_db_livreurs);
    destroy(&table_livreurs);

    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "w");
    ecriture_table_clients(test_db_clients, &table_clients);
    fclose(test_db_clients);
    destroy(&table_clients);
}

void efface_db(
    char const* dossier)
{
    char chemin[PATH_MAX];

    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "items.csv");
    remove(chemin);

    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    remove(chemin);
}
