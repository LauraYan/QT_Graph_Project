#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "GraphicsView.h"

class QGraphicsScene;
class QTreeWidget;
class QTreeWidgetItem;

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
   void initTreeWidget();
   void closeEvent(QCloseEvent* event) override;

   

protected slots:
  void openFile();
  void saveImageFile();
  void savePDFFile();
  void exitApp();

  void addCircleShape();
  void addRectShape();
  void addEllipseShape();
  void onTreeItemClicked(QTreeWidgetItem* item, int column);
  void addNewTreeWidgetItem(const QString& groupName, QGraphicsItem* graphicItem);
 
private:
    Ui::MainWindow ui;
    QGraphicsScene* mGraphScene;
    GraphicsView* mGraphView;
};

