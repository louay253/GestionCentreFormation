#include "stagiaire.h"

#include <utility>

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

#include <QDebug>
#include <QDate>

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QFont>


// ======================================================
// CONSTRUCTEUR PAR DEFAUT
// ======================================================

Stagiaire::Stagiaire()
{
    idStagiaire = 0;
    age = 0;
}


// ======================================================
// CONSTRUCTEUR AVEC PARAMETRES
// ======================================================

Stagiaire::Stagiaire(const QString &n,
                     const QString &p,
                     const QString &e,
                     const QString &t,
                     int a,
                     const QString &niv,
                     const QDate &di)
{
    idStagiaire = 0;

    nom = n;
    prenom = p;
    email = e;
    telephone = t;
    age = a;
    niveau = niv;
    dateInscription = di;
}


// ======================================================
// 1. AJOUTER UN STAGIAIRE
// ======================================================

bool Stagiaire::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO STAGIAIRE "
        "(NOM, PRENOM, EMAIL, TELEPHONE, AGE, NIVEAU, DATEINSCRIPTION) "
        "VALUES "
        "(:nom, :prenom, :email, :telephone, :age, :niveau, :dateInscription)"
        );

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":niveau", niveau);
    query.bindValue(":dateInscription", dateInscription);

    if (query.exec())
    {
        qDebug() << "Stagiaire ajouté avec succès.";
        return true;
    }

    qDebug() << "Erreur ajout stagiaire :"
             << query.lastError().text();

    return false;
}


// ======================================================
// 2. AFFICHER TOUS LES STAGIAIRES
// ======================================================

QSqlQueryModel* Stagiaire::afficher()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "IDSTAGIAIRE, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "AGE, "
        "NIVEAU, "
        "DATEINSCRIPTION "
        "FROM STAGIAIRE "
        "ORDER BY IDSTAGIAIRE"
        );

    if (!query.exec())
    {
        qDebug() << "Erreur affichage stagiaires :"
                 << query.lastError().text();

        delete model;
        return nullptr;
    }

    model->setQuery(
        std::move(query)
        );

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Âge");
    model->setHeaderData(6, Qt::Horizontal, "Niveau");
    model->setHeaderData(7, Qt::Horizontal, "Date d'inscription");

    return model;
}


// ======================================================
// 3. SUPPRIMER UN STAGIAIRE
// ======================================================

bool Stagiaire::supprimer(int id)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM STAGIAIRE "
        "WHERE IDSTAGIAIRE = :id"
        );

    query.bindValue(
        ":id",
        id
        );

    if (query.exec())
    {
        qDebug() << "Stagiaire supprimé avec succès.";
        return true;
    }

    qDebug() << "Erreur suppression stagiaire :"
             << query.lastError().text();

    return false;
}


// ======================================================
// 4. MODIFIER UN STAGIAIRE
// ======================================================

bool Stagiaire::modifier(int id)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE STAGIAIRE SET "
        "NOM = :nom, "
        "PRENOM = :prenom, "
        "EMAIL = :email, "
        "TELEPHONE = :telephone, "
        "AGE = :age, "
        "NIVEAU = :niveau, "
        "DATEINSCRIPTION = :dateInscription "
        "WHERE IDSTAGIAIRE = :id"
        );

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":age", age);
    query.bindValue(":niveau", niveau);
    query.bindValue(":dateInscription", dateInscription);

    if (query.exec())
    {
        qDebug() << "Stagiaire modifié avec succès.";
        return true;
    }

    qDebug() << "Erreur modification stagiaire :"
             << query.lastError().text();

    return false;
}


// ======================================================
// 5. RECHERCHE MULTICRITERES
// Nom + Niveau + Age minimum
// ======================================================

QSqlQueryModel* Stagiaire::rechercher(
    const QString &nomRecherche,
    const QString &niveauRecherche,
    int ageMin)
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    QString requete =
        "SELECT "
        "IDSTAGIAIRE, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "AGE, "
        "NIVEAU, "
        "DATEINSCRIPTION "
        "FROM STAGIAIRE "
        "WHERE 1=1 ";


    // Critère 1 : nom
    if (!nomRecherche.trimmed().isEmpty())
    {
        requete +=
            "AND UPPER(NOM) LIKE UPPER(:nomRecherche) ";
    }


    // Critère 2 : niveau
    if (!niveauRecherche.isEmpty() &&
        niveauRecherche != "Tous")
    {
        requete +=
            "AND NIVEAU = :niveauRecherche ";
    }


    // Critère 3 : âge minimum
    if (ageMin > 0)
    {
        requete +=
            "AND AGE >= :ageMin ";
    }


    requete +=
        "ORDER BY IDSTAGIAIRE";


    query.prepare(requete);


    if (!nomRecherche.trimmed().isEmpty())
    {
        query.bindValue(
            ":nomRecherche",
            "%" + nomRecherche.trimmed() + "%"
            );
    }


    if (!niveauRecherche.isEmpty() &&
        niveauRecherche != "Tous")
    {
        query.bindValue(
            ":niveauRecherche",
            niveauRecherche
            );
    }


    if (ageMin > 0)
    {
        query.bindValue(
            ":ageMin",
            ageMin
            );
    }


    if (!query.exec())
    {
        qDebug() << "Erreur recherche stagiaire :"
                 << query.lastError().text();

        delete model;
        return nullptr;
    }


    model->setQuery(
        std::move(query)
        );

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Âge");
    model->setHeaderData(6, Qt::Horizontal, "Niveau");
    model->setHeaderData(7, Qt::Horizontal, "Date d'inscription");

    return model;
}


// ======================================================
// 6. TRI DES STAGIAIRES
// ======================================================

QSqlQueryModel* Stagiaire::trier(
    const QString &critere,
    const QString &ordre)
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    // ==================================================
    // CHOIX COLONNE
    // ==================================================

    QString colonne;


    if (critere == "Nom")
    {
        colonne = "NOM";
    }
    else if (critere == "Âge")
    {
        colonne = "AGE";
    }
    else if (critere == "Niveau")
    {
        colonne = "NIVEAU";
    }
    else if (critere == "Date inscription")
    {
        colonne = "DATEINSCRIPTION";
    }
    else
    {
        colonne = "IDSTAGIAIRE";
    }


    // ==================================================
    // ORDRE
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
        "IDSTAGIAIRE, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "AGE, "
        "NIVEAU, "
        "DATEINSCRIPTION "
        "FROM STAGIAIRE "
        "ORDER BY "
        + colonne
        + " "
        + ordreSQL;


    QSqlQuery query;

    query.prepare(requete);


    if (!query.exec())
    {
        qDebug() << "Erreur tri stagiaire :"
                 << query.lastError().text();

        delete model;
        return nullptr;
    }


    model->setQuery(
        std::move(query)
        );

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Âge");
    model->setHeaderData(6, Qt::Horizontal, "Niveau");
    model->setHeaderData(7, Qt::Horizontal, "Date d'inscription");


    return model;
}


// ======================================================
// 7. STATISTIQUES PAR NIVEAU
// ======================================================

QSqlQueryModel* Stagiaire::statistiquesParNiveau()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "NIVEAU, "
        "COUNT(*) AS NOMBRE "
        "FROM STAGIAIRE "
        "GROUP BY NIVEAU "
        "ORDER BY NIVEAU"
        );


    if (!query.exec())
    {
        qDebug() << "Erreur statistiques stagiaires :"
                 << query.lastError().text();

        delete model;
        return nullptr;
    }


    model->setQuery(
        std::move(query)
        );

    model->setHeaderData(
        0,
        Qt::Horizontal,
        "Niveau"
        );

    model->setHeaderData(
        1,
        Qt::Horizontal,
        "Nombre de stagiaires"
        );


    return model;
}


// ======================================================
// 8. GENERATION PDF PERSONNALISE
// ======================================================

bool Stagiaire::genererPdf(
    const QString &cheminFichier)
{
    // ==================================================
    // RECUPERER LES STAGIAIRES
    // ==================================================

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "IDSTAGIAIRE, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "AGE, "
        "NIVEAU, "
        "DATEINSCRIPTION "
        "FROM STAGIAIRE "
        "ORDER BY IDSTAGIAIRE"
        );


    if (!query.exec())
    {
        qDebug()
        << "Erreur récupération stagiaires PDF :"
        << query.lastError().text();

        return false;
    }


    // ==================================================
    // STOCKER LES DONNEES
    // ==================================================

    struct LigneStagiaire
    {
        QString id;
        QString nom;
        QString prenom;
        QString email;
        QString telephone;
        QString age;
        QString niveau;
        QString dateInscription;
    };


    QList<LigneStagiaire> stagiaires;


    while (query.next())
    {
        LigneStagiaire ligne;

        ligne.id =
            query.value(0).toString();

        ligne.nom =
            query.value(1).toString();

        ligne.prenom =
            query.value(2).toString();

        ligne.email =
            query.value(3).toString();

        ligne.telephone =
            query.value(4).toString();

        ligne.age =
            query.value(5).toString();

        ligne.niveau =
            query.value(6).toString();

        if (ligne.niveau.trimmed().isEmpty())
        {
            ligne.niveau =
                "Non défini";
        }


        QDate date =
            query.value(7).toDate();

        if (date.isValid())
        {
            ligne.dateInscription =
                date.toString("dd/MM/yyyy");
        }


        stagiaires.append(
            ligne
            );
    }


    // ==================================================
    // CREATION DU PDF
    // ==================================================

    QPdfWriter pdf(
        cheminFichier
        );


    pdf.setPageSize(
        QPageSize(QPageSize::A4)
        );

    pdf.setResolution(
        300
        );

    pdf.setTitle(
        "Liste des stagiaires"
        );

    pdf.setCreator(
        "Gestion Centre Formation"
        );


    QPainter painter(
        &pdf
        );


    if (!painter.isActive())
    {
        qDebug()
        << "Impossible de créer le PDF.";

        return false;
    }


    // ==================================================
    // PARAMETRES GENERAUX
    // ==================================================

    const int largeurPage =
        pdf.width();

    const int hauteurPage =
        pdf.height();

    const int margeGauche =
        250;

    int y =
        250;


    // ==================================================
    // TITRE PRINCIPAL
    // ==================================================

    QFont titreFont(
        "Arial",
        18
        );

    titreFont.setBold(
        true
        );

    painter.setFont(
        titreFont
        );


    painter.drawText(
        QRect(
            0,
            y,
            largeurPage,
            180
            ),
        Qt::AlignCenter,
        "CENTRE DE FORMATION"
        );


    y += 230;


    // ==================================================
    // SOUS TITRE
    // ==================================================

    QFont sousTitreFont(
        "Arial",
        14
        );

    sousTitreFont.setBold(
        true
        );

    painter.setFont(
        sousTitreFont
        );


    painter.drawText(
        QRect(
            0,
            y,
            largeurPage,
            160
            ),
        Qt::AlignCenter,
        "LISTE DES STAGIAIRES"
        );


    y += 230;


    // ==================================================
    // INFORMATIONS
    // ==================================================

    QFont infoFont(
        "Arial",
        9
        );

    painter.setFont(
        infoFont
        );


    painter.drawText(
        margeGauche,
        y,
        "Date de génération : "
            + QDate::currentDate()
                  .toString("dd/MM/yyyy")
        );


    y += 150;


    painter.drawText(
        margeGauche,
        y,
        "Nombre total de stagiaires : "
            + QString::number(
                stagiaires.size()
                )
        );


    y += 250;


    // ==================================================
    // TABLEAU
    // ==================================================

    const int hauteurLigne =
        180;

    const int xDepart =
        150;


    const int largeurID =
        250;

    const int largeurNom =
        600;

    const int largeurPrenom =
        650;

    const int largeurEmail =
        1200;

    const int largeurTel =
        700;

    const int largeurAge =
        300;

    const int largeurNiveau =
        750;

    const int largeurDate =
        850;


    auto dessinerCellule =
        [&](int x,
            int positionY,
            int largeur,
            const QString &texte)
    {
        QRect rect(
            x,
            positionY,
            largeur,
            hauteurLigne
            );


        painter.drawRect(
            rect
            );


        painter.drawText(
            rect.adjusted(
                10,
                5,
                -10,
                -5
                ),
            Qt::AlignCenter
                | Qt::TextWordWrap,
            texte
            );
    };


    // ==================================================
    // FONCTION ENTETE
    // ==================================================

    auto dessinerEntete =
        [&]()
    {
        QFont enteteFont(
            "Arial",
            8
            );

        enteteFont.setBold(
            true
            );

        painter.setFont(
            enteteFont
            );


        int x =
            xDepart;


        dessinerCellule(
            x,
            y,
            largeurID,
            "ID"
            );

        x += largeurID;


        dessinerCellule(
            x,
            y,
            largeurNom,
            "Nom"
            );

        x += largeurNom;


        dessinerCellule(
            x,
            y,
            largeurPrenom,
            "Prénom"
            );

        x += largeurPrenom;


        dessinerCellule(
            x,
            y,
            largeurEmail,
            "Email"
            );

        x += largeurEmail;


        dessinerCellule(
            x,
            y,
            largeurTel,
            "Téléphone"
            );

        x += largeurTel;


        dessinerCellule(
            x,
            y,
            largeurAge,
            "Âge"
            );

        x += largeurAge;


        dessinerCellule(
            x,
            y,
            largeurNiveau,
            "Niveau"
            );

        x += largeurNiveau;


        dessinerCellule(
            x,
            y,
            largeurDate,
            "Inscription"
            );


        y += hauteurLigne;
    };


    dessinerEntete();


    // ==================================================
    // DONNEES
    // ==================================================

    painter.setFont(
        QFont(
            "Arial",
            7
            )
        );


    for (const LigneStagiaire &ligne :
         stagiaires)
    {
        if (y + hauteurLigne >
            hauteurPage - 300)
        {
            pdf.newPage();

            y =
                300;

            dessinerEntete();

            painter.setFont(
                QFont(
                    "Arial",
                    7
                    )
                );
        }


        int x =
            xDepart;


        dessinerCellule(
            x,
            y,
            largeurID,
            ligne.id
            );

        x += largeurID;


        dessinerCellule(
            x,
            y,
            largeurNom,
            ligne.nom
            );

        x += largeurNom;


        dessinerCellule(
            x,
            y,
            largeurPrenom,
            ligne.prenom
            );

        x += largeurPrenom;


        dessinerCellule(
            x,
            y,
            largeurEmail,
            ligne.email
            );

        x += largeurEmail;


        dessinerCellule(
            x,
            y,
            largeurTel,
            ligne.telephone
            );

        x += largeurTel;


        dessinerCellule(
            x,
            y,
            largeurAge,
            ligne.age
            );

        x += largeurAge;


        dessinerCellule(
            x,
            y,
            largeurNiveau,
            ligne.niveau
            );

        x += largeurNiveau;


        dessinerCellule(
            x,
            y,
            largeurDate,
            ligne.dateInscription
            );


        y += hauteurLigne;
    }


    // ==================================================
    // PIED DE PAGE
    // ==================================================

    y += 250;


    if (y <
        hauteurPage - 300)
    {
        painter.setFont(
            QFont(
                "Arial",
                8
                )
            );


        painter.drawText(
            margeGauche,
            y,
            "Document généré automatiquement par "
            "l'application Gestion Centre Formation."
            );
    }


    painter.end();


    qDebug()
        << "PDF généré avec succès :"
        << cheminFichier;


    return true;
}


// ======================================================
// 9. CONTENU QR CODE STAGIAIRE
// ======================================================

QString Stagiaire::contenuQrCode(int id)
{
    // ==================================================
    // REQUETE
    // ==================================================

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "IDSTAGIAIRE, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "AGE, "
        "NIVEAU, "
        "DATEINSCRIPTION "
        "FROM STAGIAIRE "
        "WHERE IDSTAGIAIRE = :id"
        );


    query.bindValue(
        ":id",
        id
        );


    // ==================================================
    // EXECUTION
    // ==================================================

    if (!query.exec())
    {
        qDebug()
        << "Erreur récupération stagiaire QR Code :"
        << query.lastError().text();

        return QString();
    }


    // ==================================================
    // VERIFIER EXISTENCE
    // ==================================================

    if (!query.next())
    {
        qDebug()
        << "Aucun stagiaire trouvé avec l'ID :"
        << id;

        return QString();
    }


    // ==================================================
    // RECUPERER LES DONNEES
    // ==================================================

    QString idTexte =
        query.value(0).toString();

    QString nomTexte =
        query.value(1).toString();

    QString prenomTexte =
        query.value(2).toString();

    QString emailTexte =
        query.value(3).toString();

    QString telephoneTexte =
        query.value(4).toString();

    QString ageTexte =
        query.value(5).toString();

    QString niveauTexte =
        query.value(6).toString();


    QDate date =
        query.value(7).toDate();


    QString dateTexte;

    if (date.isValid())
    {
        dateTexte =
            date.toString("dd/MM/yyyy");
    }
    else
    {
        dateTexte =
            "Non définie";
    }


    if (niveauTexte.trimmed().isEmpty())
    {
        niveauTexte =
            "Non défini";
    }


    // ==================================================
    // CONSTRUIRE LE TEXTE DU QR CODE
    // ==================================================

    QString contenu;

    contenu +=
        "CENTRE DE FORMATION\n";

    contenu +=
        "-------------------------\n";

    contenu +=
        "FICHE STAGIAIRE\n";

    contenu +=
        "-------------------------\n";

    contenu +=
        "ID : "
        + idTexte
        + "\n";

    contenu +=
        "Nom : "
        + nomTexte
        + "\n";

    contenu +=
        "Prenom : "
        + prenomTexte
        + "\n";

    contenu +=
        "Email : "
        + emailTexte
        + "\n";

    contenu +=
        "Telephone : "
        + telephoneTexte
        + "\n";

    contenu +=
        "Age : "
        + ageTexte
        + " ans\n";

    contenu +=
        "Niveau : "
        + niveauTexte
        + "\n";

    contenu +=
        "Date inscription : "
        + dateTexte;


    // ==================================================
    // DEBUG
    // ==================================================

    qDebug()
        << "Contenu QR Code stagiaire :";

    qDebug()
        << contenu;


    return contenu;
}
