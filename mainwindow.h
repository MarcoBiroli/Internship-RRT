#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QComboBox>
#include "painter_widget.h"
#include "rrt.h"
#include "rrt_dubin.h"
#include "rrt_connect.h"
#include "rrt_connect_dubin.h"
#include "rrt_star.h"
#include "rrt_star_connect.h"
#include "rrt_star_fn.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int program_id = 0;
    ~MainWindow();

private slots:
    void on_run_button_clicked();

private:
    Ui::MainWindow *ui;
    painter_widget *paintArea;
    QComboBox *g_select;
    QComboBox *ng_select;
    QCheckBox *end_check;
    QAction *dcheck;
    QSpinBox *max_iter;
    QSpinBox *MBox;
    QDoubleSpinBox *step_box;
    QDoubleSpinBox *scale_box;
    QProgressBar *progress_bar;
    bool running = false;
    QVector<void (*)(QWidget *widget)> programs;
};

#endif // MAINWINDOW_H
