#ifndef PATH_H
#define PATH_H


#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QMap>
#include "point.h"

class Path
{
public:
    Path();
    Path(Point start, Point end, double rho);
    Path(Point start, Point end, QString type, double alpha, double beta, double gamma, double rho);
    Path(Point start, Point end, QString type, QVector<double> values, double rho);
    void draw(QPainter *painter);

private:
    QString type = "";
    Point start = Point();
    Point end = Point();
    double rho = 1;
    double alpha = 0;
    double beta = 0;
    double gamma = 0;
};

#endif // PATH_H
