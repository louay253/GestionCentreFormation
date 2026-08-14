#ifndef STAGIAIRE_H
#define STAGIAIRE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Stagiaire
{
private:

    int idStagiaire;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    int age;
    QString niveau;
    QDate dateInscription;


public:

    // ==================================================
    // CONSTRUCTEURS
    // ==================================================

    Stagiaire();

    Stagiaire(const QString &n,
              const QString &p,
              const QString &e,
              const QString &t,
              int a,
              const QString &niv,
              const QDate &di);


    // ==================================================
    // GETTERS
    // ==================================================

    int getId() const
    {
        return idStagiaire;
    }

    QString getNom() const
    {
        return nom;
    }

    QString getPrenom() const
    {
        return prenom;
    }

    QString getEmail() const
    {
        return email;
    }

    QString getTelephone() const
    {
        return telephone;
    }

    int getAge() const
    {
        return age;
    }

    QString getNiveau() const
    {
        return niveau;
    }

    QDate getDateInscription() const
    {
        return dateInscription;
    }


    // ==================================================
    // CRUD
    // ==================================================

    bool ajouter();

    QSqlQueryModel* afficher();

    bool supprimer(int id);

    bool modifier(int id);


    // ==================================================
    // METIERS
    // ==================================================

    // Recherche multicritères :
    // Nom + Niveau + Age minimum
    QSqlQueryModel* rechercher(
        const QString &nomRecherche,
        const QString &niveauRecherche,
        int ageMin
        );


    // Tri :
    // Nom / Age / Niveau / Date inscription
    QSqlQueryModel* trier(
        const QString &critere,
        const QString &ordre
        );


    // ==================================================
    // STATISTIQUES
    // ==================================================

    // Retourne :
    // NIVEAU | NOMBRE
    QSqlQueryModel* statistiquesParNiveau();


    // ==================================================
    // GENERATION PDF
    // ==================================================

    // Génère un PDF personnalisé contenant
    // la liste complète des stagiaires
    bool genererPdf(
        const QString &cheminFichier
        );


    // ==================================================
    // QR CODE
    // ==================================================

    // Récupère les informations d'un stagiaire
    // depuis Oracle et construit le texte
    // qui sera encodé dans le QR Code
    QString contenuQrCode(
        int id
        );
};

#endif // STAGIAIRE_H
