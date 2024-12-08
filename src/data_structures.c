/**
 * Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "../include/data_structures.h"
#include <string.h>

/* Initialize the dictionary */
void
initDictionary (Dictionary *dict)
{
    dict->keys = NULL;
    dict->values = NULL;
    dict->size = 0;
}

/* Free the dictionary */
void
freeDictionary (Dictionary *dict)
{
    for (size_t i = 0; i < dict->size; i++)
        {
            free (dict->keys[i]);
            free (dict->values[i]);
        }
    free (dict->keys);
    free (dict->values);
    dict->size = 0;
}

/**
 * Function: addHeader
 * -------------------
 * Adds a header to the dictionary.
 *
 * dict: Pointer to the dictionary.
 * key: Key of the header.
 * value: Value of the header.
 */
void
addHeader (Dictionary *dict, const char *key, const char *value)
{
    char **new_keys = realloc(dict->keys, (dict->size + 1) * sizeof(char *));
    char **new_values = realloc(dict->values, (dict->size + 1) * sizeof(char *));

    if (new_keys == NULL || new_values == NULL)
    {
        free(new_keys);
        free(new_values);
        logError("Failed to allocate memory for headers");
        return;
    }

    dict->keys = new_keys;
    dict->values = new_values;
    dict->keys[dict->size] = strdup(key);
    dict->values[dict->size] = strdup(value);
    if (dict->keys[dict->size] == NULL || dict->values[dict->size] == NULL)
    {
        free(dict->keys[dict->size]);
        free(dict->values[dict->size]);
        logError("Failed to duplicate header key or value");
        return;
    }
    dict->size++;
}
