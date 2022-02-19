#include "avl.h"

AVL3::AVL3()
{

}

int AVL3::height(AVL* p) {
    return p?p->height:0;
}

int AVL3::bfactor(AVL* p) {
    return height(p->right) - height(p->left);
}

void AVL3::recalc(AVL* &p) {
    int h_left = height(p->left);
    int h_right = height(p->right);
    p->height = fmax(h_left, h_right) + 1;
}

AVL* AVL3::rotateright(AVL* p) {
    AVL* q = p->left;
    p->left = q->right;
    q->right = p;
    recalc(p);
    recalc(q);
    sz_recalc(p);
    sz_recalc(q);
    return q;
}

AVL* AVL3::rotateleft(AVL* q) {
    AVL* p = q->right;
    q->right = p->left;
    p->left = q;
    recalc(q);
    recalc(p);
    sz_recalc(p);
    sz_recalc(q);
    return p;
}

AVL* AVL3::balance(AVL* p) // балансировка узла p
{
    recalc(p);
    if(bfactor(p) == 2)
    {
        if(bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if(bfactor(p) == -2)
    {
        if(bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}

AVL* AVL3::insert(AVL* p, int k) // вставка ключа k в дерево с корнем p
{
    if(p == nullptr) {
        // Scene->addEllipse(300, 300, 30, 10);
        // system("pause");
        return new AVL(k);
    }
    if(k < p->key)
        p->left = insert(p->left,k);
    else
        p->right = insert(p->right,k);
    sz_recalc(p);
    return balance(p);
}

AVL* AVL3::findmin(AVL* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}

AVL* AVL3::removemin(AVL* p) // удаление узла с минимальным ключом из дерева p
{
    if(p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

AVL* AVL3::remove(AVL* p, int k) // удаление ключа k из дерева p
{
    if(!p) return 0;
    if(k < p->key)
        p->left = remove(p->left,k);
    else if(k > p->key)
        p->right = remove(p->right,k);
    else //  k == p->key
    {
        AVL* q = p->left;
        AVL* r = p->right;
        delete p;
        if(!r) return q;
        AVL* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    sz_recalc(p);
    return balance(p);
}

void AVL3::sz_recalc(AVL* p) {
    if (p == nullptr)
        return;
    if (p->left == nullptr && p->right == nullptr) {
        p->sz = 0;
        return;
    }
    if (p->left == nullptr) {
        sz_recalc(p->right);
        p->sz = p->right->sz + 1;
        return;
    }
    if (p->right == nullptr) {
        sz_recalc(p->left);
        p->sz = p->left->sz + 1;
        return;
    }

    sz_recalc(p->left);
    sz_recalc(p->right);
    p->sz = p->left->sz + p->right->sz + 2;
}

const int Ellipse_width = 40, Ellipse_height = 30;
const int x_dist = 10, y_dist = 20;

void AVL3::Draw(QGraphicsScene* Scene, AVL* cur, int x, int y, int x_prev, int y_prev) {
    if (cur == nullptr) {
        return;
    }

    Draw(Scene, cur->left, x - (Ellipse_width + x_dist) * (cur->left?cur->left->sz:0) / 2 - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
    Draw(Scene, cur->right, x + (Ellipse_width + x_dist) * (cur->right?cur->right->sz:0) / 2 + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);

    if (x_prev != -1 && y_prev != -1) {
        Scene->addLine(QLineF(x_prev + Ellipse_width / 2, y_prev + Ellipse_height, x + Ellipse_width / 2, y));
    }
    Scene->addEllipse(x, y, Ellipse_width, Ellipse_height);
    QString s = QString::number(cur->key);
    auto text = Scene->addText(s);
    text->setTextWidth(100);
    text->setPos(x + Ellipse_width / 4, y);

}
