#ifndef RRT_H
#define RRT_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QProgressBar>
#include "point.h"
#include "common.h"
#include "common_func.h"
#include "tree.h"
#include "painter_widget.h"
#include "node.h"

QPair<double, double> RRT(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step);

#endif // RRT_H
