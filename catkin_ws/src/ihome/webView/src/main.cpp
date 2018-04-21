/**
 * @file /src/main.cpp
 *
 * @brief Qt based gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QApplication>
#include "../include/webView/main_window.hpp"

#include <QPainter>  
#include <QMovie> 
#include <QApplication>
#include <QLabel>
#include <QMovie>
#include <QDesktopWidget>
/*****************************************************************************
** Main
*****************************************************************************/

int gifViewer(char *file)
{
    QLabel *label = new QLabel();
    QMovie *movie = new QMovie("./img/44.gif");
    movie->setScaledSize(QSize(1924,1080));
    label->setMovie(movie);
    movie->start();
    //label->showFullScreen();
    label->show();
}

int main(int argc, char **argv) {

    /*********************
    ** Qt
    **** c v              ******************/
    QApplication app(argc, argv);
                                                               
    webView::MainWindow w(argc,argv);

    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    int result = app.exec();

	return result;
}
