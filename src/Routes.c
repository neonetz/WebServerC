#include "Routes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 

/**
 * Initializes a new Route structure with the given key and value.
 *
 * @param key A string representing the key for the route.
 * @param value A string representing the value for the route.
 * @return A pointer to the newly allocated Route structure.
 */
struct Route * initRoute(char* key, char* value) {
	struct Route * temp = (struct Route *) malloc(sizeof(struct Route));

	temp->key = key;
	temp->value = value;

	temp->left = temp->right = NULL;
	return temp;
}

/**
 * @brief Performs an in-order traversal of the binary tree of routes.
 *
 * This function recursively traverses the binary tree in in-order fashion
 * (left subtree, root, right subtree) and prints the key-value pairs of each node.
 *
 * @param root A pointer to the root node of the binary tree.
 */
void inorder(struct Route* root)
{

    if (root != NULL) {
        inorder(root->left);
        printf("%s -> %s \n", root->key, root->value);
        inorder(root->right);
    }
}

/**
 * @brief Adds a new route to the binary search tree of routes.
 *
 * This function inserts a new route into the binary search tree based on the provided key.
 * If a route with the same key already exists, a warning message is printed.
 *
 * @param root Pointer to the root of the binary search tree of routes.
 * @param key The key associated with the route to be added.
 * @param value The value associated with the route to be added.
 * @return Pointer to the root of the binary search tree after the new route has been added.
 */
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

/**
 * @brief Searches for a route in the binary search tree.
 *
 * This function searches for a route in the binary search tree based on the given key.
 * It performs a recursive search, comparing the key with the current node's key and
 * traversing the left or right subtree accordingly.
 *
 * @param root Pointer to the root node of the binary search tree.
 * @param key The key to search for in the binary search tree.
 * @return Pointer to the node containing the key if found, otherwise NULL.
 */
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

/**
 * @brief Initializes the routes for the web server.
 *
 * This function creates the root of the route tree and adds the initial routes
 * to it. The routes added are:
 * - "/" which maps to "index.html"
 * - "/about" which maps to "about.html"
 *
 * @return A pointer to the root of the route tree.
 */
struct Route *initialize_routes() {
    struct Route *root = NULL;
    root = addRoute(root, "/", "index.html");
    root = addRoute(root, "/about", "about.html");
    return root;
}

/**
 * @brief Deletes a route from the binary search tree and removes the associated file.
 *
 * This function deletes a route node from the binary search tree based on the given key.
 * If the node is found, it also deletes the associated HTML file from the "templates" directory.
 *
 * @param root Pointer to the root of the binary search tree.
 * @param key The key of the route to be deleted.
 * @return Pointer to the new root of the binary search tree after deletion.
 */
struct Route *deleteRoute(struct Route *root, char* key) {
    if (root == NULL) return NULL;

    if (strcmp(key, root->key) < 0) {
        root->left = deleteRoute(root->left, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = deleteRoute(root->right, key);
    } else {
        // Hapus file terkait rute
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "templates/%s.html", root->value);

        if (remove(filepath) == 0) {
            printf("File %s deleted successfully.\n", filepath);
        } else {
            perror("Error deleting file");
        }

        // Hapus node
        if (root->left == NULL) {
            struct Route *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Route *temp = root->left;
            free(root);
            return temp;
        }

        // Cari pengganti node yang dihapus
        struct Route *temp = root->right;
        while (temp && temp->left != NULL) temp = temp->left;

        root->key = temp->key;
        root->value = temp->value;
        root->right = deleteRoute(root->right, temp->key);
    }
    return root;
}

/**
 * @brief Recursively traverses and prints the routes in a binary search tree.
 *
 * This function performs an in-order traversal of the binary search tree
 * starting from the given root node. It prints each route in the format
 * "Route: key -> value".
 *
 * @param root A pointer to the root node of the binary search tree.
 */
void showRoutes(struct Route *root) {
    if (root == NULL) {
        return;
    }

    // Traverse left subtree
    showRoutes(root->left);

    // Print current route
    printf("Route: %s -> %s\n", root->key, root->value);

    // Traverse right subtree
    showRoutes(root->right);
}
