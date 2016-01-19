// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>


#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    xx=-1;
    yy=-1;
    distanceDesDoigts=0.01;
    angleSphereHaut=0;
    avantbrasX=0;
    sphereBasX=0;
    sphereBasZ=0;
    Xcible=-1;
    Ycible=0.7;
    Xtrou=1;
    Ytrou=1;
    Rayon_cible_trou=0.16;
    foisCible=1;



  }

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void MyGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::LoadGLTextures( const char * name )
{

    QImage img(name);
    img = img.convertToFormat(QImage::Format_RGB888);
    img = img.mirrored();

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D( GL_TEXTURE_2D, 0, 3,
            img.width(), img.height(),
            0, GL_RGB, GL_UNSIGNED_BYTE,img.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

static float valeurRandX()
{


    float valeur=qrand()%5-2;//La borne du valeur est [-75,75)


    return valeur;
}

static float valeurRandY()
{

    float valeur=qrand()%5-2;//La borne du valeur est [-75,75)

    return valeur;
}

void MyGLWidget::setXX(int xxvaleur)
{

    this->xx=(float)xxvaleur/100;
    updateGL();

}

void MyGLWidget::setYY(int yyvaleur)
{

    this->yy=(float)yyvaleur/100;
    updateGL();
}

void MyGLWidget::setRayonCible(int rayonCible)
{

    this->foisCible = (float)(4-rayonCible)/2;
    this->cible_init();
    updateGL();

}






void MyGLWidget::cercle_trou_init()
{
    glNewList(cercle_trou, GL_COMPILE);


    glPushMatrix();//#1

    float R1=2;
    float R2=Rayon_cible_trou;
    float PI=3.141592653;
    int i;
    int n=40;

    LoadGLTextures(":/image/texture.png");

    glBegin(GL_QUAD_STRIP);
    glNormal3f( 0.0F, 0.0F, 1.0F);
    glColor4d(1,1,1,1);


    for(i=0; i<=n; i++)
    {
         glPushMatrix();//#1

         glTexCoord2f(0.75+0.0375*cos(2*PI/n*i), 0.75+0.375*sin(2*PI/n*i));
         glVertex3f(R2*cos(2*PI/n*i)+1,R2*sin(2*PI/n*i)+1,0);

         glTexCoord2f(0.5+0.5*cos(2*PI/n*i), 0.5+0.5*sin(2*PI/n*i));
         glVertex3f(R1*cos(2*PI/n*i),R1*sin(2*PI/n*i),0);

        glPopMatrix();//#1
    }

    glEnd();

    glPopMatrix();//#1


    glEndList();


}

void MyGLWidget::cercle_init()
{
    glNewList(cercle, GL_COMPILE);



    glPushMatrix();//#1

    double n=40;
    float R=2;
    float PI=3.141592653;
    int i;
    glNormal3f( 0.0F, 0.0F, 1.0F);
    glColor4d(1,1,1,1);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);

    for(i=0; i<=n; i++)


    glVertex2f(R*cos(2*PI/n*i),R*sin(2*PI/n*i));
    glEnd();

    glPopMatrix();//#1

    glEndList();


}

void MyGLWidget::cible_init()
{
    glNewList(cible, GL_COMPILE);



    glPushMatrix();//#1
    glScalef(foisCible, foisCible, foisCible);

    float R=Rayon_cible_trou*1.5;

    float PI=3.141592653;
    int i;
    int n=40;

    LoadGLTextures(":/image/cible.png");

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f( 0.0F, 0.0F, 1.0F);
    glColor4d(1,1,1,0.5);

    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,0,0.01);
    for(i=0; i<=n; i++)
    {
         glPushMatrix();//#1


         glTexCoord2f(0.5+0.5*cos(2*PI/n*i), 0.5+0.5*sin(2*PI/n*i));
         glVertex3f(R*cos(2*PI/n*i),R*sin(2*PI/n*i),0.01);

        glPopMatrix();//#1
    }

    glEnd();

    glPopMatrix();//#1


    glEndList();


}

void MyGLWidget::cylindre_init()//problème avec carte graphique c'est pour cela que l'on a baissé le nombre de polygones
{
    glNewList(cylindre, GL_COMPILE);

    glPushMatrix();//#1

    cylinder_obj=gluNewQuadric();
    gluCylinder(cylinder_obj, 2, 2, 0.2, 40, 40);

    glPopMatrix();//#1
    glEndList();

}

void MyGLWidget::sphere_init()//0.2 rayon
{
    glNewList(sphere, GL_COMPILE);

    glPushMatrix();//#1
    glScalef(0.2, 0.2, 0.2);


    int i, j;
    int lats=40;
    int longs=40;

    for(i = 0; i <= lats; i++)
    {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++)
        {
             double lng = 2 * M_PI * (double) (j - 1) / longs;
             double x = cos(lng);
             double y = sin(lng);

             glNormal3f(x * zr0, y * zr0, z0);
             glVertex3f(x * zr0, y * zr0, z0);
             glNormal3f(x * zr1, y * zr1, z1);
             glVertex3f(x * zr1, y * zr1, z1);
        }
         glEnd();
    }

    glPopMatrix();//#1


    glEndList();
}




void MyGLWidget::arene_init()
{
     glPushMatrix();//#1

    glColor4d(0.988,0.904,0.42,1);
    glCallList(cercle);
    glCallList(cylindre);



    glTranslatef(0,0,0.2);//translater


    glCallList(cercle_trou);

    glPushMatrix();
    glTranslatef(Xcible,Ycible,0);
    glCallList(cible);
    glPopMatrix();

    glColor4d(1,1,0,0.7);
    glScalef(1, 1, 5);
    glCallList(cylindre);

    glPopMatrix();//#1

}

void MyGLWidget::socle_init()
{
glNewList(socle, GL_COMPILE);



        glPushMatrix();

        glTranslatef(0.0, 0.0, 0.2);

        glPushMatrix();
        glScalef(0.15, 0.15, 0.2);
        glCallList(cylindre);



        glTranslatef(0,0,0.2);//translater
        glCallList(cercle);

        glPopMatrix();


glEndList();

}

void MyGLWidget::bras_init()
{
    glNewList(bras, GL_COMPILE);
    LoadGLTextures(":/image/telecom_carre.png");
    glPushMatrix();


     //Multi-colored side - FRONT
      glBegin(GL_POLYGON);
      glColor4f(   1,  1, 1, 1 );
      glNormal3f( 0.0F, 0.0F, -1.0F);
      glTexCoord2d(1,1);    glVertex3f(  0.5, -0.5, -0.5 );
      glTexCoord2d(1,0);    glVertex3f(  0.5,  0.5, -0.5 );     // P2 is green
      glTexCoord2d(0,0);    glVertex3f( -0.5,  0.5, -0.5 );    // P3 is blue
      glTexCoord2d(0,1);    glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple

      glEnd();

      // White side - BACK
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, 0.0F, 1.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,0);    glVertex3f(  0.5, -0.5, 0.5 );
      glTexCoord2d(1,1);    glVertex3f(  0.5,  0.5, 0.5 );
      glTexCoord2d(0,1);    glVertex3f( -0.5,  0.5, 0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();

      // Purple side - RIGHT
      glBegin(GL_POLYGON);
      glNormal3f( 1.0F, 0.0F, 0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,0);    glVertex3f( 0.5, -0.5, -0.5 );
      glTexCoord2d(1,1);    glVertex3f( 0.5,  0.5, -0.5 );
      glTexCoord2d(0,1);    glVertex3f( 0.5,  0.5,  0.5 );
      glTexCoord2d(0,0);    glVertex3f( 0.5, -0.5,  0.5 );
      glEnd();

      // Green side - LEFT
      glBegin(GL_POLYGON);
      glColor4f(   1,  1, 1, 1 );
      glNormal3f(- 1.0F, 0.0F, 0.0F);
      glTexCoord2d(1,0);    glVertex3f( -0.5, -0.5,  0.5 );
      glTexCoord2d(1,1);    glVertex3f( -0.5,  0.5,  0.5 );
      glTexCoord2d(0,1);    glVertex3f( -0.5,  0.5, -0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // Blue side - TOP
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, 1.0F, 0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,1);    glVertex3f(  0.5,  0.5,  0.5 );
      glTexCoord2d(0,1);    glVertex3f(  0.5,  0.5, -0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5,  0.5, -0.5 );
      glTexCoord2d(1,0);    glVertex3f( -0.5,  0.5,  0.5 );
      glEnd();

      // Red side - BOTTOM
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, -1.0F,0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,1);    glVertex3f(  0.5, -0.5, -0.5 );
      glTexCoord2d(0,1);    glVertex3f(  0.5, -0.5,  0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5,  0.5 );
      glTexCoord2d(1,0);    glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();





    glPopMatrix();//#1

    glEndList();
}

void MyGLWidget::bras2_init()
{
    glNewList(bras2, GL_COMPILE);

    LoadGLTextures(":/image/text.png");
    glPushMatrix();//#1

     //Multi-colored side - FRONT
      glBegin(GL_POLYGON);
      glColor4f(   1,  1, 1, 1 );
      glNormal3f( 0.0F, 0.0F, -1.0F);
      glTexCoord2d(1,1);    glVertex3f(  0.5, -0.5, -0.5 );
      glTexCoord2d(1,0);    glVertex3f(  0.5,  0.5, -0.5 );     // P2 is green
      glTexCoord2d(0,0);    glVertex3f( -0.5,  0.5, -0.5 );    // P3 is blue
      glTexCoord2d(0,1);    glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple

      glEnd();

      // White side - BACK
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, 0.0F, 1.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,0);    glVertex3f(  0.5, -0.5, 0.5 );
      glTexCoord2d(1,1);    glVertex3f(  0.5,  0.5, 0.5 );
      glTexCoord2d(0,1);    glVertex3f( -0.5,  0.5, 0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();

      // Purple side - RIGHT
      glBegin(GL_POLYGON);
      glNormal3f( 1.0F, 0.0F, 0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,0);    glVertex3f( 0.5, -0.5, -0.5 );
      glTexCoord2d(1,1);    glVertex3f( 0.5,  0.5, -0.5 );
      glTexCoord2d(0,1);    glVertex3f( 0.5,  0.5,  0.5 );
      glTexCoord2d(0,0);    glVertex3f( 0.5, -0.5,  0.5 );
      glEnd();

      // Green side - LEFT
      glBegin(GL_POLYGON);
      glColor4f(   1,  1, 1, 1 );
      glNormal3f(- 1.0F, 0.0F, 0.0F);
      glTexCoord2d(1,0);    glVertex3f( -0.5, -0.5,  0.5 );
      glTexCoord2d(1,1);    glVertex3f( -0.5,  0.5,  0.5 );
      glTexCoord2d(0,1);    glVertex3f( -0.5,  0.5, -0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // Blue side - TOP
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, 1.0F, 0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,1);    glVertex3f(  0.5,  0.5,  0.5 );
      glTexCoord2d(0,1);    glVertex3f(  0.5,  0.5, -0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5,  0.5, -0.5 );
      glTexCoord2d(1,0);    glVertex3f( -0.5,  0.5,  0.5 );
      glEnd();

      // Red side - BOTTOM
      glBegin(GL_POLYGON);
      glNormal3f( 0.0F, -1.0F,0.0F);
      glColor4f(   1,  1, 1, 1 );
      glTexCoord2d(1,1);    glVertex3f(  0.5, -0.5, -0.5 );
      glTexCoord2d(0,1);    glVertex3f(  0.5, -0.5,  0.5 );
      glTexCoord2d(0,0);    glVertex3f( -0.5, -0.5,  0.5 );
      glTexCoord2d(1,0);    glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();





    glPopMatrix();//#1

    glEndList();
}

void MyGLWidget::robot_init(){

  glPushMatrix();
    glPushMatrix();
    glCallList(socle);
    glPopMatrix();

    glTranslatef(0, 0, 0.3 );
    glRotatef(sphereBasZ, 0, 0, 1);
    glRotatef(sphereBasX, 1, 0, 0);


//...................................................//
    glPushMatrix();//ensemble_bras

        //sphere base
        glPushMatrix();
        glScalef(0.65, 0.65, 0.65);
        glColor3d(1,0,0);

        glCallList(sphere);

        glPopMatrix();


        glTranslatef(0, 0, 0.4);


        //le bras
        glPushMatrix();
        glScalef(0.2, 0.2, 0.7);
        glCallList(bras);
        glPopMatrix();


        glTranslatef(0, 0, 0.4);
        glRotatef(avantbrasX, 1, 0, 0);
  //............................................................//
        glPushMatrix();//avant_bras


               //sphere milieu
                glPushMatrix();//#1
                glColor3d(1,0,0);
                glScalef(0.65, 0.65, 0.65);
                glCallList(sphere);
                glPopMatrix();//#1


                glTranslatef(0, 0, 0.4);

                //bras haut
                glPushMatrix();
                glColor3d(1,0,0);
                glScalef(0.2, 0.2, 0.6);
                glCallList(bras);
                glPopMatrix();


                glTranslatef(0, 0, 0.36);
                glRotatef(angleSphereHaut, 0, 0, 1);

//.........................................................//
                glPushMatrix();//ensemble des mains et la sphere haut

                    glPushMatrix();
                     //sphere haut
                     glColor3d(1,0,0);
                     glScalef(0.65, 0.65, 0.65);
                     glCallList(sphere);



                     //bras
                     glTranslatef(0, 0, 0.2);
                     glScalef(0.6, 0.2, 0.05);
                     glCallList(bras2);
                     glPopMatrix();

                     glTranslatef(0, 0, 0.25);
//....................................................//
                     glPushMatrix();//      les mains




                              //doigt gauche
                              glPushMatrix();
                              glTranslatef(distanceDesDoigts, 0, 0);
                              glScalef(0.05, 0.2, 0.2);
                              glCallList(bras2);
                              glPopMatrix();

                              //doigt droite
                              glPushMatrix();
                              glTranslatef(-distanceDesDoigts, 0, 0);
                              glScalef(0.05, 0.2, 0.2);
                              glCallList(bras2);
                              glPopMatrix();


                       glPopMatrix();

                glPopMatrix();

             glPopMatrix();

           glPopMatrix();
 glPopMatrix();

}







void MyGLWidget::initializeGL()
{

 /*..............lumiere...............................................................*/

    qglClearColor(Qt::white);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

    static GLfloat lightPosition[4] =  {0,0,1,0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    /*...........list.................................................................*/

       cercle = glGenLists(1);
       cible = glGenLists(1);
       cylindre = glGenLists(1);
       cercle_trou = glGenLists(1);

       sphere = glGenLists(1);
       arene = glGenLists(1);
       socle = glGenLists(1);
       bras = glGenLists(1);  
       bras2 = glGenLists(1);
       robot = glGenLists(1);




      cercle_init();
      cible_init();
      cercle_trou_init();
      cylindre_init();
      arene_init();
      sphere_init();
      socle_init();
      bras_init();
      bras2_init();
      robot_init();




   /*.......................................................................................*/

      connect(&timer1, SIGNAL(timeout()), this, SLOT(rotation_robot()));
      connect(&timer1, SIGNAL(timeout()), this, SLOT(bordure()));
      timer1.start(20);

}

void MyGLWidget::paintGL(){

// lumière
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//lumière



//création figures

    glTranslatef(0.0, -.25, -4.0);
    glRotatef(-30, 1, 0, 0);

// boule
    glPushMatrix();

      glTranslatef(xx, yy, zz+0.35);
      glScalef(0.7,0.7,0.7);
      glColor3d(1,1,1);
      glCallList(sphere);
      glPopMatrix();

      robot_init();


     arene_init();



}


void MyGLWidget::rotation_robot(){


if ((xx>= Xcible-(Rayon_cible_trou*foisCible) && xx<= Xcible+(Rayon_cible_trou*foisCible))  &&  (yy>= Ycible-(Rayon_cible_trou*foisCible) && yy<= Ycible+(Rayon_cible_trou*foisCible)) && actif==1)
{




    xx=Xcible;
    yy=Ycible;
    float PI=3.141592653;
    float angle_rotation_robot;
    float angle_X_bas;
    float angle_X_milieu;
    int ok_rotation=0;
    int ok_bras1=0;
    int ok_bras2=0;
    int mouvement=0;
    int ok_pince=0;


//calcul rotation du robot + angle bras
    if (Xcible>0)
    {
        if (Ycible>0)
        {
            angle_rotation_robot=(180*qAtan(Ycible/Xcible)/PI)+90;

        }
        else
        {
        angle_rotation_robot=-180*qAtan(Xcible/Ycible)/PI;
        }
    }
    else
    {
        if (Ycible>0)
        {
           angle_rotation_robot=(180*qAtan(Ycible/Xcible)/PI)-90;

        }
        else
        {
        angle_rotation_robot=-180*qAtan(Xcible/Ycible)/PI;
        }
    }


    angle_X_bas =90-(180*qAcos((1-((Xcible*Xcible)+(Ycible*Ycible))-0.64)/(-2*qSqrt((Xcible*Xcible)+(Ycible*Ycible))))/PI);
    angle_X_milieu =180-(180*qAcos((((Xcible*Xcible)+(Ycible*Ycible))-1-0.64)/(-1.6))/PI);
    angle_X_bas = qSqrt(angle_X_bas*angle_X_bas);
    angle_X_milieu=qSqrt(angle_X_milieu*angle_X_milieu);

//rotation du robot


while (mouvement<=0)
       {

     if(ok_rotation==0)
     {
        if (angle_rotation_robot>0)
         {
            if(sphereBasZ<angle_rotation_robot)
             {
              sphereBasZ +=1;
              updateGL();
            }
            else
            {
                 ok_rotation=1;
            }
        }
        else
        {
            if(sphereBasZ>angle_rotation_robot)
            {
             sphereBasZ -=1;
             updateGL();
            }
            else
            {
                ok_rotation=1;
            }
        }
    }

     if(ok_bras1==0)
     {
       if(sphereBasX<angle_X_bas)
        {
         sphereBasX += 1 ;
         updateGL();
        }
       else
       {
        ok_bras1=1;
       }
     }

     if (ok_bras2==0)
     {
         if (avantbrasX<angle_X_milieu)
          {
             avantbrasX += 1;
             updateGL();
          }
         else
         {
             ok_bras2=1;

         }
     }

     if (ok_pince==0)
     {
         if (distanceDesDoigts<0.2)
         {
             distanceDesDoigts+=0.01;
         }
         else
         {
             ok_pince=1;
         }
     }

     if (ok_rotation==1 && ok_bras1==1 && ok_bras2==1 && ok_pince==1)
     {
      mouvement = 1;
     }


    }

//récupération de la cible (écart pince)
while(distanceDesDoigts>0.15)
{
    distanceDesDoigts-=0.01;
    updateGL();
}




// rotation du robot vers trou
float distance_polaire;


distance_polaire=qSqrt((Xcible*Xcible)+(Ycible*Ycible));



if (sphereBasZ>-45)
{
         while (sphereBasZ<135)
         {
          sphereBasZ +=1;

          xx=distance_polaire*qCos((sphereBasZ-90)*3.141516/180);
          yy=distance_polaire*qSin((sphereBasZ-90)*3.141516/180);


          updateGL();
        }
}
else
    {
        while (sphereBasZ>-225)
        {
         sphereBasZ -=1;

         xx=distance_polaire*qCos((sphereBasZ-90)*3.141516/180);
         yy=distance_polaire*qSin((sphereBasZ-90)*3.141516/180);



         updateGL();
        }
    }


angle_X_bas =90-(180*qAcos((1-((Xtrou*Xtrou)+(Ytrou*Ytrou))-0.64)/(-2*qSqrt((Xtrou*Xtrou)+(Ytrou*Ytrou))))/PI);
angle_X_bas = qSqrt(angle_X_bas*angle_X_bas);

angle_X_milieu =180-(180*qAcos((((Xtrou*Xtrou)+(Ytrou*Ytrou))-1-0.64)/(-1.6))/PI);
angle_X_milieu=qSqrt(angle_X_milieu*angle_X_milieu);




//rotation du robot
int changement_2=0;


while (changement_2==0)
{


    if (avantbrasX > angle_X_bas)
    {
        avantbrasX-=1;
        sphereBasX+=1;
   }
    else
    {
        avantbrasX+=1;
        sphereBasX-=1;



    }

    float distance;
    distance= 0.8*qCos(PI*(90-sphereBasX)/180)+qSin(PI*(180-avantbrasX-sphereBasX)/180);



    xx= distance * qCos(PI*(sphereBasZ-90)/180);
    yy= distance * qSin(PI*(sphereBasZ-90)/180);

    if ((xx>= Xtrou-(Rayon_cible_trou*foisCible)/8 && xx<= Xtrou+(Rayon_cible_trou*foisCible)/8)  &&  (yy>= Ytrou-Rayon_cible_trou/8 && yy<= Ytrou+Rayon_cible_trou/8))
    {
        changement_2=1;

        xx=1;
        yy=1;



        updateGL();

   }

}

while(zz>-1)
{
zz-=0.1;
updateGL();
}

emit Score_change();


while(distanceDesDoigts<0.2)
{
    distanceDesDoigts+=0.01;

}


while (sphereBasX>0)
{
    sphereBasX-=1;
    if(distanceDesDoigts>0.01)
    {
        distanceDesDoigts-=0.01;

    }
    updateGL();
}

while (avantbrasX>0)
{
    avantbrasX-=1;
    updateGL();
}


if (sphereBasZ<0)
{
while (sphereBasZ>-360)
{
    sphereBasZ-=1;
    updateGL();
}

sphereBasZ=0;

}
else
{
while (sphereBasZ>0)
{
    sphereBasZ -=1;
    updateGL();
}

}


int distance;
int distanceTrou;

this->Xcible = (double)valeurRandX();
this->Ycible = (double)valeurRandY();
distance = sqrt(Xcible*Xcible + Ycible*Ycible);
distanceTrou = sqrt((Xcible-1)*(Xcible-1) + (Ycible-1)*(Ycible-1));

while (distance>1.85 || distance<0.45 ||distanceTrou<0.16)
{
    this->Xcible = (double)valeurRandX();
    this->Ycible = (double)valeurRandY();
    distance = sqrt(Xcible*Xcible + Ycible*Ycible);
    distanceTrou = sqrt((Xcible-1)*(Xcible-1) + (Ycible-1)*(Ycible-1));
}


zz=0;
xx=-1;
yy=-1;
emit bordure_signal();


updateGL();





}

}

void MyGLWidget::bordure()
{
    float distance_robot_arene;
    float distance_trou;
    distance_robot_arene= qSqrt((xx*xx)+(yy*yy));
    distance_trou= qSqrt(((xx-1)*(xx-1))+((yy-1)*(yy-1)));


    //qDebug()<<distance;
    if (distance_robot_arene>=1.85)
    {emit bordure_signal();

        QMessageBox::information(this, "Perdu","Vous avez percuté l'arène: Veuillez recommencer du début") ;
        xx=-1;
        yy=-1;
        random_cible();
    }

    if (distance_robot_arene<=0.45)
    {
        emit bordure_signal();

        QMessageBox::information(this, "Perdu","Vous avez percuté le robot: Veuillez recommencer du début") ;
        xx=-1;
        yy=-1;
        random_cible();
    }


    if (distance_trou<=0.16)
    {
        emit bordure_signal();

        QMessageBox::information(this, "Perdu","Vous êtes tombé dans le trou: Veuillez recommencer du début") ;
        xx=-1;
        yy=-1;
        random_cible();
    }


}

void MyGLWidget::random_cible()
{
    float Xavant;
    float Yavant;

    Xcible = valeurRandX();
    Ycible = valeurRandY();
    float distance= sqrt((Xcible*Xcible)+(Ycible*Ycible));

    while ((Xcible==1 && Ycible==1)|| (Xcible==-1 && Ycible==-1) || distance>2-(Rayon_cible_trou*foisCible) || distance<=0.30+(Rayon_cible_trou*foisCible)|| Xcible==Xavant || Ycible==Yavant )
    {
        Xcible = (double)valeurRandX();
        Ycible = (double)valeurRandY();
        distance= sqrt((Xcible*Xcible)+(Ycible*Ycible));
    }
}


