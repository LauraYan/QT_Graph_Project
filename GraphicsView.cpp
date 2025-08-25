#include "GraphicsView.h"

#include <QGraphicsScene>
#include <QContextMenuEvent>
#include <QMenu>

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent) {
}

void GraphicsView::contextMenuEvent(QContextMenuEvent* event) {

  QMenu menu(this);

  QAction* addCircle = menu.addAction("Add Circle");
  QAction* addRect = menu.addAction("Add Rectangle");
  QAction* addEllipse = menu.addAction("Add Ellipse");

  QAction* selectedAction = menu.exec(event->globalPos());

  if (selectedAction == addCircle) {
    scene()->addEllipse(event->pos().x(), event->pos().y(), 50, 50,
      QPen(Qt::black), QBrush(Qt::green));
  }
  else if (selectedAction == addRect) {
    scene()->addRect(event->pos().x(), event->pos().y(), 80, 40,
      QPen(Qt::black), QBrush(Qt::blue));
  }
  else if (selectedAction == addEllipse) {
    scene()->addEllipse(event->pos().x(), event->pos().y(), 80, 40,
      QPen(Qt::black), QBrush(Qt::yellow));
  }
}