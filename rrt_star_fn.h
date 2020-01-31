#ifndef RRT_START_FN_H
#define RRT_START_FN_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QVector>
#include <QGridLayout>
#include <QProgressBar>
#include "common.h"
#include "common_func.h"
#include "tree.h"
#include "painter_widget.h"
#include "node.h"

QPair<double, double> RRT_Star_FN(painter_widget *paintArea, QProgressBar *progress_bar, int max_iter, bool dynamic_drawing, bool stop_when_found, double step, int M);


#endif // RRT_START_FN_H
