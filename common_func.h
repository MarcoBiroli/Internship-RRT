#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QRandomGenerator>
#include <QtMath>
#include "point.h"
#include "painter_widget.h"
#include "robot.h"
#include "tree.h"

QPair<double, QVector<Point>> connect(painter_widget *paintArea, QPair<int, Point> A, QPair<int, Point> B, Tree *T1,
               Tree *T2, Robot robot, QVector<QRect> obstacles);

#endif // COMMON_FUNC_H
