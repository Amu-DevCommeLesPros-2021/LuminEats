#include "db.h"

#include "vector_api.h"
#include "vector_types.h"

#include <stdio.h>
#include <string.h>

vector lecture_table_restaurant(
    FILE* fichier)
{
    vector restaurants = make_vector(sizeof(restaurant), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        restaurant r;
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%zu\n", &r.index, r.nom, r.code_postal, r.telephone, r.type, r.items_s, &r.solde);
        
        // Parse string of semicolon separated items.
        memset(r.items, 0, sizeof(r.items));
        int i = 0;
        for(char *item = strtok(r.items_s, ";"); item; item = strtok(r.items_s, ";"))
        {
            r.items[i++] = atoi(item);
        }

        push_back(&restaurants, &r);
    }
    free(buffer);

    return restaurants;
}

void ecriture_table_restaurant(
    FILE* fichier,
    vector const* db)
{
    for(iterator i = begin(db); compare(i, end(db)) != 0; increment(&i, 1))
    {
        restaurant *r = (restaurant*)value(i);

        fprintf(fichier, "%zu,%s,%s,%s,%s", r->index, r->nom, r->code_postal, r->telephone, r->type);

        for(int i = 0; r->items[i] != 0 && i != TAILLE_ITEMS; ++i)
        {
            fprintf(fichier, "%zu", r->items[i]);
            if(i != TAILLE_ITEMS && r->items[i + 1] != 0)
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, "%zu\n", r->solde);
    }
}
