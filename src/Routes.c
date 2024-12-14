#include "Routes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Route * initRoute(char* key, char* value) {
	struct Route * temp = (struct Route *) malloc(sizeof(struct Route));

	temp->key = key;
	temp->value = value;

	temp->left = temp->right = NULL;
	return temp;
}

void inorder(struct Route* root)
{

    if (root != NULL) {
        inorder(root->left);
        printf("%s -> %s \n", root->key, root->value);
        inorder(root->right);
    }
}

struct Route *addRoute(struct Route *root, char* key, char* value) {
    if (root == NULL) {
        return initRoute(key, value);
    }

    if (strcmp(key, root->key) == 0) {
        printf("============ WARNING ============\n");
        printf("A Route For \"%s\" Already Exists\n", key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = addRoute(root->right, key, value);
    } else {
        root->left = addRoute(root->left, key, value);
    }
    return root; // Pastikan untuk mengembalikan root
}

struct Route * search(struct Route * root, char* key) {
	if (root == NULL) {
		return NULL;
	} 

	if (strcmp(key, root->key) == 0){
		return root;
	}else if (strcmp(key, root->key) > 0) {
		return search(root->right, key);
	}else if (strcmp(key, root->key) < 0) {
		return search(root->left, key);
	}  

}

struct Route *initialize_routes() {
    struct Route *root = NULL;
    root = addRoute(root, "/", "index.html");
    root = addRoute(root, "/about", "about.html");
    return root;
}

struct Route *deleteRoute(struct Route *root, char* key) {
    if (root == NULL) return NULL;

    if (strcmp(key, root->key) < 0) {
        root->left = deleteRoute(root->left, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = deleteRoute(root->right, key);
    } else {
        if (root->left == NULL) {
            struct Route *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Route *temp = root->left;
            free(root);
            return temp;
        }

        struct Route *temp = root->right;
        while (temp && temp->left != NULL) temp = temp->left;

        root->key = temp->key;
        root->value = temp->value;
        root->right = deleteRoute(root->right, temp->key);
    }
    return root;
}
