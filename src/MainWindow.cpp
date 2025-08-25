#include "MainWindow.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    scene = new QGraphicsScene(this);
    //scene->setSceneRect(0, 0, 400, 200);

    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(view);

    createMenus();

    // Example shapes
    scene->addRect(50, 50, 200, 100, QPen(Qt::black), QBrush(Qt::blue));
    scene->addEllipse(300, 200, 100, 100, QPen(Qt::red), QBrush(Qt::yellow));
}

MainWindow::~MainWindow() {

}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Images (*.png *.jpg *.bmp)");
  if (!fileName.isEmpty()) {
    QPixmap pix(fileName);
    scene->addPixmap(pix);
  }
}

void MainWindow::saveImageFile() {
  QString fileName = QFileDialog::getSaveFileName(this, "Save Image File", "", "PNG Images (*.png)");
  if (!fileName.isEmpty()) {
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);
    image.save(fileName);
  }
}

void MainWindow::savePDFFile() {

  QString fileName = QFileDialog::getSaveFileName(this, "Save PDFFile", "", "PNG Images (*.png)");
  if (!fileName.isEmpty()) {
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);
    image.save(fileName);
  }
}


void MainWindow::exitApp() {

  QApplication::quit();
}

void MainWindow::createMenus() {

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


