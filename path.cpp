#include "path.h"

Path::Path()
{

}

Path::Path(Point istart, Point iend, double irho){
    this->start = istart;
    this->end = iend;
    this->rho = irho;
}

Path::Path(Point istart, Point iend, QString itype, double ialpha, double ibeta, double igamma, double irho){
    this->start = istart;
    this->end = iend;
    this->type = itype;
    this->alpha = ialpha;
    this->beta = ibeta;
    this->gamma = igamma;
    this->rho = irho;
}

Path::Path(Point istart, Point iend, QString itype, QVector<double> ivalues, double irho){
    this->start = istart;
    this->end = iend;
    this->type = itype;
    this->alpha = ivalues[0];
    this->beta = ivalues[1];
    this->gamma = ivalues[2];
    this->rho = irho;
}

void Path::draw(QPainter *painter){
    if(this->type == "" || this->rho == 0){
        return;
    }
    if(this->type[0] == "L"){
        painter->drawArc(this->start.x(), this->start.y(), this->rho, this->rho, 0, this->alpha);
    }
    if(this->type[0] == "R"){
        painter->drawArc(this->start.x(), this->start.y(), this->rho, this->rho, 0, this->alpha);
    }
}
