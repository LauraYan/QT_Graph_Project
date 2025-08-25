#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "GraphicsView.h"

class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void createMenus();
 

 protected:
   void createFileMenus();
   void createEditMenus();

protected slots:
  void openFile();
  void saveImageFile();
  void savePDFFile();
  void exitApp();

  void addCircleShape();
  void addRectShape();
  void addEllipseShape();

private:
    Ui::MainWindow ui;
    QGraphicsScene* scene;
    GraphicsView* view;
};

