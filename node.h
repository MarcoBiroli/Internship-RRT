#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include "point.h"

class Node
{
public:
    Node(double x, double y, double theta, Node *p);
    Node(double x, double y, Node *p);
    Node(Point point, Node *p);
    Node(Point point);
    Node();
    Node *parent;
    Point point;
};

#endif // NODE_H
