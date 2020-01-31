#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paintArea = ui->drawing_widget;
    ng_select = ui->ng_comboBox;
    g_select = ui->g_comboBox;
    end_check = ui->end_criterion;
    progress_bar = ui->progressBar;
    dcheck = ui->Dynamic_Drawing;
    max_iter = ui->max_iterations;
    MBox = ui->MBox;
    step_box = ui->stepBox;
    scale_box = ui->scale;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_run_button_clicked()
{
    if(running){
        return;
    }
    paintArea->draw = true;
    paintArea->reset();
    QPair<double, double> results;
    if(ng_select->currentText() != ""){
        if (ng_select->currentText() == "RRT"){
            running = true;
            results = RRT_Dubin(paintArea, progress_bar, max_iter->value(), dcheck->isChecked(), end_check->isChecked(), step_box->value()/scale_box->value());
            running = false;
        }
        else if (ng_select->currentText() == "RRT Connect"){
            running = true;
            results = RRT_Connect_Dubin(paintArea, progress_bar, max_iter->value(), dcheck->isChecked(), end_check->isChecked(),step_box->value()/scale_box->value());
            running = false;
        }
        else if (ng_select->currentText() == "RRT*"){
            running = true;
            results = RRT_Star(paintArea, progress_bar, max_iter->value(), dcheck->isChecked(), end_check->isChecked(), step_box->value()/scale_box->value());
            running = false;
        }
        else if (ng_select->currentText() == "RRT* Connect"){
            running = true;
            results = RRT_Star_Connect(paintArea, progress_bar, max_iter->value(), dcheck->isChecked(), end_check->isChecked(), step_box->value()/scale_box->value());
            running = false;
        }
        else if (ng_select->currentText() == "RRT* FN"){
            running = true;
            results = RRT_Star_FN(paintArea, progress_bar, max_iter->value(), dcheck->isChecked(), end_check->isChecked(), step_box->value()/scale_box->value(), MBox->value());
            running = false;
        }
    }
    ui->lcdLength->display(results.first*ui->scale->value());
    ui->lcdTime->display(results.second);
    this->repaint();
}
