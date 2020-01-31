#ifndef ROBOT_H
#define ROBOT_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtMath>
#include <QtDebug>
#include <QVector>
#include <cmath>
#include "point.h"

class Robot
{
public:
    Robot();
    Robot(double si, double Li, double max_anglei);
    QPair<QPair<QString, QVector<double>>, double> min_dist(Point node0, Point node1);
    QVector<Point> gen_L(Point start, double delta_theta, int nb_points);
    QVector<Point> gen_R(Point start, double delta_theta, int nb_points);
    QVector<Point> gen_S(Point start, double delta_d);
    double s = 1;
    double L = 1;
    double max_angle = M_PI/2;
    double rho = 1;

private:
    double dist(QPointF node1, QPointF node2);
    QPair<QVector<double>, double> min_LSL(Point node0, Point node1);
    QPair<QVector<double>, double> min_LSR(Point node0, Point node1);
    QPair<QVector<double>, double> min_RSR(Point node0, Point node1);
    QPair<QVector<double>, double> min_RSL(Point node0, Point node1);
    QPair<QVector<double>, double> min_LRL(Point node0, Point node1);
    QPair<QVector<double>, double> min_RLR(Point node0, Point node1);
};

#endif // ROBOT_H
