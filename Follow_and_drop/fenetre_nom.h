#ifndef FENETRE_NOM
#define FENETRE_NOM

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QInputDialog>

class Fenetre_Nom : public QWidget
{
    Q_OBJECT

    public:
    Fenetre_Nom();
    QString Nom_Joueur;

    public slots:


    private:
    void ouvrirDialog();

};



#endif // FENETRE_NOM

