#pragma once
#include<QGraphicsView>
#include<QGraphicsScene>

class QGraphicsEllipseItem;
class QGraphicsRectItem;

class GraphicsView : public QGraphicsView {

  Q_OBJECT
public:
  GraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);

signals:
  void itmeAdded(const QString name, QGraphicsItem* circle);
 

protected:
  void contextMenuEvent(QContextMenuEvent* event) override;

  QGraphicsScene* mScene;
};