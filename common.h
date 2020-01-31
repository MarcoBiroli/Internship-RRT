#ifndef COMMON_H
#define COMMON_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QRandomGenerator>
#include <QtMath>
#include "point.h"

Point lines_collided(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y);
Point rand_point(double width, double height);
Point build(Point start, Point end, double step);
bool is_valid(QWidget *paintArea,  Point pp, Point np, QVector<QRect> obstacles);
bool is_path_valid(QWidget *paintArea, QVector<Point> path, QVector<QRect> obstacles);

#endif // COMMON_H
