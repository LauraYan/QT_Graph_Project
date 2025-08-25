#pragma once
#include<QGraphicsView>
#include<QGraphicsScene>

class GraphicsView : public QGraphicsView {

  Q_OBJECT
public:
  GraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);

protected:
  void contextMenuEvent(QContextMenuEvent* event) override;
};