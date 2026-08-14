#include "connection.h"

#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    db = QSqlDatabase::addDatabase("QODBC");

    // Nom EXACT du DSN configuré dans Windows
    db.setDatabaseName("Source_Oracle");

    // Utilisateur Oracle
    db.setUserName("system");

    // Mot de passe Oracle
    db.setPassword("admin123");

    if (db.open())
    {
        qDebug() << "====================================";
        qDebug() << "Connexion Oracle réussie !";
        qDebug() << "====================================";
        QSqlQuery testQuery;

        if (testQuery.exec("SELECT USER FROM DUAL"))
        {
            if (testQuery.next())
            {
                qDebug() << "Utilisateur Oracle Qt :"
                         << testQuery.value(0).toString();
            }
        }

        if (testQuery.exec(
                "SELECT SYS_CONTEXT('USERENV','CURRENT_SCHEMA') FROM DUAL"))
        {
            if (testQuery.next())
            {
                qDebug() << "Schema Oracle Qt :"
                         << testQuery.value(0).toString();
            }
        }

        return true;
    }
    else
    {
        qDebug() << "====================================";
        qDebug() << "ERREUR DE CONNEXION ORACLE";
        qDebug() << "====================================";
        qDebug() << "Driver :" << db.driverName();
        qDebug() << "Base :" << db.databaseName();
        qDebug() << "Utilisateur :" << db.userName();
        qDebug() << "Erreur :" << db.lastError().text();

        QMessageBox::critical(
            nullptr,
            "Erreur Oracle",
            "Impossible de se connecter à Oracle.\n\n"
            "Erreur détaillée :\n" +
                db.lastError().text()
            );

        return false;
    }
}

void Connection::closeConnection()
{
    if (db.isOpen())
    {
        db.close();
    }
}
