// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "linkedlist.h"
#include "database.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsItemAnimation>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_addButton_clicked();
    void on_addBeforeButton_clicked();
    void on_addAfterButton_clicked();
    void on_removeButton_clicked();
    void on_accessButton_clicked();
    void on_cleanButton_clicked();
    void on_sizeCapacityButton_clicked();
    void on_copyButton_clicked();

private:
    // Linked List and Database
    LinkedList<int> *list_;
    Database *db_;

    // GUI components
    QWidget *centralWidget_;
    QGraphicsView *graphicsView_;
    QGraphicsScene *scene_;
    //QPushButton *addButton_;
    QPushButton *addBeforeButton_;
    QPushButton *addAfterButton_;
    QPushButton *removeButton_;
    QPushButton *accessButton_;
    QPushButton *cleanButton_;
    QPushButton *sizeCapacityButton_;
    QPushButton *copyButton_;
    QLineEdit *nodeInput_;
    QLineEdit *indexInput_;
    QLabel *statusLabel_;
    QLabel *titleLabel_;
    QVector<QGraphicsEllipseItem*> ellipses_;  // Store pointers to ellipse items

    // Layouts
    QVBoxLayout *mainLayout_;
    QHBoxLayout *buttonLayout_;
    QHBoxLayout *inputLayout_;

    void updateVisualization();
    void updateStatus(const QString &message);
    Node<int>* getNodeAtIndex(int index);
    void animateNode(QGraphicsEllipseItem *node);
    void highlightNode(QGraphicsEllipseItem *node);
    void fadeOutNode(QGraphicsEllipseItem *node);

};

#endif // MAINWINDOW_H
