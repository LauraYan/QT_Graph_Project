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

/*
* Constructor to create a scene and view
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    createMenus();
    scene = new QGraphicsScene(ui.frame);
    scene->setSceneRect(ui.frame->x(), ui.frame->y(), 800, 600);
    view = new GraphicsView(scene, ui.frame);
    view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(ui.frame);  
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
    scene->addPixmap(pix);
  }
}

/*
* This will save image into a png image file, then pop up the folder of this file.
*/
void MainWindow::saveImageFile() {
  QString fileName = QFileDialog::getSaveFileName(this, "Save Image File", "", "PNG Images (*.png)");

  if (!fileName.isEmpty()) {  
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);
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
  QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::white);
  QPainter imagePainter(&image);
  scene->render(&imagePainter);

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
* slot function to draw a circle shape.
*/
void MainWindow::addCircleShape() {

  QGraphicsEllipseItem* circle = scene->addEllipse(100, 100, 100, 100, QPen(Qt::black), QBrush(Qt::green));
  circle->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*
* slot function to draw a rectangle shape.
*/
void MainWindow::addRectShape() {

  QGraphicsRectItem* rect = scene->addRect(50, 50, 200, 100, QPen(Qt::black), QBrush(Qt::blue));
  rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*
* slot function to draw a ellipse shape.
*/
void MainWindow::addEllipseShape() {

  QGraphicsEllipseItem* ellipse = scene->addEllipse(300, 200, 100, 100, QPen(Qt::red), QBrush(Qt::yellow));
  ellipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

