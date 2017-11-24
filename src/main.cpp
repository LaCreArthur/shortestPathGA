#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    // place la fenetre au milieu de l'écran
    w.setGeometry(
     QStyle::alignedRect(
     Qt::LeftToRight,
     Qt::AlignCenter,
     w.size(),
     qApp->desktop()->availableGeometry()
     ));
    // affiche la fenetre
    w.show();
    // init et affiche le glwidget
    w.showGL();

    return a.exec();
}
