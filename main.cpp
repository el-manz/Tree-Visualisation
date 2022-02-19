#include "mainwindow.h"
#include "avl.h"
#include "decart.h"
#include "twothree.h"
#include "ui_mainwindow.h"

#include <QApplication>

const int x_start = 300, y_start= 300;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // w.avl_tree.Draw(w.Scene, w.avl_tree.root, x_start, y_start, -1, -1);
    /* w.decart_tree.add(3);
    w.decart_tree.add(5);
    w.decart_tree.add(4);
    w.decart_tree.del(3);
    w.decart_tree.Draw(w.Scene, w.decart_tree.Decart_root, x_start, y_start, -1, -1); */

    // w.avl_tree.root = w.avl_tree.insert(w.Scene, w.avl_tree.root, 3);
    // w.avl_tree.Draw(w.Scene, w.avl_tree.root, x_start, y_start, -1, -1);

    /* w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 1);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 2);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 3);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 4);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 5);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 6);
    w.twothree_tree.root = w.twothree_tree.insert(w.twothree_tree.root, 7);
    w.twothree_tree.Draw(w.Scene, w.twothree_tree.root, x_start, y_start, -1, -1); */

    /* w.redblack_tree.rbroot->insert(3);
    w.redblack_tree.rbroot->insert(4);
    w.redblack_tree.rbroot->insert(5);
    w.redblack_tree.rbroot->insert(6);
    w.redblack_tree.Draw(w.Scene, w.redblack_tree.rbroot->root, x_start, y_start, -1, -1); */

    w.show();

    return a.exec();
}
