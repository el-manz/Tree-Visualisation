#include "redblack.h"
#include <queue>

RedBlack3::RedBlack3()
{

}

Node* Node::uncle() {
  // If no parent or grandparent, then no uncle
  if (parent == nullptr or parent->parent == nullptr)
    return nullptr;

  if (parent->isOnLeft())
    // uncle on right
    return parent->parent->right;
  else
    // uncle on left
    return parent->parent->left;
}

bool Node::isOnLeft() { return this == parent->left; }

Node* Node::sibling() {
  // sibling null if no parent
  if (parent == nullptr)
    return nullptr;

  if (isOnLeft())
    return parent->right;

  return parent->left;
}

void Node::moveDown(Node *nParent) {
  if (parent != nullptr) {
    if (isOnLeft()) {
      parent->left = nParent;
    } else {
      parent->right = nParent;
    }
  }
  nParent->parent = parent;
  parent = nParent;
}

bool Node::hasRedChild() {
    return (left != NULL and left->color == RED) or
           (right != NULL and right->color == RED);
}

void RBTree::leftRotate(Node *x) {
  // new parent will be node's right child
  Node *nParent = x->right;

  // update root if current node is root
  if (x == root)
    root = nParent;

  x->moveDown(nParent);

  // connect x with new parent's left element
  x->right = nParent->left;
  // connect new parent's left element with node
  // if it is not null
  if (nParent->left != nullptr)
    nParent->left->parent = x;

  // connect new parent with x
  nParent->left = x;
}

void RBTree::rightRotate(Node *x) {
  // new parent will be node's left child
  Node *nParent = x->left;

  // update root if current node is root
  if (x == root)
    root = nParent;

  x->moveDown(nParent);

  // connect x with new parent's right element
  x->left = nParent->right;
  // connect new parent's right element with node
  // if it is not null
  if (nParent->right != nullptr)
    nParent->right->parent = x;

  // connect new parent with x
  nParent->right = x;
}

void RBTree::swapColors(Node *x1, Node *x2) {
  COLOR temp;
  temp = x1->color;
  x1->color = x2->color;
  x2->color = temp;
}

void RBTree::swapValues(Node *u, Node *v) {
  int temp;
  temp = u->val;
  u->val = v->val;
  v->val = temp;
}

void RBTree::fixRedRed(Node *x) {
  // if x is root color it black and return
  if (x == root) {
    x->color = BLACK;
    return;
  }

  // initialize parent, grandparent, uncle
  Node *parent = x->parent, *grandparent = parent->parent,
       *uncle = x->uncle();

  if (parent->color != BLACK) {
    if (uncle != nullptr && uncle->color == RED) {
      // uncle red, perform recoloring and recurse
      parent->color = BLACK;
      uncle->color = BLACK;
      grandparent->color = RED;
      fixRedRed(grandparent);
    } else {
      // Else perform LR, LL, RL, RR
      if (parent->isOnLeft()) {
        if (x->isOnLeft()) {
          // for left right
          swapColors(parent, grandparent);
        } else {
          leftRotate(parent);
          swapColors(x, grandparent);
        }
        // for left left and left right
        rightRotate(grandparent);
      } else {
        if (x->isOnLeft()) {
          // for right left
          rightRotate(parent);
          swapColors(x, grandparent);
        } else {
          swapColors(parent, grandparent);
        }

        // for right right and right left
        leftRotate(grandparent);
      }
    }
  }
}

Node* RBTree::successor(Node *x) {
  Node *temp = x;

  while (temp->left != nullptr)
    temp = temp->left;

  return temp;
}

Node* RBTree::BSTreplace(Node *x) {
  // when node have 2 children
  if (x->left != nullptr and x->right != nullptr)
    return successor(x->right);

  // when leaf
  if (x->left == nullptr and x->right == nullptr)
    return nullptr;

  // when single child
  if (x->left != nullptr)
    return x->left;
  else
    return x->right;
}

void RBTree::deleteNode(Node *v) {
  Node *u = BSTreplace(v);

  // True when u and v are both black
  bool uvBlack = ((u == nullptr or u->color == BLACK) and (v->color == BLACK));
  Node *parent = v->parent;

  if (u == nullptr) {
    // u is NULL therefore v is leaf
    if (v == root) {
      // v is root, making root null
      root = nullptr;
    } else {
      if (uvBlack) {
        // u and v both black
        // v is leaf, fix double black at v
        fixDoubleBlack(v);
      } else {
        // u or v is red
        if (v->sibling() != nullptr)
          // sibling is not null, make it red"
          v->sibling()->color = RED;
      }

      // delete v from the tree
      if (v->isOnLeft()) {
        parent->left = nullptr;
      } else {
        parent->right = nullptr;
      }
    }
    delete v;
    return;
  }

  if (v->left == nullptr or v->right == nullptr) {
    // v has 1 child
    if (v == root) {
      // v is root, assign the value of u to v, and delete u
      v->val = u->val;
      v->left = v->right = nullptr;
      delete u;
    } else {
      // Detach v from tree and move u up
      if (v->isOnLeft()) {
        parent->left = u;
      } else {
        parent->right = u;
      }
      delete v;
      u->parent = parent;
      if (uvBlack) {
        // u and v both black, fix double black at u
        fixDoubleBlack(u);
      } else {
        // u or v red, color u black
        u->color = BLACK;
      }
    }
    return;
  }

  // v has 2 children, swap values with successor and recurse
  swapValues(u, v);
  deleteNode(u);
}

void RBTree::fixDoubleBlack(Node *x) {
  if (x == root)
    // Reached root
    return;

  Node *sibling = x->sibling(), *parent = x->parent;
  if (sibling == nullptr) {
    // No sibiling, double black pushed up
    fixDoubleBlack(parent);
  } else {
    if (sibling->color == RED) {
      // Sibling red
      parent->color = RED;
      sibling->color = BLACK;
      if (sibling->isOnLeft()) {
        // left case
        rightRotate(parent);
      } else {
        // right case
        leftRotate(parent);
      }
      fixDoubleBlack(x);
    } else {
      // Sibling black
      if (sibling->hasRedChild()) {
        // at least 1 red children
        if (sibling->left != nullptr and sibling->left->color == RED) {
          if (sibling->isOnLeft()) {
            // left left
            sibling->left->color = sibling->color;
            sibling->color = parent->color;
            rightRotate(parent);
          } else {
            // right left
            sibling->left->color = parent->color;
            rightRotate(sibling);
            leftRotate(parent);
          }
        } else {
          if (sibling->isOnLeft()) {
            // left right
            sibling->right->color = parent->color;
            leftRotate(sibling);
            rightRotate(parent);
          } else {
            // right right
            sibling->right->color = sibling->color;
            sibling->color = parent->color;
            leftRotate(parent);
          }
        }
        parent->color = BLACK;
      } else {
        // 2 black children
        sibling->color = RED;
        if (parent->color == BLACK)
          fixDoubleBlack(parent);
        else
          parent->color = BLACK;
      }
    }
  }
}

Node* RBTree::search(int n) {
    Node *temp = root;
    while (temp != nullptr) {
      if (n < temp->val) {
        if (temp->left == nullptr)
          break;
        else
          temp = temp->left;
      } else if (n == temp->val) {
        break;
      } else {
        if (temp->right == nullptr)
          break;
        else
          temp = temp->right;
      }
    }

    return temp;
}

void RBTree::insert(int n) {
  Node *newNode = new Node(n);
  if (root == nullptr) {
    newNode->color = BLACK;
    root = newNode;
  } else {
    Node *temp = search(n);

    if (temp->val == n) {
      return;
    }

    newNode->parent = temp;

    if (n < temp->val)
      temp->left = newNode;
    else
      temp->right = newNode;

    fixRedRed(newNode);
  }
}

void RBTree::deleteByVal(int n) {
  if (root == nullptr)
    return;

  Node *v = search(n), *u;

  if (v->val != n) {
    return;
  }

  deleteNode(v);
}


const int Ellipse_width = 40, Ellipse_height = 30;
const int x_dist = 10, y_dist = 20;

void RedBlack3::Draw(QGraphicsScene* Scene, Node* cur, int x, int y, int x_prev, int y_prev) {
    if (cur == nullptr) {
        /* Scene->addEllipse(x_start + drawn * (Ellipse_width + 10), y_start + drawn * (Ellipse_height + 10), Ellipse_width, Ellipse_height);
        QString s = "NULL";
        auto text = Scene->addText(s);
        text->setTextWidth(100);
        text->setPos(x_start + drawn * (Ellipse_width + 10) + Ellipse_width / 4, y_start + drawn * (Ellipse_height + 10)); */
        return;
    }

    Draw(Scene, cur->left, x - (Ellipse_width + x_dist / 2) - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
    Draw(Scene, cur->right, x + (Ellipse_width + x_dist / 2) + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);

    if (x_prev != -1 && y_prev != -1) {
        Scene->addLine(QLineF(x_prev + Ellipse_width / 2, y_prev + Ellipse_height, x + Ellipse_width / 2, y));
    }
    auto el = Scene->addEllipse(x, y, Ellipse_width, Ellipse_height);
    if (cur->color == RED) {
        el->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    }
    QString s = QString::number(cur->val);
    auto text = Scene->addText(s);
    text->setTextWidth(100);
    text->setPos(x + Ellipse_width / 4, y);

}

