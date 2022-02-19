#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include "avl.h"
#include "decart.h"
#include "twothree.h"
#include "redblack.h"
#include <random>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* Scene;
    QGraphicsView* View;
    AVL3 avl_tree;
    Decart3 decart_tree;
    TwoThree3 twothree_tree;
    RedBlack3 redblack_tree;

private slots:
    void on_Val_editingFinished();

    void on_AddButton_clicked();

    void on_DeleteButton_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
