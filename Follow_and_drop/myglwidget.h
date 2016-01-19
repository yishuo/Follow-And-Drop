    // myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <GL/glu.h>
#include <QGLWidget>
#include <QTimer>
#include <QtCore/qmath.h>




class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
    float xx;
    float yy;
    float zz;
    int actif;
    void random_cible();


signals:

void Score_change();
void bordure_signal();



protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setXX(int xx);//entrée =varible entier x scrollbar/ pas de sort8e/ yoann
    void setYY(int yy);
    void rotation_robot();
    void bordure();
    void setRayonCible(int rayonCible);




private:
void cercle_init();
void cible_init();
void cercle_trou_init();
void cylindre_init();

void arene_init();
void sphere_init();
void socle_init();
void bras_init();
void bras2_init();

void robot_init();


void LoadGLTextures( const char * name );



    QTimer timer1;

    GLUquadricObj *cylinder_obj;
    GLuint cylindre;
    GLuint sphere;
    GLuint cercle;
    GLuint cible;
    GLuint cercle_trou;

    GLuint socle;
    GLuint bras;
    GLuint bras2;
    GLuint arene;
    GLuint pince;

    GLuint mains;
    GLuint avant_bras;
    GLuint ensemble_bras;
    GLuint robot;

    GLuint texture[1];
    QPoint lastPos;

    float Xcible;
    float Ycible;
    int Xtrou;
    int Ytrou;



    float foisCible;
    float Rayon_cible_trou;
    float distanceDesDoigts;
    int angleSphereHaut;
    int avantbrasX;
    int sphereBasX;
    int sphereBasZ;


};

#endif // MYGLWIDGET_H

