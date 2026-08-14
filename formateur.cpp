#include "formateur.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <utility>

// ======================================================
// PDF
// ======================================================

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>
#include <QFont>
#include <QDateTime>


// ======================================================
// CONSTRUCTEUR PAR DEFAUT
// ======================================================

Formateur::Formateur()
{
    idFormateur = 0;
}


// ======================================================
// CONSTRUCTEUR AVEC PARAMETRES
// ======================================================

Formateur::Formateur(const QString &n,
                     const QString &p,
                     const QString &e,
                     const QString &t,
                     const QString &s,
                     const QDate &d)
{
    idFormateur = 0;

    nom = n;
    prenom = p;
    email = e;
    telephone = t;
    specialite = s;
    dateEmbauche = d;
}


// ======================================================
// GETTERS
// ======================================================

int Formateur::getId() const
{
    return idFormateur;
}

QString Formateur::getNom() const
{
    return nom;
}

QString Formateur::getPrenom() const
{
    return prenom;
}

QString Formateur::getEmail() const
{
    return email;
}

QString Formateur::getTelephone() const
{
    return telephone;
}

QString Formateur::getSpecialite() const
{
    return specialite;
}

QDate Formateur::getDateEmbauche() const
{
    return dateEmbauche;
}


// ======================================================
// 1. AJOUTER UN FORMATEUR
// ======================================================

bool Formateur::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO FORMATEUR "
        "(NOM, PRENOM, EMAIL, TELEPHONE, SPECIALITE, DATEEMBAUCHE) "
        "VALUES "
        "(:nom, :prenom, :email, :telephone, :specialite, :dateEmbauche)"
        );

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);
    query.bindValue(":dateEmbauche", dateEmbauche);

    if (query.exec())
    {
        qDebug() << "Formateur ajouté avec succès.";
        return true;
    }

    qDebug() << "Erreur ajout formateur :"
             << query.lastError().text();

    return false;
}


// ======================================================
// 2. AFFICHER TOUS LES FORMATEURS
// ======================================================

QSqlQueryModel* Formateur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "IDFORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "DATEEMBAUCHE "
        "FROM FORMATEUR "
        "ORDER BY IDFORMATEUR"
        );

    if (!query.exec())
    {
        qDebug() << "Erreur affichage formateurs :"
                 << query.lastError().text();

        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");
    model->setHeaderData(6, Qt::Horizontal, "Date d'embauche");

    return model;
}


// ======================================================
// 3. SUPPRIMER UN FORMATEUR
// ======================================================

bool Formateur::supprimer(int id)
{
    // ==================================================
    // VERIFIER SI LE FORMATEUR EST LIE A UN COURS
    // ==================================================

    QSqlQuery verification;

    verification.prepare(
        "SELECT COUNT(*) "
        "FROM COURS "
        "WHERE IDFORMATEUR = :id"
        );

    verification.bindValue(":id", id);

    if (!verification.exec())
    {
        qDebug() << "Erreur vérification cours :"
                 << verification.lastError().text();

        return false;
    }

    if (verification.next())
    {
        int nombreCours =
            verification.value(0).toInt();

        if (nombreCours > 0)
        {
            qDebug()
            << "Suppression impossible :"
            << "le formateur est lié à"
            << nombreCours
            << "cours.";

            return false;
        }
    }


    // ==================================================
    // SUPPRIMER LE FORMATEUR
    // ==================================================

    QSqlQuery query;

    query.prepare(
        "DELETE FROM FORMATEUR "
        "WHERE IDFORMATEUR = :id"
        );

    query.bindValue(":id", id);

    if (query.exec())
    {
        qDebug()
        << "Formateur supprimé avec succès.";

        return true;
    }

    qDebug()
        << "Erreur suppression formateur :"
        << query.lastError().text();

    return false;
}


// ======================================================
// 4. MODIFIER UN FORMATEUR
// ======================================================

bool Formateur::modifier(int id)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE FORMATEUR SET "
        "NOM = :nom, "
        "PRENOM = :prenom, "
        "EMAIL = :email, "
        "TELEPHONE = :telephone, "
        "SPECIALITE = :specialite, "
        "DATEEMBAUCHE = :dateEmbauche "
        "WHERE IDFORMATEUR = :id"
        );

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);
    query.bindValue(":dateEmbauche", dateEmbauche);

    if (query.exec())
    {
        qDebug()
        << "Formateur modifié avec succès.";

        return true;
    }

    qDebug()
        << "Erreur modification formateur :"
        << query.lastError().text();

    return false;
}


// ======================================================
// 5. RECHERCHE MULTICRITERES FORMATEUR
// Critères : Nom + Spécialité + Date minimum
// ======================================================

QSqlQueryModel* Formateur::rechercher(
    const QString &nomRecherche,
    const QString &specialiteRecherche,
    const QDate &dateMin)
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    QString requete =
        "SELECT "
        "IDFORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "DATEEMBAUCHE "
        "FROM FORMATEUR "
        "WHERE 1=1 ";


    // ==================================================
    // CRITERE 1 : NOM
    // ==================================================

    if (!nomRecherche.trimmed().isEmpty())
    {
        requete +=
            "AND UPPER(NOM) LIKE UPPER(:nomRecherche) ";
    }


    // ==================================================
    // CRITERE 2 : SPECIALITE
    // ==================================================

    if (!specialiteRecherche.trimmed().isEmpty())
    {
        requete +=
            "AND UPPER(SPECIALITE) LIKE UPPER(:specialiteRecherche) ";
    }


    // ==================================================
    // CRITERE 3 : DATE EMBAUCHE MINIMALE
    // ==================================================

    if (dateMin.isValid())
    {
        requete +=
            "AND DATEEMBAUCHE >= :dateMin ";
    }


    requete +=
        "ORDER BY IDFORMATEUR";


    // ==================================================
    // PREPARATION
    // ==================================================

    query.prepare(requete);


    // ==================================================
    // BIND NOM
    // ==================================================

    if (!nomRecherche.trimmed().isEmpty())
    {
        query.bindValue(
            ":nomRecherche",
            "%" + nomRecherche.trimmed() + "%"
            );
    }


    // ==================================================
    // BIND SPECIALITE
    // ==================================================

    if (!specialiteRecherche.trimmed().isEmpty())
    {
        query.bindValue(
            ":specialiteRecherche",
            "%" + specialiteRecherche.trimmed() + "%"
            );
    }


    // ==================================================
    // BIND DATE
    // ==================================================

    if (dateMin.isValid())
    {
        query.bindValue(
            ":dateMin",
            dateMin
            );
    }


    // ==================================================
    // EXECUTION
    // ==================================================

    if (!query.exec())
    {
        qDebug()
        << "Erreur recherche formateur :"
        << query.lastError().text();

        delete model;

        return nullptr;
    }


    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");
    model->setHeaderData(6, Qt::Horizontal, "Date d'embauche");

    return model;
}


// ======================================================
// 6. TRI FORMATEUR
// ======================================================

QSqlQueryModel* Formateur::trier(
    const QString &critere,
    const QString &ordre)
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QString colonne;


    // ==================================================
    // CHOIX DE LA COLONNE
    // ==================================================

    if (critere == "Nom")
    {
        colonne = "NOM";
    }
    else if (critere == "Spécialité")
    {
        colonne = "SPECIALITE";
    }
    else if (critere == "Date d'embauche")
    {
        colonne = "DATEEMBAUCHE";
    }
    else
    {
        colonne = "IDFORMATEUR";
    }


    // ==================================================
    // CHOIX DE L'ORDRE
    // ==================================================

    QString ordreSQL;

    if (ordre == "Décroissant")
    {
        ordreSQL = "DESC";
    }
    else
    {
        ordreSQL = "ASC";
    }


    // ==================================================
    // REQUETE
    // ==================================================

    QString requete =
        "SELECT "
        "IDFORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "DATEEMBAUCHE "
        "FROM FORMATEUR "
        "ORDER BY "
        + colonne
        + " "
        + ordreSQL;


    QSqlQuery query;

    query.prepare(requete);


    // ==================================================
    // EXECUTION
    // ==================================================

    if (!query.exec())
    {
        qDebug()
        << "Erreur tri formateur :"
        << query.lastError().text();

        delete model;

        return nullptr;
    }


    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");
    model->setHeaderData(6, Qt::Horizontal, "Date d'embauche");

    return model;
}


// ======================================================
// 7. STATISTIQUES FORMATEURS PAR SPECIALITE
// ======================================================

QSqlQueryModel* Formateur::statistiquesParSpecialite()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "SPECIALITE, "
        "COUNT(*) AS NOMBRE "
        "FROM FORMATEUR "
        "GROUP BY SPECIALITE "
        "ORDER BY SPECIALITE"
        );


    if (!query.exec())
    {
        qDebug()
        << "Erreur statistiques formateurs :"
        << query.lastError().text();

        delete model;

        return nullptr;
    }


    model->setQuery(std::move(query));


    model->setHeaderData(
        0,
        Qt::Horizontal,
        "Spécialité"
        );


    model->setHeaderData(
        1,
        Qt::Horizontal,
        "Nombre de formateurs"
        );


    return model;
}


// ======================================================
// 8. GENERER PDF FORMATEURS
// ======================================================

bool Formateur::genererPdf(
    const QString &cheminFichier)
{
    // ==================================================
    // RECUPERER LES FORMATEURS
    // ==================================================

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "IDFORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "DATEEMBAUCHE "
        "FROM FORMATEUR "
        "ORDER BY IDFORMATEUR"
        );


    if (!query.exec())
    {
        qDebug()
        << "Erreur récupération formateurs pour PDF :"
        << query.lastError().text();

        return false;
    }


    // ==================================================
    // CREER LE PDF
    // ==================================================

    QPdfWriter pdfWriter(
        cheminFichier
        );


    pdfWriter.setPageSize(
        QPageSize(QPageSize::A4)
        );


    pdfWriter.setPageOrientation(
        QPageLayout::Landscape
        );


    pdfWriter.setPageMargins(
        QMarginsF(10, 10, 10, 10),
        QPageLayout::Millimeter
        );


    pdfWriter.setResolution(300);


    pdfWriter.setTitle(
        "Liste des formateurs"
        );


    pdfWriter.setCreator(
        "Centre de Formation"
        );


    // ==================================================
    // PAINTER
    // ==================================================

    QPainter painter(
        &pdfWriter
        );


    if (!painter.isActive())
    {
        qDebug()
        << "Impossible de créer le fichier PDF.";

        return false;
    }


    // ==================================================
    // DIMENSIONS
    // ==================================================

    int largeurPage =
        pdfWriter.width();

    int marge = 200;

    int y = 250;


    // ==================================================
    // TITRE
    // ==================================================

    QFont policeTitre(
        "Arial",
        18,
        QFont::Bold
        );


    painter.setFont(
        policeTitre
        );


    painter.drawText(
        QRect(
            marge,
            y,
            largeurPage - (2 * marge),
            300
            ),
        Qt::AlignCenter,
        "CENTRE DE FORMATION"
        );


    y += 350;


    QFont policeSousTitre(
        "Arial",
        15,
        QFont::Bold
        );


    painter.setFont(
        policeSousTitre
        );


    painter.drawText(
        QRect(
            marge,
            y,
            largeurPage - (2 * marge),
            300
            ),
        Qt::AlignCenter,
        "LISTE DES FORMATEURS"
        );


    y += 400;


    // ==================================================
    // DATE DE GENERATION
    // ==================================================

    QFont policeInfo(
        "Arial",
        9
        );


    painter.setFont(
        policeInfo
        );


    QString dateGeneration =
        QDateTime::currentDateTime()
            .toString(
                "dd/MM/yyyy à HH:mm"
                );


    painter.drawText(
        marge,
        y,
        "Date de génération : "
            + dateGeneration
        );


    y += 300;


    // ==================================================
    // COMPTER LE NOMBRE DE FORMATEURS
    // ==================================================

    int totalFormateurs = 0;

    QSqlQuery queryTotal;

    queryTotal.prepare(
        "SELECT COUNT(*) "
        "FROM FORMATEUR"
        );


    if (queryTotal.exec() &&
        queryTotal.next())
    {
        totalFormateurs =
            queryTotal.value(0).toInt();
    }


    QFont policeTotal(
        "Arial",
        10,
        QFont::Bold
        );


    painter.setFont(
        policeTotal
        );


    painter.drawText(
        marge,
        y,
        "Nombre total de formateurs : "
            + QString::number(
                totalFormateurs
                )
        );


    y += 400;


    // ==================================================
    // TABLEAU
    // ==================================================

    int hauteurLigne = 300;


    // Largeurs des colonnes
    int largeurID = 500;
    int largeurNom = 1100;
    int largeurPrenom = 1100;
    int largeurEmail = 2200;
    int largeurTel = 1200;
    int largeurSpecialite = 1800;
    int largeurDate = 1500;


    int x = marge;


    // ==================================================
    // ENTETE DU TABLEAU
    // ==================================================

    QFont policeEntete(
        "Arial",
        8,
        QFont::Bold
        );


    painter.setFont(
        policeEntete
        );


    painter.drawRect(
        x,
        y,
        largeurID,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurID,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "ID"
        );

    x += largeurID;


    painter.drawRect(
        x,
        y,
        largeurNom,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurNom,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Nom"
        );

    x += largeurNom;


    painter.drawRect(
        x,
        y,
        largeurPrenom,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurPrenom,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Prénom"
        );

    x += largeurPrenom;


    painter.drawRect(
        x,
        y,
        largeurEmail,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurEmail,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Email"
        );

    x += largeurEmail;


    painter.drawRect(
        x,
        y,
        largeurTel,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurTel,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Téléphone"
        );

    x += largeurTel;


    painter.drawRect(
        x,
        y,
        largeurSpecialite,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurSpecialite,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Spécialité"
        );

    x += largeurSpecialite;


    painter.drawRect(
        x,
        y,
        largeurDate,
        hauteurLigne
        );

    painter.drawText(
        QRect(
            x,
            y,
            largeurDate,
            hauteurLigne
            ),
        Qt::AlignCenter,
        "Date d'embauche"
        );


    y += hauteurLigne;


    // ==================================================
    // DONNEES DU TABLEAU
    // ==================================================

    QFont policeDonnees(
        "Arial",
        7
        );


    painter.setFont(
        policeDonnees
        );


    while (query.next())
    {
        // ==============================================
        // NOUVELLE PAGE SI NECESSAIRE
        // ==============================================

        if (y + hauteurLigne >
            pdfWriter.height() - 400)
        {
            pdfWriter.newPage();

            y = 300;


            // ==========================================
            // REPETER L'ENTETE
            // ==========================================

            x = marge;

            painter.setFont(
                policeEntete
                );


            painter.drawRect(
                x,
                y,
                largeurID,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurID,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "ID"
                );

            x += largeurID;


            painter.drawRect(
                x,
                y,
                largeurNom,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurNom,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Nom"
                );

            x += largeurNom;


            painter.drawRect(
                x,
                y,
                largeurPrenom,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurPrenom,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Prénom"
                );

            x += largeurPrenom;


            painter.drawRect(
                x,
                y,
                largeurEmail,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurEmail,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Email"
                );

            x += largeurEmail;


            painter.drawRect(
                x,
                y,
                largeurTel,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurTel,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Téléphone"
                );

            x += largeurTel;


            painter.drawRect(
                x,
                y,
                largeurSpecialite,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurSpecialite,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Spécialité"
                );

            x += largeurSpecialite;


            painter.drawRect(
                x,
                y,
                largeurDate,
                hauteurLigne
                );

            painter.drawText(
                QRect(
                    x,
                    y,
                    largeurDate,
                    hauteurLigne
                    ),
                Qt::AlignCenter,
                "Date d'embauche"
                );


            y += hauteurLigne;

            painter.setFont(
                policeDonnees
                );
        }


        // ==============================================
        // RECUPERER LES DONNEES
        // ==============================================

        QString id =
            query.value(0).toString();

        QString nomFormateur =
            query.value(1).toString();

        QString prenomFormateur =
            query.value(2).toString();

        QString emailFormateur =
            query.value(3).toString();

        QString telephoneFormateur =
            query.value(4).toString();

        QString specialiteFormateur =
            query.value(5).toString();


        QDate date =
            query.value(6).toDate();


        QString dateFormatee;

        if (date.isValid())
        {
            dateFormatee =
                date.toString(
                    "dd/MM/yyyy"
                    );
        }


        // ==============================================
        // DESSINER LA LIGNE
        // ==============================================

        x = marge;


        painter.drawRect(
            x,
            y,
            largeurID,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurID - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            id
            );

        x += largeurID;


        painter.drawRect(
            x,
            y,
            largeurNom,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurNom - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            nomFormateur
            );

        x += largeurNom;


        painter.drawRect(
            x,
            y,
            largeurPrenom,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurPrenom - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            prenomFormateur
            );

        x += largeurPrenom;


        painter.drawRect(
            x,
            y,
            largeurEmail,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurEmail - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            emailFormateur
            );

        x += largeurEmail;


        painter.drawRect(
            x,
            y,
            largeurTel,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurTel - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            telephoneFormateur
            );

        x += largeurTel;


        painter.drawRect(
            x,
            y,
            largeurSpecialite,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurSpecialite - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            specialiteFormateur
            );

        x += largeurSpecialite;


        painter.drawRect(
            x,
            y,
            largeurDate,
            hauteurLigne
            );

        painter.drawText(
            QRect(
                x + 10,
                y,
                largeurDate - 20,
                hauteurLigne
                ),
            Qt::AlignCenter,
            dateFormatee
            );


        y += hauteurLigne;
    }


    // ==================================================
    // TERMINER LE PDF
    // ==================================================

    painter.end();


    qDebug()
        << "PDF des formateurs généré avec succès :"
        << cheminFichier;


    return true;
}
