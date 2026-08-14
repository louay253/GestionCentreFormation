#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ============================================
    // CONNEXION UNIQUE A LA BASE DE DONNEES
    // ============================================

    Connection c;

    if (!c.createconnect())
    {
        QMessageBox::critical(
            nullptr,
            "Erreur de connexion",
            "Impossible de se connecter à la base de données Oracle."
            );

        return -1;
    }

    // ============================================
    // LANCEMENT DE L'APPLICATION
    // ============================================

    MainWindow w;
    w.show();

    return a.exec();
}
