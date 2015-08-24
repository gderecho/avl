/* rb_tree.c
 * contains the implementation
 * of the red-black tree functions.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "rb_tree.h"

typedef enum {false, true} bool;
/* for the rotate methods */
#define RIGHT true
#define LEFT false

struct node {
    bool is_red;
    int value;
    struct node *left_child;
    struct node *right_child;
};

/* helper functions */
struct node *rb_insert_at(int,struct node*);
int rb_free_elements_at(struct node*);
struct node *new_node(int,bool);
struct node *get_other_child(struct node *, struct node*);
int node_swap_values(struct node*, struct node*);
struct node *rotate(struct node*, bool direction);

/* constructs a new node, which later must be freed */
struct node *new_node(int value, bool is_red)
{
    struct node *retval = malloc(sizeof(*retval));
    if(!retval)
        return NULL;
    retval->is_red = is_red;
    retval->left_child = NULL;
    retval->right_child = NULL;
    retval->value = value;
    return retval;
}

/* if child1 is not a child of the parent node,
 * the method will return the left child */
struct node *get_other_child(struct node *parent, struct node *child1)
{
    if(parent->left_child == child1)
        return parent->right_child;
    return parent->left_child;
}

int node_swap_values(struct node *node1, struct node *node2)
{
    node1->value = node1->value ^ node2->value;
    node2->value = node1->value ^ node2->value;
    node1->value = node1->value ^ node2->value;
    return EXIT_SUCCESS;
}

struct node *rotate(struct node *root, bool direction)
{
    if(direction == RIGHT) {
        struct node *prev_left;
        prev_left = root->left_child;
        root->left_child = prev_left->right_child;
        prev_left->right_child = root;
        return prev_left;
    } else {
        struct node *prev_right;
        prev_right = root->right_child;
        root->right_child = prev_right->left_child;
        prev_right->left_child = root;
        return prev_right;
    }
    return NULL;
}

/* initializes the tree, then returns
 * EXIT_SUCCESS */
int rb_init(struct rb_tree *tree)
{
    tree->head=NULL;
    return EXIT_SUCCESS;
}


/* attemps to insert a value into the
 * tree. returns EXIT_SUCCESS on success
 * and EXIT_FAILURE on failure. */
int rb_insert(int value, struct rb_tree *tree)
{
    if(!tree->head) {
        struct node *to_insert;
        if( !(to_insert = new_node(value,false)) )
            return EXIT_FAILURE;
        tree->head = to_insert;

        return EXIT_SUCCESS;
    }
    tree->head = rb_insert_at(value, tree->head);
    tree->head->is_red = false;
    return EXIT_SUCCESS;
}

/* helper method for rb_insert
 * attempts to insert as a child
 * of the current node 
 * returns parent 
 *
 * algorithm from Data Structures:
 * Abstraction and Design using Java
 * 2e, Koffman & Wolfgang*/
struct node *rb_insert_at(int value, struct node *current)
{
    if(value <= current->value) {
        if(current->left_child) {
            if(current->left_child->is_red && 
                    current->right_child &&
                    current->right_child->is_red) {
                current->left_child->is_red = false;
                current->right_child->is_red = false;
                current->is_red = true;
            }
            current->left_child = rb_insert_at(value, current->left_child);
            if(current->left_child->is_red) {
                if(current->left_child->left_child &&
                        current->left_child->left_child->is_red) {
                    current->left_child->is_red = false;
                    current->is_red = true;
                    current = rotate(current,RIGHT);
                } else if(current->left_child->right_child &&
                        current->left_child->right_child->is_red) {
                    current->left_child = rotate(current->left_child,LEFT);
                    current->left_child->is_red = false;
                    current->is_red = true;
                    current = rotate(current,RIGHT);
                }
            }
        } else {
            current->left_child = new_node(value,true);
        }
    } else {
        if(current->right_child) {
            if(current->right_child->is_red && 
                    current->left_child &&
                    current->left_child->is_red) {
                current->right_child->is_red = false;
                current->left_child->is_red = false;
                current->is_red = true;
            }
            current->right_child = rb_insert_at(value, current->right_child);
            if(current->right_child->is_red) {
                if(current->right_child->right_child &&
                        current->right_child->right_child->is_red) {
                    current->right_child->is_red = false;
                    current->is_red = true;
                    current = rotate(current,LEFT);
                } else if(current->right_child->left_child &&
                        current->right_child->left_child->is_red) {
                    current->right_child = rotate(current->right_child,RIGHT);
                    current->right_child->is_red = false;
                    current->is_red = true;
                    current = rotate(current,LEFT);
                }
            }
        } else {
            current->right_child = new_node(value,true);
        }
    }

    return current;
}

int rb_free_elements(struct rb_tree *tree)
{
    return rb_free_elements_at(tree->head);
}

int rb_free_elements_at(struct node *current)
{
    if(!current)
        return EXIT_SUCCESS;
    if(rb_free_elements_at(current->left_child)==EXIT_SUCCESS
            && rb_free_elements_at(current->right_child)==EXIT_SUCCESS) {
        free(current);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void rb_print_helper(struct node *curr, int level)
{
    if(!curr) return;
    printf("level: %d value: %d color: %s\n",
            level, curr->value, curr->is_red?"red":"black");
    rb_print_helper(curr->left_child,level+1);
    rb_print_helper(curr->right_child,level+1);
}

void rb_print(struct rb_tree *tree)
{
    rb_print_helper(tree->head,0);
}

int main(int argc, char *argv[])
{
    struct rb_tree tree;
    rb_init(&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_insert(11,&tree);
    rb_insert(14,&tree);
    rb_insert(2,&tree);
    rb_insert(1,&tree);
    rb_insert(7,&tree);
    rb_insert(5,&tree);
    rb_insert(8,&tree);
    rb_insert(4,&tree);
    rb_print(&tree);
    rb_free_elements(&tree);
    return 0;
}
