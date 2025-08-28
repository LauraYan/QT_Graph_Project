#include "MainWindow.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QPdfWriter>  //PDF 
#include <QDesktopServices> // pop up the file folder 
#include <QUrl>
#include <QCloseEvent>

#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>

const QString Shape_Group           = "Shapes";
const QString Shape_Circle_Group    = "Circle";
const QString Shape_Ellipse_Group   = "Ellipse";
const QString Shape_Rectangle_Group = "Rectangle";
/*
* Constructor to create a scene and view
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    createMenus(); 
  
    mGraphScene = new QGraphicsScene(ui.frame);
    mGraphScene->setSceneRect(ui.frame->x(), ui.frame->y(), 1000, 600);
    mGraphView = new GraphicsView(mGraphScene, ui.frame);
    mGraphView->setRenderHint(QPainter::Antialiasing);

    // Tree Widget
    initTreeWidget();
    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Sync selection between tree and scene
    connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::onTreeItemClicked);
    connect(mGraphView, &GraphicsView::itmeAdded, this, &MainWindow::addNewTreeWidgetItem);
    connect(ui.treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showTreeWidgetContextMenu);
}

/*
* Destructor function
*/
MainWindow::~MainWindow() {

}

/*
* This will open an image to display it on the view.
*/
void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Images (*.png *.jpg *.bmp)");
  if (!fileName.isEmpty()) {
    QPixmap pix(fileName);
    mGraphScene->addPixmap(pix);
  }
}

/*
* This will save image into a png image file, then pop up the folder of this file.
*/
void MainWindow::saveImageFile() {
  QString fileName = QFileDialog::getSaveFileName(this, "Save Image File", "", "PNG Images (*.png)");

  if (!fileName.isEmpty()) {  
    QImage image(mGraphScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    mGraphScene->render(&painter);
    image.save(fileName);
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(fileName).absolutePath()));
  }  // if
}

/*
* This will save image into a pdf file, then pop up the folder of this file.
*/
void MainWindow::savePDFFile() {

  QString fileName = QFileDialog::getSaveFileName(this, "Save PDF File", "", "PDF File (*.pdf)");
  if (fileName.isEmpty()) 
    return;

  // Get image
  QImage image(mGraphScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::white);
  QPainter imagePainter(&image);
  mGraphScene->render(&imagePainter);

  // Start PDF to work
  QPdfWriter writer(fileName);
  writer.setPageSize(QPageSize(QPageSize::A4));     
  writer.setResolution(600);                        // Optional: DPI
  writer.setPageMargins(QMarginsF(30, 30, 30, 30)); // Optional: margins

  QPainter pdfPainter(&writer);
  QRect rect = pdfPainter.viewport();                  // PDF page rectangle
  QSize size = image.size();
  size.scale(rect.size(), Qt::KeepAspectRatio);     // Scale image to fit
  pdfPainter.setViewport(rect.x(), rect.y(), size.width(), size.height());
  pdfPainter.setWindow(image.rect());

  pdfPainter.drawImage(0, 0, image);

  pdfPainter.end();
  imagePainter.end();

  QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(fileName).absolutePath()));
}

/*
* quit this main window.
*/
void MainWindow::exitApp() {

  QApplication::quit();
}

/*
* This is overrided close event function.
*/
void MainWindow::closeEvent(QCloseEvent* event)  {

  // Show a warning dialog
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Save Changes","Do you want to save before exiting?",
                                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

  if (reply == QMessageBox::Yes) {

    saveImageFile();
    event->accept();  // allow closing
  }
  else if (reply == QMessageBox::No) 
    event->accept();  
  else 
    event->ignore(); 
}

/*
* Wrapper funciton to add File and Edit menu.
*/
void MainWindow::createMenus() {

  createFileMenus();
  createEditMenus();
}

/*
* Create a File menu and actions.
*/
void MainWindow::createFileMenus() {

  QMenu* fileMenu = menuBar()->addMenu("&File");

  QAction* openAction = new QAction("&Open", this);
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
  fileMenu->addAction(openAction);

  QAction* saveImageAction = new QAction("&Save As Image", this);
  connect(saveImageAction, &QAction::triggered, this, &MainWindow::saveImageFile);
  fileMenu->addAction(saveImageAction);

  QAction* savePDFAction = new QAction("&Export PDF File", this);
  connect(savePDFAction, &QAction::triggered, this, &MainWindow::savePDFFile);
  fileMenu->addAction(savePDFAction);

  fileMenu->addSeparator();

  QAction* exitAction = new QAction("E&xit", this);
  connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);
  fileMenu->addAction(exitAction);
}

/*
* Create a Edit menu and actions.
*/
void MainWindow::createEditMenus() {

  QMenu* editMenu = menuBar()->addMenu("&Edit");

  QAction* addCircleAction = new QAction("&Add Circle", this);
  connect(addCircleAction, &QAction::triggered, this, &MainWindow::addCircleShape);
  editMenu->addAction(addCircleAction);


  QAction* addRctAction = new QAction("&Add Rectangle", this);
  connect(addRctAction, &QAction::triggered, this, &MainWindow::addRectShape);
  editMenu->addAction(addRctAction);

  QAction* addEllipseAction = new QAction("&Add Ellipse", this);
  connect(addEllipseAction, &QAction::triggered, this, &MainWindow::addEllipseShape);
  editMenu->addAction(addEllipseAction);
}

/*
* This will create a tree widget and its shape group name.
*/
void MainWindow::initTreeWidget() {

  // Root categories
  ui.treeWidget->setHeaderLabel(Shape_Group);   

  // set the column header text
  QTreeWidgetItem* circleRoot = new QTreeWidgetItem(ui.treeWidget);
  circleRoot->setText(0, Shape_Circle_Group);

  QTreeWidgetItem* ellipseRoot = new QTreeWidgetItem(ui.treeWidget);
  ellipseRoot->setText(0, Shape_Ellipse_Group);

  QTreeWidgetItem* rectRoot = new QTreeWidgetItem(ui.treeWidget);
  rectRoot->setText(0, Shape_Rectangle_Group);
}

/*
* slot function click the tree widget item, will select it on the view area.
*/
void MainWindow::onTreeItemClicked(QTreeWidgetItem* item, int column) {

  if (!item) return;
  QVariant ptr = item->data(0, Qt::UserRole);
  QGraphicsItem* shape = reinterpret_cast<QGraphicsItem*>(ptr.value<void*>());

  if (shape) {
    mGraphScene->clearSelection();
    shape->setSelected(true);
    ui.statusBar->showMessage(item->text(column));
  }
}

void MainWindow::showTreeWidgetContextMenu(const QPoint& pos) {

  QTreeWidgetItem* item = ui.treeWidget->itemAt(pos);
  if (!item) 
    return;

  QMenu menu(this);

  if (item->text(0) == Shape_Circle_Group) {

    QAction* addCircle = menu.addAction("Add Circle");
    connect(addCircle, &QAction::triggered, this, &MainWindow::addCircleShape);

  }
  else if (item->text(0) == Shape_Ellipse_Group) {

    QAction* addEllipse = menu.addAction("Add Ellipse");
    connect(addEllipse, &QAction::triggered, this, &MainWindow::addEllipseShape);
  }
  else if (item->text(0) == Shape_Rectangle_Group) {

    QAction* addRectangle = menu.addAction("Add Rectangle");
    connect(addRectangle, &QAction::triggered, this, &MainWindow::addRectShape);
  }

  menu.exec(ui.treeWidget->viewport()->mapToGlobal(pos));
}

/*
* slot function to draw a circle shape.
*/
void MainWindow::addCircleShape() {

  QGraphicsEllipseItem* circle = mGraphScene->addEllipse(100, 100, 100, 100, QPen(Qt::black), QBrush(Qt::green));
  circle->setFlag(QGraphicsItem::ItemIsSelectable, true);
  ui.statusBar->showMessage("Added a circle!", 3000);

  addNewTreeWidgetItem(Shape_Circle_Group, circle);
}

/*
* slot function to draw a rectangle shape.
*/
void MainWindow::addRectShape() {

  QGraphicsRectItem* rect = mGraphScene->addRect(50, 50, 200, 100, QPen(Qt::black), QBrush(Qt::blue));
  rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
  ui.statusBar->showMessage("Added a rectangle!", 3000);

  addNewTreeWidgetItem(Shape_Rectangle_Group, rect);
}

/*
* slot function to draw a ellipse shape.
*/
void MainWindow::addEllipseShape() {

  QGraphicsEllipseItem* ellipse = mGraphScene->addEllipse(300, 200, 100, 100, QPen(Qt::red), QBrush(Qt::yellow));
  ellipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
  ui.statusBar->showMessage("Added an ellipse!", 3000);

  addNewTreeWidgetItem(Shape_Ellipse_Group, ellipse);
}

/*
* This function will add new item into the tree widget.
*/
void MainWindow::addNewTreeWidgetItem(const QString& groupName, QGraphicsItem* graphicItem) {
  // 2. Add corresponding entry in the tree under a group
  QList<QTreeWidgetItem*> found = ui.treeWidget->findItems(groupName, Qt::MatchExactly | Qt::MatchRecursive);
  if (!found.isEmpty()) {
    QTreeWidgetItem* group = found.first();

    QTreeWidgetItem* newItem = new QTreeWidgetItem(group);
    newItem->setText(0, groupName + "_"+ QString::number(group->childCount()));

    // Optional: link the QGraphicsItem with the tree item via QVariant
    newItem->setData(0, Qt::UserRole, QVariant::fromValue((void*)graphicItem));
  }
}