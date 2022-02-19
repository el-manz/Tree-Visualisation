#include "decart.h"

Decart3::Decart3()
{

}

int Decart3::get_sz(Decart *T) {
    if (T == nullptr) return 0;
    return T->sz;
}

long long Decart3::get_sum(Decart *T) {
    if (T == nullptr) return 0;
    return T->sum;
}

void Decart3::recalc(Decart *T) {
    if (T == nullptr) return;
    T->sz = get_sz(T->left) + get_sz(T->right) + 1;
    T->sum = get_sum(T->left) + get_sum(T->right) + T->key;
}

Decart* Decart3::merge(Decart *L, Decart *R) {
    if (L == nullptr) return R;
    if (R == nullptr) return L;
    if (L->y <= R->y) {
        L->right = merge(L->right, R);
        recalc(L);
        return L;
    }
    R->left = merge(L, R->left);
    recalc(R);
    return R;
}

std::pair<Decart*, Decart*> Decart3::split(Decart *T, int key) {
    if (T == nullptr) return { nullptr, nullptr };
    if (T->key <= key) {
        auto tmp = split(T->right, key);
        // tmp.first.x <= key
        // tmp.second.x > key
        T->right = tmp.first;
        recalc(T);
        return { T, tmp.second };
    }
    auto tmp = split(T->left, key);
    // tmp.first.x <= key
    // tmp.second.x > key
    T->left = tmp.second;
    recalc(T);
    return { tmp.first, T };
}

bool Decart3::find(Decart *T, int key) {
    if (T == nullptr)
        return 0;
    if (T->key == key)
        return true;
    if (T->key < key)
        return find(T->right, key);
    return find(T->left, key);
}

void Decart3::add(int key) {
    if (find(Decart_root, key)) return;
    Decart* v = new Decart(key);
    auto tmp = split(Decart_root, key);
    // tmp.first.x <= key
    // tmp.second.x > key
    Decart_root = merge(tmp.first, merge(v, tmp.second));
}

void Decart3::del(int key) {
    auto tmp = split(Decart_root, key);
    // tmp.first <= key
    // tmp.second > key
    auto tmp2 = split(tmp.first, key - 1);
    // tmp2.first <= key - 1
    // tmp2.second == key
    Decart_root = merge(tmp2.first, tmp.second);
}

const int Ellipse_width = 40, Ellipse_height = 30;
const int x_dist = 10, y_dist = 20;

void Decart3::Draw(QGraphicsScene* Scene, Decart* cur, int x, int y, int x_prev, int y_prev) {
    if (cur == nullptr) {
        /*Scene->addEllipse(x_start + drawn * (Ellipse_width + 10), y_start + drawn * (Ellipse_height + 10), Ellipse_width, Ellipse_height);
        QString s = "NULL";
        auto text = Scene->addText(s);
        text->setTextWidth(100);
        text->setPos(x_start + drawn * (Ellipse_width + 10) + Ellipse_width / 4, y_start + drawn * (Ellipse_height + 10));*/
        return;
    }

    Draw(Scene, cur->left, x - (Ellipse_width + x_dist) * (cur->left?(cur->left->sz)-1:0) / 2 - Ellipse_width - x_dist / 2, y + (Ellipse_height + y_dist), x, y);
    Draw(Scene, cur->right, x + (Ellipse_width + x_dist) * (cur->right?(cur->right->sz)-1:0) / 2 + Ellipse_width + x_dist / 2, y + (Ellipse_height + y_dist), x, y);

    if (x_prev != -1 && y_prev != -1) {
        Scene->addLine(QLineF(x_prev + Ellipse_width / 2, y_prev + Ellipse_height, x + Ellipse_width / 2, y));
    }
    Scene->addEllipse(x, y, Ellipse_width, Ellipse_height);
    QString s = QString::number(cur->key);
    auto text = Scene->addText(s);
    text->setTextWidth(100);
    text->setPos(x + Ellipse_width / 4, y);

}
