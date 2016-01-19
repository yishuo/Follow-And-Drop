
// main.cpp

#include <QApplication>
#include <QDesktopWidget>

#include "fenetre_nom.h"

Fenetre_Nom::Fenetre_Nom() : QWidget()
{
    setFixedSize(230, 120);
    ouvrirDialog();

   // QObject::connect(m_boutonDialogue, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
}


void Fenetre_Nom::ouvrirDialog()
{

    Nom_Joueur = QInputDialog::getText(this, "Nom Joueur", "Quel est votre nom ?");

    if (!Nom_Joueur.isEmpty())
    {

    }
    else
    {
        QMessageBox::critical(this, "Nom", "Vous n'avez pas voulu donner votre nom. Vous allez quitter l'application. Au revoir!");
    }
}
