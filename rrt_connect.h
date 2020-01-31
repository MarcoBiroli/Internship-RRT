#ifndef RRT_CONNECT_H
#define RRT_CONNECT_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QProgressBar>
#include "common.h"
#include "common_func.h"
#include "tree.h"
#include "painter_widget.h"
#include "node.h"

QPair<double, double> RRT_Connect(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step);

#endif // RRT_CONNECT_H
