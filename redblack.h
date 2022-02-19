#ifndef REDBLACK_H
#define REDBLACK_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

enum COLOR {RED, BLACK};

class Node {
public:
  int val;
  COLOR color;
  Node *left, *right, *parent;

  Node(int val) : val(val) {
    parent = left = right = nullptr;
    color = RED;
  }

  Node *uncle();

  bool isOnLeft();

  Node *sibling();

  void moveDown(Node *nParent);

  bool hasRedChild();
};

struct RBTree {
  Node *root;

  void leftRotate(Node *x);

  void rightRotate(Node *x);

  void swapColors(Node *x1, Node *x2);

  void swapValues(Node *u, Node *v);

  void fixRedRed(Node *x);

  Node *successor(Node *x);

  Node *BSTreplace(Node *x);

  void deleteNode(Node *v);

  void fixDoubleBlack(Node *x);

  RBTree();

  Node *getRoot() {return root;}

  Node *search(int n);

  void insert(int n);

  void deleteByVal(int n);

};

class RedBlack3
{
public:
    RedBlack3();

    void Draw(QGraphicsScene* Scene, Node* cur, int x, int y, int x_prev, int y_prev);

    RBTree* rbroot;

};

#endif // REDBLACK_H
