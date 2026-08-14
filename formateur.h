#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Formateur
{
private:
    int idFormateur;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QString specialite;
    QDate dateEmbauche;

public:

    // ==================================================
    // CONSTRUCTEURS
    // ==================================================

    Formateur();

    Formateur(const QString &n,
              const QString &p,
              const QString &e,
              const QString &t,
              const QString &s,
              const QDate &d);


    // ==================================================
    // GETTERS
    // ==================================================

    int getId() const;

    QString getNom() const;

    QString getPrenom() const;

    QString getEmail() const;

    QString getTelephone() const;

    QString getSpecialite() const;

    QDate getDateEmbauche() const;


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
    // Nom + Spécialité + Date d'embauche minimale
    QSqlQueryModel* rechercher(
        const QString &nomRecherche,
        const QString &specialiteRecherche,
        const QDate &dateMin
        );


    // Tri :
    // Nom / Spécialité / Date d'embauche
    QSqlQueryModel* trier(
        const QString &critere,
        const QString &ordre
        );


    // ==================================================
    // STATISTIQUES
    // ==================================================

    // Retourne :
    // SPECIALITE | NOMBRE
    QSqlQueryModel* statistiquesParSpecialite();


    // ==================================================
    // GENERATION PDF
    // ==================================================

    // Génère un PDF contenant la liste des formateurs
    bool genererPdf(
        const QString &cheminFichier
        );
};

#endif // FORMATEUR_H
