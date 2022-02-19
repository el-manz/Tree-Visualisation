#ifndef AVL_H
#define AVL_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

struct AVL {
    int key;
    int height;
    AVL* left;
    AVL* right;
    int sz = 0;
    AVL(int k) {
        key = k;
        left = right = nullptr;
        height = 1;
        sz = 0;
    }
};

class AVL3
{
public:
    AVL3();

    int height(AVL* p);

    int bfactor(AVL* p);

    void recalc(AVL* &p);

    AVL* rotateright(AVL* p);

    AVL* rotateleft(AVL* q);

    AVL* balance(AVL* p);

    AVL* insert(AVL* p, int k);

    AVL* findmin(AVL* p);

    AVL* removemin(AVL* p);

    AVL* remove(AVL* p, int k);

    void sz_recalc(AVL* p);

    void Draw(QGraphicsScene* Scene, AVL* cur, int x, int y, int x_prev, int y_prev);

    AVL* root = nullptr;

private:

};

#endif // AVL_H
