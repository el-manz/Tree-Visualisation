#ifndef DECART_H
#define DECART_H
#include <random>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>

struct Decart {
    int key, y, sz;
    long long sum;
    Decart *left, *right;

    Decart() {}
    Decart(int key) : key(key), y(rand()), sz(1), sum(key), left(nullptr), right(nullptr) {}

};

class Decart3
{
public:
    Decart3();

    int get_sz(Decart *T);

    long long get_sum(Decart *T);

    void recalc(Decart *T);

    Decart *merge(Decart *L, Decart *R);

    std::pair<Decart*, Decart*> split(Decart *T, int key);

    Decart* root = nullptr;

    bool find(Decart *T, int key);

    void add(int key);

    void del(int key);

    void Draw(QGraphicsScene* Scene, Decart* cur, int x, int y, int x_prev, int y_prev);

    Decart* Decart_root = nullptr;
};

#endif // DECART_H
