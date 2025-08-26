#include "GraphicsView.h"

#include <QGraphicsScene>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

const QString Shape_Circle_Group = "Circle";
const QString Shape_Ellipse_Group = "Ellipse";
const QString Shape_Rectangle_Group = "Rectangle";

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {

  mScene = scene;
}

void GraphicsView::contextMenuEvent(QContextMenuEvent* event) {

  if (!mScene)
    return;

  QPointF scenePos = mapToScene(event->pos());
  QGraphicsItem* item = mScene->itemAt(scenePos, transform());

  // Only select the item if it's not already selected
  if (item && !item->isSelected()) {
    mScene->clearSelection();
    item->setSelected(true);
  }

  QList<QGraphicsItem*> selected = mScene->selectedItems();

  QMenu menu(this);

  if (selected.isEmpty()) {

    QAction* addCircle = menu.addAction("Add Circle");
    QAction* addRect = menu.addAction("Add Rectangle");
    QAction* addEllipse = menu.addAction("Add Ellipse");

    QAction* selectedAction = menu.exec(event->globalPos());

    if (selectedAction == addCircle) {
      QGraphicsEllipseItem* circle = scene()->addEllipse(event->pos().x(), event->pos().y(), 50, 50,
        QPen(Qt::black), QBrush(Qt::green));

      circle->setFlag(QGraphicsItem::ItemIsSelectable, true);
      emit itmeAdded(Shape_Circle_Group, circle);
    }
    else if (selectedAction == addRect) {
      QGraphicsRectItem* rect = scene()->addRect(event->pos().x(), event->pos().y(), 80, 40,
        QPen(Qt::black), QBrush(Qt::blue));

      rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
      emit itmeAdded(Shape_Rectangle_Group, rect);

    }
    else if (selectedAction == addEllipse) {
      QGraphicsEllipseItem* ellipse = scene()->addEllipse(event->pos().x(), event->pos().y(), 80, 40,
        QPen(Qt::black), QBrush(Qt::yellow));

      ellipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
      emit itmeAdded(Shape_Ellipse_Group, ellipse);
    }
  }
  else if (selected.size() == 1) {

    QMenu menu(this);

    QAction* deleteAction = menu.addAction("Delete Selected");
    QAction* chosen = menu.exec(event->globalPos());
    if (chosen == deleteAction) {
      for (QGraphicsItem* item : selected) {
        scene()->removeItem(item);
        delete item; // free memory
      }  // for
    }  // if
    //QAction* chosen = menu.exec(event->globalPos());
  }  // else if
}