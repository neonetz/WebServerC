#ifndef ROUTES_H
#define ROUTES_H

#include <stdlib.h>
#include <string.h>

struct Route {
	char* key;
	char* value;

	struct Route *left, *right;
};

struct Route *initialize_routes();

struct Route * initRoute(char* key, char* value);

struct Route *addRoute(struct Route *root, char* key, char* value);

struct Route * search(struct Route * root, char * key);

struct Route *deleteRoute(struct Route *root, char* key);


void inorder(struct Route * root );

#endif // ROUTES_H