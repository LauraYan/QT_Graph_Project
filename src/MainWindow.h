#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class QGraphicsView;
class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void createMenus();

protected slots:
  void openFile();
  void saveImageFile();
  void savePDFFile();
  void exitApp();

private:
    Ui::MainWindow ui;
    QGraphicsScene* scene;
    QGraphicsView* view;
};

