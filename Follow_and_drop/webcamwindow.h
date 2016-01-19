#ifndef WEBCAMWINDOW_H
#define WEBCAMWINDOW_H

#include <QtWidgets>
#include <QMessageBox>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class WebCamWindow : public QWidget
{
    Q_OBJECT
    
public:
    WebCamWindow(QWidget *parent = 0);
    ~WebCamWindow();
    void displayImage();
    void detectHand();
    void trackHand();
    float X;
    float Y;
    int frameWidth_;
    int frameHeight_;
    void changement_cam_bar();


signals:
   void SignalXBouge();
   void initialisation();



private slots:
    void aquire();
    void startWebCam();





private:


    int templateWidth_;
    int templateHeight_;
    int match_method;
    int etat;
    char* result_window = "Result window";
    QPushButton *webCamButton_;
    QCheckBox *trackCheckBox_;
    QMessageBox msgBox;
    QTimer *timer_;
    QLabel *label_;
    cv::Mat image_;
    cv::Mat imgRoi_;
    cv::Mat result;
    cv::VideoCapture *webcam_;

    void arret();
    int temps_arret;



    };

#endif // WEBCAMWINDOW_H
