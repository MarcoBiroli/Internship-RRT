#include "node.h"

Node::Node(double x, double y, double theta, Node *p){
    parent = p;
    point = Point(x, y, theta);
}

Node::Node(double x, double y, Node *iparent)
{
    parent = iparent;
    point = Point(x, y);
}

Node::Node(Point ipoint, Node *iparent){
    parent = iparent;
    point = ipoint;
}

Node::Node(Point node){
    point = node;
    parent = NULL;
}

Node::Node(){
    point = Point();
    parent = NULL;
}
