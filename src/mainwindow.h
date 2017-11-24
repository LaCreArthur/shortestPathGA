#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "individu.h"
#include "village.h"
#include "xdr_adn_connection.h"
#include "pthread.h"
#include "errno.h"
#include "infoconsole.h"
#include <iostream>
#include <QMessageBox>

#define ERR_PERROR(f,code) 	do{perror(f); exit(code);}while(0)

#define P(pmutex) 	do{if((errno=pthread_mutex_lock(pmutex))!=0)\
                    ERR_PERROR("pthread_mutex_lock",1);}while(0)

#define V(pmutex) 	do{if((errno=pthread_mutex_unlock(pmutex))!=0)\
                    ERR_PERROR("pthread_mutex_unlock",1);}while(0)

main_connect *connection();
Adn_vector *trade(Adn_vector *arg);
void *callThreads_handler(void *arg);
void *connection_handler(void *arg);
void *trade_handler(void *arg);

namespace Ui {
class MainWindow;
}

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GLWidget* glwidget;
    InfoConsole* console;

    main_connect *p_main_connexion;//Blamez les mecs qui ont fait rpc
    Adn_vector *p_bureau;

    void showGL();
    void main_connexion_init_infos();
    void main_connexion_init_map();
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_min_valueChanged(int arg1);
    void on_pushButton_launchServer_clicked();
};

#endif // MAINWINDOW_H
