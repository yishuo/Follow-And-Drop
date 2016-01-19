#include <QtWidgets>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "webcamwindow.h"

using namespace cv;


WebCamWindow::WebCamWindow(QWidget *parent)
    : QWidget(parent)
{
    etat=0;
    temps_arret=0;
    frameWidth_=320;
    frameHeight_=240;
    templateWidth_=50;
    templateHeight_=50;

    webCamButton_ = new QPushButton(tr("Demarrer aquisition"));
    label_ = new QLabel(tr("Image"));
    trackCheckBox_= new QCheckBox(tr("Jouer"));

    msgBox.setText("Erreur de tracking. Veuillez remettre votre main");
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setVisible(false);

    connect(webCamButton_, SIGNAL(clicked()), this, SLOT(startWebCam()));

    QVBoxLayout *vl1=new QVBoxLayout;
    vl1->addWidget(trackCheckBox_);
    QHBoxLayout *hl=new QHBoxLayout;
    hl->addWidget(webCamButton_);
    hl->addLayout(vl1);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label_);
    layout->addLayout(hl);

    setLayout(layout);
    setWindowTitle(tr("Track WebCam"));

    timer_=new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(aquire()));


 }

WebCamWindow::~WebCamWindow()
{
    delete webcam_;
}

void WebCamWindow::displayImage()
{
    if (image_.data!=NULL)
    {
        cv::flip(image_,image_,1); // process the image_
        // change color channel ordering
        cv::cvtColor(image_,image_,COLOR_BGR2RGB);
        // Qt image_
        QImage img= QImage((const unsigned char*)(image_.data),
                           image_.cols,image_.rows,QImage::Format_RGB888);
        // display on label_
        label_->setPixmap(QPixmap::fromImage(img));
        // resize the label_ to fit the image_
        label_->resize(label_->pixmap()->size());
    }
    else QMessageBox(QMessageBox::Critical,tr("Error"),tr("Image is void")).exec();
}

void WebCamWindow::aquire()
{
    if(webcam_->isOpened())
    {
        if (webcam_->read(image_))
        {
            if (!trackCheckBox_->isChecked()) detectHand();
            if (trackCheckBox_->isChecked()) trackHand();
            displayImage();
        }
    }

}

void WebCamWindow::startWebCam()
{
    if (!timer_->isActive())
    {   emit initialisation();
        webcam_= new cv::VideoCapture(0);
        webcam_->set(CV_CAP_PROP_FRAME_WIDTH,frameWidth_);
        webcam_->set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight_);
        qDebug()<<"width: "<<webcam_->get(CV_CAP_PROP_FRAME_WIDTH);
        qDebug()<<"height: "<<webcam_->get(CV_CAP_PROP_FRAME_HEIGHT);

        timer_->start(20);
        webCamButton_->setText(tr("Arreter aquisition"));


    }
    else
    {
        timer_->stop();
        delete webcam_;
        webcam_=0;
        webCamButton_->setText(tr("Demarrer aquisition"));

    }

}

void WebCamWindow::detectHand()
{
    cv::Rect rectRoi((frameWidth_-templateWidth_)/2,(frameHeight_-templateHeight_)/2,templateWidth_,templateHeight_);

    Mat roi(image_,rectRoi);
    roi.copyTo(imgRoi_);
    waitKey(10);
    rectangle(image_,rectRoi,Scalar( 0, 255, 0),2,8,0);

}

void WebCamWindow::trackHand()
{


     /// Create the result matrix
     int result_cols =  image_.cols - imgRoi_.cols + 1;
     int result_rows = image_.rows - imgRoi_.rows + 1;

     result.create( result_rows, result_cols, CV_32FC1 );

     /// Do the Matching and Normalize
     match_method=CV_TM_CCORR_NORMED;
     matchTemplate( image_, imgRoi_, result, match_method );


     /// Localizing the best match with minMaxLoc
     double minVal; double maxVal; Point minLoc; Point maxLoc;


     minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );


     /// Show me what you got
     if (maxVal>0.975)
        {

             if (maxLoc.x<frameWidth_-(templateWidth_/2)-1 && maxLoc.x>1 && maxLoc.y< frameHeight_ - (templateHeight_/2)-1 && maxLoc.y>1 )
           {

            cv::Rect rectRoi(maxLoc.x,maxLoc.y,templateWidth_,templateHeight_);

            Mat roi(image_,rectRoi);
            roi.copyTo(imgRoi_);



            rectangle( image_,rectRoi,Scalar::all(0), 2, 8, 0 );
            emit SignalXBouge();

                if (etat==1)
                {
                msgBox.setWindowModality(Qt::NonModal);
                msgBox.setVisible(false);
                etat=0;
                }
            }

             else
            {
                if (etat==0)
                     {
                      msgBox.setWindowModality(Qt::NonModal);
                      msgBox.show();
                      arret();
                      etat=1;

                      }

            }
         }


   else
         {
             msgBox.setWindowModality(Qt::NonModal);
             msgBox.show();
             arret();

             etat=1;

         }
    X=maxLoc.x;
    Y=maxLoc.y;

}

void WebCamWindow::arret()
{
    qDebug()<<temps_arret;
    temps_arret+=1;

    if(temps_arret>=50)
    {
        trackCheckBox_->setCheckState(Qt::Unchecked);
        msgBox.setWindowModality(Qt::NonModal);
        msgBox.setVisible(false);
        etat=0;
        temps_arret=0;

        QMessageBox txt;
        txt.setText("La main n'a pas été détecté depuis trop longtemps. Veuillez recommencer l'initialisation");
        txt.exec();
    }

}

void WebCamWindow::changement_cam_bar()
{
    trackCheckBox_->setCheckState(Qt::Unchecked);
}
