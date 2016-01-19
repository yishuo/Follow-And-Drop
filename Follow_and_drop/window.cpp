// window.cpp

#include <QtWidgets>
#include "window.h"
#include "ui_window.h"


Window::Window(QWidget *parent)
    : QWidget(parent),  ui(new Ui::Window)
{
    score=0;
    msgBox.setVisible(false);
    temps=60;


    slm = new QStringListModel(this);

    sl = new QStringList();
//fenêtre qui demande le nom au début

    fenetre = new Fenetre_Nom();
    fenetre->show();

    ui->setupUi(this);
    ui->Nom_Joueur->setText(fenetre->Nom_Joueur);
    ui->minute->setText("1");
    ui->seconde->setText("0");




// si Nom ok
    if (!(fenetre->Nom_Joueur.isEmpty()))
    {

            delete fenetre;
            ui->myGLWidget->actif=0;


            //démarrer webcamwindow
            m_webcam = new WebCamWindow();


            ui->arreter->setEnabled(false);
            ui->change_nom->setEnabled(true);
            ui->demarrer->setEnabled(true);
            ui->rayonCible->setEnabled(true);
            ui->xxvaleur->setEnabled(false);
            ui->yyvaleur->setEnabled(false);

             //connecté trackbar balle /rayon cible
            connect(ui->xxvaleur, SIGNAL(valueChanged(int)), ui->myGLWidget, SLOT(setXX(int)));
            connect(ui->yyvaleur, SIGNAL(valueChanged(int)), ui->myGLWidget, SLOT(setYY(int)));
            connect(ui->rayonCible, SIGNAL(valueChanged(int)), ui->myGLWidget, SLOT(setRayonCible(int)));

            //connecté position camera/position balle
            connect(m_webcam,SIGNAL(SignalXBouge()),this,SLOT(XMoveCam()));
            connect(ui->change_nom, SIGNAL(clicked()), this, SLOT(setNom()));
            connect(ui->cam_scroll, SIGNAL(valueChanged(int)), this, SLOT(camera_scrollbar()));
            connect(ui->myGLWidget , SIGNAL(Score_change()), this, SLOT(Score()));

            connect(ui->demarrer,SIGNAL(clicked()),this,SLOT(demarrer_partie()));
            connect(ui->arreter ,SIGNAL(clicked()),this,SLOT(arreter_partie()));
            connect(ui->enregistrer ,SIGNAL(clicked()),this,SLOT(enregistrer_fichier()));
            connect(ui->myGLWidget, SIGNAL(bordure_signal()), this, SLOT(trackbar_init()));
            connect(ui->rayonCible, SIGNAL(valueChanged(int)), this, SLOT(changement_temps()));


    }

    else
    {

        delete fenetre;
        delete this;
    }

}

Window::~Window()
{
    delete ui;
}


void Window::debut()
{
   ui->arreter->setEnabled(false);
   ui->change_nom->setEnabled(true);
   ui->demarrer->setEnabled(true);
   ui->rayonCible->setEnabled(true);
   ui->xxvaleur->setEnabled(false);
   ui->yyvaleur->setEnabled(false);


}

void Window::camera_scrollbar()
{
    if (ui->cam_scroll->value()==1)
    {

        m_webcam->close();
        m_webcam->changement_cam_bar();

        ui->change_nom->setEnabled(true);
        ui->demarrer->setEnabled(true);
        ui->rayonCible->setEnabled(true);
        ui->arreter->setEnabled(false);
        ui->xxvaleur->setEnabled(false);
        ui->yyvaleur->setEnabled(false);

    }
    else
       {
        m_webcam->show();
        ui->arreter->setEnabled(false);
        ui->change_nom->setEnabled(false);
        ui->xxvaleur->setEnabled(false);
        ui->yyvaleur->setEnabled(false);
        ui->demarrer->setEnabled(false);
        ui->rayonCible->setEnabled(false);

        connect(m_webcam,SIGNAL(initialisation()),this,SLOT(debut()));
    }

}



void Window::XMoveCam()
{

    ui->myGLWidget->xx=(-(m_webcam->X)*4/m_webcam->frameWidth_)+2;
    ui->myGLWidget->yy=(-(m_webcam->Y)*4/m_webcam->frameHeight_)+2;
    ui->myGLWidget->updateGL();

}

void Window::setNom()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Your Name"),
                                         tr("Input your name :"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(ok && !text.isEmpty()){
        ui->Nom_Joueur->setText(text);
    }
}

void Window::Score()
{
    ui->xxvaleur->setValue(-100);
    ui->yyvaleur->setValue(-100);
    score+=1;
    QString s = QString::number(score);
    ui->score->setText(s);

}


void Window::demarrer_partie()
{
    seconde_attente=3;
    attente = new QTimer(this);
    attente->start(1000); //1000ms=1s
    connect(attente, SIGNAL(timeout()), this, SLOT(attente_add()));

    ui->demarrer->setEnabled(false);
    ui->arreter->setEnabled(true);
    ui->rayonCible->setEnabled(false);
    ui->myGLWidget->random_cible();

  }

void Window::attente_add()
{

    if (seconde_attente>0)
    {


        msgBox.setText("Debut dans "+QString::number(seconde_attente)+" seconde");
        msgBox.setStandardButtons(QMessageBox::NoButton);
        //msgBox.setText("Debut dés que vous cliquerez sur OK");
        msgBox.setWindowModality(Qt::NonModal);
        msgBox.show();




        seconde_attente-=1;
    }
    else
    {
        msgBox.setVisible(false);
        attente->stop();
        ui->xxvaleur->setEnabled(true);
        ui->yyvaleur->setEnabled(true);
        timer = new QTimer(this);
        timer->start(1000); //1000ms=1s
        connect(timer, SIGNAL(timeout()), this, SLOT(timer_jeu()));


        score=0;
        ui->myGLWidget->actif=1;
    }

}

void Window::arreter_partie()
{
     timer->stop();
     gestion_liste();
     score=0;
     changement_temps();
     ui->myGLWidget->actif=0;
     QString s = QString::number(score);
     ui->score->setText(s);

     ui->rayonCible->setEnabled(true);
     ui->demarrer->setEnabled(true);
     ui->arreter->setEnabled(false);
     ui->xxvaleur->setEnabled(false);
     ui->yyvaleur->setEnabled(false);
     ui->xxvaleur->setValue(-100);
     ui->yyvaleur->setValue(-100);

     QMessageBox msgBox;
      msgBox.setText("Fin du temps");
      msgBox.exec();



}

void Window::gestion_liste()
{

    QString nom=ui->Nom_Joueur->text();
    QString s = QString::number(score);

    int niveau= ui->rayonCible->value();

    if(niveau==1)
    {
    sl->append(s+"          "+nom+"          "+"Facile");
    }
    if(niveau==2)
    {
    sl->append(s+"          "+nom+"        "+"Normale");
    }
    if(niveau==3)
    {
    sl->append(s+"         "+nom+"          "+"Difficile");
    }



    slm->setStringList(*sl);


    ui->listView->setModel(slm);
}

void Window::timer_jeu()
{
    temps-=1;


    QString s = QString::number(temps-((temps/60)*60));
    ui->seconde->setText(s);
    QString m = QString::number(temps/60);
    ui->minute->setText(m);




    if(temps<=0)
    {
        arreter_partie();
    }
}

void Window::changement_temps()
{
    int niveau= ui->rayonCible->value();

    if(niveau==1)
    {
        temps=90;

    }
    if(niveau==2)
    {
        temps=60;

    }
    if(niveau==3)
    {
        temps=30;

    }

    QString s = QString::number(temps-((temps/60)*60));
    ui->seconde->setText(s);
    QString m = QString::number(temps/60);
    ui->minute->setText(m);


}



void Window::enregistrer_fichier()
{
    QMessageBox msbox;
    QString texte;
    int i=0;
    int limite = sl->size();

    while(i <= (limite-1))
    {
    texte=texte+"\r\n"+sl->value(i);
    i++;
    }

    QString dir=QFileDialog::getExistingDirectory(this, "Choisir ou créer votre repertoire de travail!", "", QFileDialog::ShowDirsOnly|      QFileDialog::DontResolveSymlinks);
    QString fileName= QFileDialog::getSaveFileName(this, "Enregistrer votre fichier dans votre repertoire de travaiol!", dir, "*.txt;; *.doc;;* *.xml;;");

    if (dir.isEmpty())
    {
         msbox.setText("Vous avez annulé la création de votre repertoire de travail");
         msbox.exec();
    }
    else
    {
         if (fileName.isEmpty())
         {
             msbox.setText("Vous n'avez pas créé de fichier dans votre repertoire de travail");
             msbox.exec();
         }
         else
         {
                 QFile fichier(fileName);
                 fichier.open(QIODevice::WriteOnly | QIODevice::Text);

                 QTextStream flux(&fichier);
                 flux << texte;

                 fichier.close();

         }
    }
}

void Window::trackbar_init()
{
    ui->xxvaleur->setValue(-100);
    ui->yyvaleur->setValue(-100);
}
