#include "mainwindow.h"
#include "ui_mainwindow.h"

Adn_vector bureau;
main_connect main_connexion;
MainWindow *p_mv;
pthread_cond_t condLaunchServer;
pthread_mutex_t mutex;
bool_t launchServer=false;

void *callThreads_handler(void *arg)
{
    if((errno=pthread_cond_init(&condLaunchServer,NULL))!=0)
        ERR_PERROR("pthread_cond_init",1);
    pthread_mutex_init(&mutex,NULL);
    pthread_t thread[2];

    if((errno=pthread_create(&thread[0],NULL,connection_handler,NULL))!=0)
        ERR_PERROR("pthread_create",1);
    P(&mutex);
    while (!launchServer)
        pthread_cond_wait (&condLaunchServer, &mutex);
    V(&mutex);
    pthread_cancel(thread[0]);
    if((errno=pthread_create(&thread[1],NULL,trade_handler,NULL))!=0)
        ERR_PERROR("pthread_create",2);
    return arg;
}

void *connection_handler(void *arg)
{
    bool_t stat;
    stat = registerrpc( PROGNUM,
                  VERSNUM,
                  PROCFIRST,
                  (char *(*)(char *))connection,
                  (xdrproc_t) xdr_void,
                  (xdrproc_t) xdr_connect); //Une erreur de compilation ici ? Essayez sudo apt-get install libtirpc-dev
    if (stat)
    {
        QMessageBox::critical(this,QString("Erreur"),QString("La création du serveur a échouée :\nEchec de l'enregistrement du service de connexion."),QMessageBox::Ok);
        return arg;//NULL
    }
    else
    {
        svc_run(); /* le serveur est en attente de clients eventuels */
    }
    return arg;
}

void *trade_handler(void *arg)
{
    bool_t stat;
    stat = registerrpc(
                     PROGNUM,
                     VERSNUM,
                     PROCSECOND,
                     (char *(*)(char *))trade,
                     (xdrproc_t) xdr_adn_vector,
                     (xdrproc_t) xdr_adn_vector
                     );
    if (stat)
    {
        QMessageBox::critical(this,QString("Erreur"),QString("La création du serveur a échouée :\nEchec de l'enregistrement du service de connexion."),QMessageBox::Ok);
        return arg;
    }
    else
    {
        svc_run(); /* le serveur est en attente de clients eventuels */
    }
    return arg;
}

main_connect *connection()
{
    bool_t notTooMany=true;
    P(&mutex);
        main_connexion.informations.num_village++;
        if(main_connexion.informations.num_village>main_connexion.informations.nb_villages)
            notTooMany=false;
        else if(main_connexion.informations.num_village==main_connexion.informations.nb_villages)
        {
            launchServer=true;
            pthread_cond_signal(&condLaunchServer);
        }
    V(&mutex);
    if(notTooMany)
    {
        p_mv->ui->label_nb_servers->setNum(main_connexion.informations.num_village);
        return &main_connexion;
    }
    else
        return NULL;
}

Adn_vector *trade(Adn_vector *arg)
{
    unsigned int i;
    static Adn_vector ret;

    if(bureau.num_village!=0)
    {
        for(i=0;i<ret.size;i++)
        {
            free(ret.adns[i]);
        }
        free(ret.adns);
    }
    ADNV_append(&bureau, arg);
    this->glwidget->repaint();
    ret=ADNV_give_adns(&bureau,
                       (main_connexion.informations.taux_migration*main_connexion.informations.nb_individus)/100,
                        arg->num_village);
    return &ret;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    console(new InfoConsole)
{
    p_mv=this;
    console->show();
    p_main_connexion=&main_connexion;
    p_bureau=&bureau;
    ADNV_new_global(&bureau); //création du bureau des immigrés
    ui->setupUi(this);

    main_connexion_init_infos();

    p_main_connexion->map_principale.nb_obstacles=5;

    p_main_connexion->map_principale.height=400;
    p_main_connexion->map_principale.width=400;

    ui->horizontalSlider_village->sliderMoved(5); // force on_horizontalSlider_village_sliderMoved
}


void MainWindow::main_connexion_init_infos()
{
    p_main_connexion->informations.taux_migration=ui->horizontalSlider_mig->value();
    p_main_connexion->informations.pc_mutations=ui->horizontalSlider_mut->value();
    p_main_connexion->informations.nb_individus=ui->horizontalSlider_pop->value();
    p_main_connexion->informations.nb_villages=ui->horizontalSlider_village->value();
    p_main_connexion->informations.gen_par_mig=ui->horizontalSlider_miggen->value();
    p_main_connexion->informations.pas=ui->horizontalSlider_pas->value();
    p_main_connexion->informations.nb_mutations=ui->horizontalSlider_mutnb->value();
    p_main_connexion->informations.convergence=ui->checkBox_convergence->isChecked();
    p_main_connexion->informations.num_village=0;
}

void MainWindow::main_connexion_init_map()
{
    p_main_connexion->map_principale.nb_obstacles=(unsigned int)glwidget->nbcircles;
    p_main_connexion->map_principale.obstacles=glwidget->cercles;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    main_connexion_init_infos();
    main_connexion_init_map();

    Village v(main_connexion,NULL,this->glwidget);
    v.mainLoop(&bureau);
}

void MainWindow::on_pushButton_launchServer_clicked()
{
    main_connexion_init_infos();
    main_connexion_init_map();
    pthread_t thread;
    if((errno=pthread_create(&thread,NULL,callThreads_handler,NULL))!=0)
        ERR_PERROR("pthread_create",1);
}

void MainWindow::showGL()
{
    glwidget = new GLWidget(this);
    glwidget->move(530,5);
    glwidget->show();
    glwidget->paintGL();
}

void MainWindow::on_pushButton_2_clicked()
{
    glwidget->nbcircles = this->ui->spinBox_nbcercle->value();
    glwidget->minradius = this->ui->spinBox_min->value();
    glwidget->maxradius = this->ui->spinBox_maxr->value();
    glwidget->drawRandomCircles(glwidget->cercles, glwidget->nbcircles,glwidget->minradius,glwidget->maxradius);
    glwidget->repaint();
}

void MainWindow::on_spinBox_min_valueChanged(int arg1)
{
    this->ui->spinBox_maxr->setMinimum(arg1);
}
