// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QString>
#include "myglwidget.h"
#include "webcamwindow.h"
#include "fenetre_nom.h"




namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();


private slots:
    void XMoveCam();
    void setNom();
    void Score();
    void debut();
    void camera_scrollbar();
    void demarrer_partie();
    void arreter_partie();
    void timer_jeu();
    void enregistrer_fichier();
    void attente_add();
    void trackbar_init();
    void changement_temps();

private:
    WebCamWindow *m_webcam;
    Fenetre_Nom *fenetre;
    QStringListModel* slm;
    QStringList* sl;
    Ui::Window *ui;
    void gestion_liste();
    int score;
    int temps;
    int seconde_attente;

    QMessageBox msgBox;

    QTimer *attente;
    QTimer *timer;


};

#endif // WINDOW_H
