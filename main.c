#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {
    LEFT,
    RIGHT
} Direction;

typedef struct knot knot;
struct knot {
    knot *leftSon;
    knot *rightSon;
    int8_t heightDifference;
    int32_t key;
    int value;
};


knot *balancing(knot *node, Direction direction) {
    assert(node != NULL);

    if (direction == RIGHT) {
        assert(node->rightSon != NULL);
        assert(node->rightSon->leftSon != NULL);

        node->rightSon->heightDifference--;
        node->heightDifference--;                           //Изменение высот
        knot *output = node->rightSon;
        knot *temp = node->rightSon->leftSon;
        node->rightSon->leftSon = node;
        node->rightSon = temp;
        return output;
    } else {
        assert(node->leftSon != NULL);
        assert(node->leftSon->rightSon != NULL);

        node->leftSon->heightDifference++;
        node->heightDifference++;                           //Изменение высот
        knot *output = node->leftSon;
        knot *temp = node->leftSon->rightSon;
        node->leftSon->rightSon = node;
        node->leftSon = temp;
        return output;
    }
}


knot *heightInfoFix(knot *node, int difference) {
    assert(node != NULL);
    node->heightDifference += difference;
    if (node->heightDifference == 2) {
        assert(node->rightSon != NULL);
        if (node->rightSon->heightDifference == -1) {        //Длинный поворот
            balancing(node->rightSon, LEFT);
            return balancing(node, RIGHT);
        } else {                                              //Короткий поворот
            return balancing(node, RIGHT);
        }
    } else if (node->heightDifference == -2) {
        assert(node->rightSon != NULL);
        if (node->leftSon->heightDifference == 1) {
            balancing(node->leftSon, RIGHT);
            return balancing(node, LEFT);
        } else {
            return balancing(node, LEFT);
        }
    }
}


knot *newKnot(knot *node, uint32_t key, int value) {
    assert(node != NULL);
    if (node->key < key) {
        if (node->rightSon == NULL) {
            knot *newKnot = (knot *) malloc(sizeof(knot));     //Создаем новый узел, проверяем наличие и вставляем
            newKnot->rightSon = NULL;
            newKnot->leftSon = NULL;
            newKnot->heightDifference = 0;
            newKnot->key = key;
            newKnot->value = value;
            node->rightSon = newKnot;
            return heightInfoFix(node, 1);
        } else { // node->rightSon != NULL
            int8_t old = node->rightSon->heightDifference;
            knot *temp = newKnot(node->rightSon, key, value);
            assert(temp != NULL);
            assert(temp->rightSon != NULL);
            if (old == temp->rightSon->heightDifference) {
                return temp;
            } else {
                return heightInfoFix(temp, 1);
            }
        }
    } else if (node->key > key) {
        if (node->leftSon == NULL) {
            knot *newKnot = NULL;
            newKnot = (knot *) malloc(sizeof(*newKnot));          //Создаем новый узел, проверяем наличие и вставляем
            newKnot->rightSon = NULL;
            newKnot->leftSon = NULL;
            newKnot->heightDifference = 0;
            newKnot->key = key;
            newKnot->value = value;
            node->rightSon = newKnot;
            return heightInfoFix(node, -1);
        } else { // node->leftSon != NULL
            int8_t old = node->leftSon->heightDifference;
            knot *temp = newKnot(node->leftSon, key, value);
            assert(temp != NULL);
            assert(temp->leftSon != NULL);
            if (old == temp->leftSon->heightDifference) {
                return temp;
            } else {
                return heightInfoFix(temp, -1);
            }
        }
    } else {
        return node;
    }
}


knot *search(knot *node, uint32_t key) {
    if (node->leftSon->key == key) {
        return node->leftSon;
    } else if (node->rightSon->key == key) {
        return node->rightSon;
    } else if (node->key < key) {
        if (node->rightSon == NULL) {
            return NULL;
        } else {
            return search(node->rightSon, key);
        }
    } else if (node->key > key) {
        if (node->leftSon = NULL) {
            return NULL;
        } else {
            return (search(node->leftSon, key));
        }
    } else return node;
}


knot *removeHeightFix(knot *tree, uint32_t key) {
    if (tree->key < key) {
        int old = tree->rightSon->heightDifference;
        knot *temp = removeHeightFix(tree->rightSon, key);
        if (old == temp->rightSon->key) {
            return temp;
        } else {
            return heightInfoFix(temp, -1);
        }
    } else if (tree->key > key) {
        int old = tree->leftSon->heightDifference;
        knot *temp = removeHeightFix(tree->leftSon, key);
        if (old == temp->leftSon->heightDifference) {
            return temp;
        } else {
            return heightInfoFix(temp, 1);
        }
    } else {
        tree->heightDifference++;
        return tree;
    }
}


knot *myRemove(knot *tree, uint32_t key) {
    knot *parentNode = tree;
    knot *node = NULL;
    if (tree->key == key || tree->rightSon == NULL) {
        knot *left = tree->leftSon;
        free(tree);
        return left;
    } else if (tree->key == key) {
        return tree;
    } else {
        node = search(tree, key);
    }
    if (node != NULL) {
        if (node->rightSon != NULL) {
            knot *minNode = node->rightSon;
            while (minNode->leftSon->leftSon != NULL) {
                minNode = minNode->leftSon;
            }
            knot *min = minNode->leftSon;
            tree = removeHeightFix(tree, minNode->key);
            minNode->leftSon = minNode->leftSon->rightSon;
            node->key = min->key;
            free(min);
        } else {
            knot *left = node->leftSon;
            free(node);
            if (parentNode->key < key) {
                parentNode->rightSon = left;
            } else {
                parentNode->leftSon = left;
            }
        }
    }
    return tree;
}


void removeTree(knot *tree) {
    if (tree->leftSon != NULL) {
        removeTree(tree->leftSon);
    }
    if (tree->rightSon != NULL) {
        removeTree(tree->rightSon);
        free(tree);
    } else {
        free(tree);
    }
}


knot *newTree(uint32_t key, int value) {
    knot *tree = (knot *) malloc(sizeof(*tree));
    tree->value = value;
    tree->rightSon = tree->leftSon = NULL;
    tree->heightDifference = 0;
    tree->key = key;
    return tree;
}


int maxDepth(knot *tree) {
    assert(tree != NULL);
    if (tree->heightDifference == 1) {
        return (maxDepth(tree->rightSon) + 1);
    } else if (tree->heightDifference == -1) {
        return (maxDepth(tree->leftSon) + 1);
    } else if (tree->rightSon == NULL) {
        return 1;
    } else {
        return (maxDepth(tree->leftSon) + 1);
    }
}


int elementDepth(knot *tree, uint32_t key) {
    assert(tree != NULL);
    if (tree->key < key) {
        return elementDepth(tree->rightSon, key) + 1;
    } else if (tree->key > key) {
        return elementDepth(tree->leftSon, key) + 1;
    } else {
        return 1;
    }
}


int keyValue(knot *tree, uint32_t key) {
    assert(tree != NULL);
    if (tree->key < key) {
        return elementDepth(tree->rightSon, key);
    } else if (tree->key > key) {
        return elementDepth(tree->leftSon, key);
    } else {
        return tree->value;
    }
}


int main() {
    double sum = 0;
    for (int j = 50; j < 999; j++) {
        srand(j);
        int a[1000];
        knot *tree = newTree(rand(), 0);
        for (int i = 0; i < 999; i++) {
            a[i] = rand();
            tree = newKnot(tree, a[i], i);
        }
        sum += maxDepth(tree);
        removeTree(tree);
    }
    printf("%f", sum / 950);
}
