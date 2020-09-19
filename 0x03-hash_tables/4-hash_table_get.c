#include "hash_tables.h"

/**
 * hash_table_get - function to get the value of a key
 * @ht: hash table to look into
 * @key: key looking for
 * Return: key or NULL
 */
char *hash_table_get(const hash_table_t *ht, const char *key)
{
	unsigned long int i = 0;
	hash_node_t *hn;

	if (ht == NULL || key == NULL)
		return (NULL);

	i = key_index((const unsigned char *)key, ht->size);
	hn = ht->array[i];

	for (; hn != NULL; hn = hn->next)
	{
		if (strcmp(hn->key, key) == 0)
			return (hn->value);
	}
	return (NULL);
}
