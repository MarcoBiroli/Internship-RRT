#include "tree.h"

Tree::Tree(Point iroot){
    root = iroot;
    this->max_nodes = INFINITY;
    this->x_max = -1;
    this->y_max = -1;
    this->from_paint_area = true;
    nodes[0] = root;
    forward_links[0] = QVector<int>();
    backward_links[0] = QVector<int>();
    distances[0] = 0;
    node_id = 1;
}

Tree::Tree(Point iroot, int imax_nodes, double ix_max, double iy_max){
    root = iroot;
    this->max_nodes = imax_nodes;
    this->x_max = ix_max;
    this->y_max = iy_max;
    nodes[0] = root;
    forward_links[0] = QVector<int>();
    backward_links[0] = QVector<int>();
    distances[0] = 0;
    node_id = 1;
}

int Tree::addNode(Point node){
    nodes[node_id] = node;
    forward_links[node_id] = QVector<int>();
    backward_links[node_id] = QVector<int>();
    distances[node_id] = -1;
    node_id ++;
    return node_id-1;
}

void Tree::addEdge(int node_id1, int node_id2){
    forward_links[node_id1].append(node_id2);
    backward_links[node_id2].append(node_id1);
    distances[node_id2] = distances[node_id1] + distance(nodes[node_id1], nodes[node_id2]);
}

QPair<int, Point> Tree::find_nearest(Point node){
    Point best_node = root;
    int best_id = -1;
    double min_dist = INFINITY;
    double d;
    QMap<int, Point>::iterator i;
    for(i = this->nodes.begin(); i != this->nodes.end(); i++)
    {
        d = distance(i.value(), node);
        if (d < min_dist){
            min_dist = d;
            best_node = i.value();
            best_id = i.key();
        }
    }
    return QPair<int, Point>(best_id, best_node);
}

QPair<int, QVector<Point>> Tree::find_nearest_dubin(Point node, Robot robot, double step){
    QVector<Point> best_path;
    int best_id;
    double min_dist = INFINITY;
    double d;
    QMap<int, Point>::iterator i;
    for(i = this->nodes.begin(); i != this->nodes.end(); i++)
    {
        QVector<Point> pos_path;
        foreach(pos_path, i.value().generate_dubin(5, robot.s, robot.L, robot.max_angle, step)){
            Point candidate;
            QVector<Point> cur_path;
            foreach(candidate, pos_path){
                cur_path.append(candidate);
                d = distance(node, candidate);
                if (d < min_dist){
                    min_dist = d;
                    best_id = i.key();
                    best_path = cur_path;
                }
            }
        }
        /*
        d = robot.min_dist(i.value(), node);
        if (d < min_dist){
            min_dist = d;
            best_node = i.value();
            best_id = i.key();
            best_path =
        }
        */
    }
    return QPair<int, QVector<Point>>(best_id, best_path);
}

QVector<QPair<int, Point>> Tree::get_close(Point node, double rad){
    QVector<QPair<int, Point>> out;
    QMap<int, Point>::iterator i;
    for(i = this->nodes.begin(); i != this->nodes.end(); i++){
        if(node == i.value()){
            continue;
        }
        double d = distance(i.value(), node);
        if (d < rad){
            out.append(QPair<int, Point>(i.key(), i.value()));
        }
    }
    return out;
}

QPair<Point, Point> Tree::remove_parent_edge(QPair<int, Point> node){
    int parent = this->backward_links.value(node.first)[0];
    this->backward_links[node.first].clear();
    this->forward_links[parent].remove(this->forward_links.value(parent).indexOf(node.first));
    return QPair<Point, Point>(this->nodes[parent], node.second);
}


QPair<int, Point> Tree::choose_parent(painter_widget *paintArea, Point node, QVector<QPair<int, Point>> candidates, QVector<QRect> obstacles){
    QPair<int, Point> best;
    double min_dist = INFINITY;
    QPair<int, Point> candidate;
    foreach(candidate, candidates){
        if (!is_valid(paintArea, candidate.second, node, obstacles)){
            continue;
        }
        double d = this->distances[candidate.first] + distance(candidate.second, node);
        if (d < min_dist){
            min_dist = d;
            best = candidate;
        }
    }
    return best;
}


void Tree::rewire(painter_widget *paintArea, QVector<QPair<int, Point>> close_points, QPair<int, Point> new_point, QVector<QRect> obstacles){
    QVector<QPair<Point, Point>> out;
    QPair<int, Point> node;
    foreach(node, close_points){
        if(this->distances[new_point.first] + distance(new_point.second, node.second) < this->distances[node.first]){
            if(is_valid(paintArea, new_point.second, node.second, obstacles))
            {
                QPair<Point, Point> parent_edge = this->remove_parent_edge(node);
                out.append(parent_edge);
                this->addEdge(new_point.first, node.first);
                paintArea->add_to_draw("1Green", QColor(0, 255, 0, 255), new_point.second, node.second);
                paintArea->delete_to_draw("1Green", QVector<QPair<Point, Point>>(1, parent_edge));
                //paintArea->add_to_draw("2Delete", paintArea->palette().color(paintArea->backgroundRole()), parent_edge.first, parent_edge.second);
            }
        }
    }
    paintArea->delete_to_draw("1Green", out);
}

bool Tree::IsOnlyChild(QPair<int, Point> node)
{
    return (this->forward_links.value(this->backward_links.value(node.first)[0]).length() == 1);
}

QVector<QPair<Point, Point>> Tree::remove_node(int node){
    QVector<QPair<Point, Point>> out;
    out.append(QPair<Point, Point>(this->nodes[this->backward_links[node][0]], this->nodes[node]));
    this->forward_links[this->backward_links.value(node)[0]].remove(this->forward_links.value(this->backward_links.value(node)[0]).indexOf(node));
    int succesor;
    foreach(succesor, this->forward_links.value(node)){
        out.append(QPair<Point, Point>(this->nodes[node], this->nodes[succesor]));
        this->backward_links[succesor].remove(this->backward_links.value(succesor).indexOf(node));
    }
    this->forward_links.remove(node);
    this->backward_links.remove(node);
    this->distances.remove(node);
    this->nodes.remove(node);
    return out;
}

QVector<QPair<Point, Point>> Tree::remove_parent(QPair<int, Point> node){
    return this->remove_node(this->backward_links[node.first][0]);
}

bool Tree::rewire_fn(painter_widget *paintArea, QVector<QPair<int, Point>> close_points, QPair<int, Point> new_point, QVector<QRect> obstacles){
    bool deleted_node = false;
    QVector<QPair<Point, Point>> out;
    QPair<int, Point> node;
    foreach(node, close_points){
        if(this->distances[new_point.first] + distance(new_point.second, node.second) < this->distances[node.first]){
            if(is_valid(paintArea, new_point.second, node.second, obstacles))
            {
                if(this->IsOnlyChild(node) && !deleted_node){
                    out.append(this->remove_parent(node));
                    deleted_node = true;
                }
                else{
                    QPair<Point, Point> parent_edge = this->remove_parent_edge(node);
                    out.append(parent_edge);
                }
                this->addEdge(new_point.first, node.first);
                paintArea->add_to_draw("1Green", QColor(0, 255, 0, 255), new_point.second, node.second);
                //paintArea->add_to_draw("2Delete", paintArea->palette().color(paintArea->backgroundRole()), parent_edge.first, parent_edge.second);
            }
        }
    }
    paintArea->delete_to_draw("1Green", out);
    return deleted_node;
}

bool Tree::force_delete(painter_widget *paintArea){
    QMap<int, QVector<int>>::iterator i;
    for (i = this->forward_links.begin(); i != this->forward_links.end(); i++){
        if (i.value().length() == 0){
            paintArea->delete_to_draw("1Green", this->remove_node(i.key()));
            return true;
        }
    }
    return false;
}

QPair<int, Point> Tree::grow_tree(painter_widget* paintArea, double step, QVector<QRect> obstacles, QString scolor, QColor color){
    if (this->nodes.size() >= this->max_nodes){
        return QPair<int, Point>(-1, Point());
    }
    if(this->from_paint_area){
        this->x_max = paintArea->width();
        this->y_max = paintArea->height();
    }
    Point cur_point = rand_point(this->x_max, this->y_max);
    QPair<int, Point> closest_pair = this->find_nearest(cur_point);
    int closest_id = closest_pair.first;
    Point closest = closest_pair.second;
    int np_id = -1;
    Point np = build(closest, cur_point, step);
    if (is_valid(paintArea, closest, np, obstacles)){
        np_id = this->addNode(np);
        this->addEdge(closest_id, np_id);
        paintArea->add_to_draw(scolor, color, closest, np);
    }
    return QPair<int, Point>(np_id, np);
}

QPair<int, Point> Tree::grow_tree_dubin(painter_widget* paintArea, double step, QVector<QRect> obstacles, Robot robot, QString scolor, QColor color){
    if (this->nodes.size() >= this->max_nodes){
        return QPair<int, Point>(-1, Point());
    }
    if(this->from_paint_area){
        this->x_max = paintArea->width();
        this->y_max = paintArea->height();
    }
    Point cur_point = rand_point(this->x_max, this->y_max);
    QPair<int, QVector<Point>> steer = this->find_nearest_dubin(cur_point, robot, step);
    int closest_id = steer.first;
    Point closest = steer.second.first();
    Point np = steer.second.last();
    int np_id = -1;
    if (is_path_valid(paintArea, steer.second, obstacles)){
        np_id = this->addNode(np);
        this->addEdge(closest_id, np_id);
        paintArea->add_path_to_draw(scolor, color, closest_id, steer.second);
    }
    return QPair<int, Point>(np_id, np);
}

QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> Tree::retrace(QPair<int, Point> fp){
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> out;
    out.first = 0;
    QPair<int, Point> pp = fp;
    QPair<int, Point> np;
    while (np.second != this->root){
        np.first = this->backward_links[pp.first][0];
        np.second = this->nodes[np.first];
        out.first += distance(pp.second, np.second);
        out.second.append(QPair<QPair<int, Point>, QPair<int, Point>>(np, pp));
        pp = np;
    }
    return out;
}

QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> Tree::retrace_dubin(QPair<int, Point> fp, Robot robot){
    QPair<double, QVector<QPair<QPair<int, Point>, QPair<int, Point>>>> out;
    out.first = 0;
    QPair<int, Point> pp = fp;
    QPair<int, Point> np;
    while (np.second != this->root){
        np.first = this->backward_links[pp.first][0];
        np.second = this->nodes[np.first];
        out.first += robot.min_dist(pp.second, np.second).second;
        out.second.append(QPair<QPair<int, Point>, QPair<int, Point>>(np, pp));
        pp = np;
    }
    return out;
}
