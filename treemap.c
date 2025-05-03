#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key) != NULL) return;
    TreeNode* nodoPadre = NULL;
    TreeNode* current = tree->root;

    while (current != NULL) {
        nodoPadre = current;
        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    TreeNode* new = createTreeNode(key, value);
    new->parent = nodoPadre;

    if (nodoPadre == NULL)
        tree->root = new;
    else if (tree->lower_than(key, nodoPadre->pair->key))
        nodoPadre->left = new;
    else
        nodoPadre->right = new;

    tree->current = new;
}

TreeNode * minimum(TreeNode * x) {
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void cambiaNodo(TreeMap* tree, TreeNode* node, TreeNode* new) {
    if (node->parent != NULL) {
        if (node == node->parent->left)
            node->parent->left = new;
        else
            node->parent->right = new;
    } else {
        tree->root = new;
    }
    if (new != NULL)
        new->parent = node->parent;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        cambiaNodo(tree, node, NULL);
        free(node);
        return;
    }

    if (node->left == NULL && node->right != NULL) {
        cambiaNodo(tree, node, node->right);
        free(node);
        return;
    }

    if (node->left != NULL && node->right == NULL) {
        cambiaNodo(tree, node, node->left);
        free(node);
        return;
    }
    
    TreeNode* sucesor = minimum(node->right);
    node->pair = sucesor->pair;
    removeNode(tree, sucesor);
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* temp = tree->root;
    while(temp != NULL) {
        if(is_equal(tree, key, temp->pair->key)) {
            tree->current = temp;
            return temp->pair;
        } 
        else if(tree->lower_than(key, temp->pair->key)) {
            temp = temp->left;
        } 
        else {
            temp = temp->right;
        }
    }
    return NULL;
}

Pair* upperBound(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* nodoUpper = NULL;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            nodoUpper = current;
            current = current->left;
        } 
        else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } 
        else {
            return current->pair;
        }
    }

    if (nodoUpper != NULL)
        return nodoUpper->pair;
    else
        return NULL; 
}


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL) return NULL;
    
    TreeNode* new = minimum(tree->root);
    tree->current = new;
     return new->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
        if (tree->current == NULL) return NULL;
        
        TreeNode* aux = tree->current;
        if (aux->right != NULL) {
            tree->current = minimum(aux->right);
            return tree->current->pair;
        }

        TreeNode* nodoPadre = aux->parent;
        while (nodoPadre != NULL && aux == nodoPadre->right) {
            aux = nodoPadre;
            nodoPadre = nodoPadre->parent;
        }
    
        tree->current = nodoPadre;
        if (tree->current != NULL)
            return tree->current->pair;
        else
            return NULL;  
}
