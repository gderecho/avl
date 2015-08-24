#ifndef RB_TREE_H_
#define RB_TREE_H_

struct rb_tree {
    struct node *head;
};


int rb_init(struct rb_tree*);
int rb_insert(int, struct rb_tree*);
int rb_free_elements(struct rb_tree*);

#endif /* RB_TREE_H_ */
