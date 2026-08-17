#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "formateur.h"
#include "stagiaire.h"

// =====================================================
// QR CODE
// =====================================================

#include "qrcodegen.hpp"

// =====================================================
// QT
// =====================================================

#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QDate>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QByteArray>

// =====================================================
// MAILING
// =====================================================

#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>

// =====================================================
// QT CHARTS
// =====================================================

#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QLegend>
#include <QPainter>


// =====================================================
// CONSTRUCTEUR
// =====================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // =================================================
    // ICONES INTERFACE
    // =================================================

    const QSize tailleIconeMenu(20, 20);
    const QSize tailleIconeAction(18, 18);

    // Menu latéral
    ui->btn_menu_dashboard->setIcon(QIcon(":/icons/home.svg"));
    ui->btn_menu_formateurs->setIcon(QIcon(":/icons/user.svg"));
    ui->btn_menu_stagiaires->setIcon(QIcon(":/icons/users.svg"));
    ui->btn_menu_cours->setIcon(QIcon(":/icons/book.svg"));
    ui->btn_menu_salles->setIcon(QIcon(":/icons/building.svg"));
    ui->btn_menu_statistiques->setIcon(QIcon(":/icons/chart.svg"));
    ui->btn_menu_documents->setIcon(QIcon(":/icons/folder.svg"));
    ui->btn_menu_parametres->setIcon(QIcon(":/icons/settings.svg"));

    ui->btn_menu_dashboard->setIconSize(tailleIconeMenu);
    ui->btn_menu_formateurs->setIconSize(tailleIconeMenu);
    ui->btn_menu_stagiaires->setIconSize(tailleIconeMenu);
    ui->btn_menu_cours->setIconSize(tailleIconeMenu);
    ui->btn_menu_salles->setIconSize(tailleIconeMenu);
    ui->btn_menu_statistiques->setIconSize(tailleIconeMenu);
    ui->btn_menu_documents->setIconSize(tailleIconeMenu);
    ui->btn_menu_parametres->setIconSize(tailleIconeMenu);

    // Actions Formateurs
    ui->btn_ajouter->setIcon(QIcon(":/icons/plus.svg"));
    ui->btn_modifier->setIcon(QIcon(":/icons/edit.svg"));
    ui->btn_supprimer->setIcon(QIcon(":/icons/trash.svg"));
    ui->btn_actualiser->setIcon(QIcon(":/icons/refresh.svg"));
    ui->btn_pdf_formateur->setIcon(QIcon(":/icons/pdf.svg"));
    ui->btn_rechercher_formateur->setIcon(QIcon(":/icons/search.svg"));
    ui->btn_reset_recherche_formateur->setIcon(QIcon(":/icons/refresh.svg"));
    ui->btn_trier_formateur->setIcon(QIcon(":/icons/sort.svg"));

    ui->btn_ajouter->setIconSize(tailleIconeAction);
    ui->btn_modifier->setIconSize(tailleIconeAction);
    ui->btn_supprimer->setIconSize(tailleIconeAction);
    ui->btn_actualiser->setIconSize(tailleIconeAction);
    ui->btn_pdf_formateur->setIconSize(tailleIconeAction);
    ui->btn_rechercher_formateur->setIconSize(tailleIconeAction);
    ui->btn_reset_recherche_formateur->setIconSize(tailleIconeAction);
    ui->btn_trier_formateur->setIconSize(tailleIconeAction);

    // Actions Stagiaires
    ui->btn_ajouter_stagiaire->setIcon(QIcon(":/icons/plus.svg"));
    ui->btn_modifier_stagiaire->setIcon(QIcon(":/icons/edit.svg"));
    ui->btn_supprimer_stagiaire->setIcon(QIcon(":/icons/trash.svg"));
    ui->btn_actualiser_stagiaire->setIcon(QIcon(":/icons/refresh.svg"));
    ui->btn_pdf_stagiaire->setIcon(QIcon(":/icons/pdf.svg"));
    ui->btn_qr_stagiaire->setIcon(QIcon(":/icons/qr.svg"));
    ui->btn_email_stagiaire->setIcon(QIcon(":/icons/mail.svg"));
    ui->btn_rechercher_stagiaire->setIcon(QIcon(":/icons/search.svg"));
    ui->btn_reset_recherche_stagiaire->setIcon(QIcon(":/icons/refresh.svg"));
    ui->btn_trier_stagiaire->setIcon(QIcon(":/icons/sort.svg"));

    ui->btn_ajouter_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_modifier_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_supprimer_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_actualiser_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_pdf_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_qr_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_email_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_rechercher_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_reset_recherche_stagiaire->setIconSize(tailleIconeAction);
    ui->btn_trier_stagiaire->setIconSize(tailleIconeAction);

    // Actualisation statistiques
    ui->btn_refresh_statistiques->setIcon(QIcon(":/icons/refresh.svg"));
    ui->btn_refresh_statistiques->setIconSize(tailleIconeAction);


    // =================================================
    // NAVIGATION SIDEBAR
    // =================================================

    auto activerMenu = [this](QPushButton *bouton, int index)
    {
        ui->btn_menu_dashboard->setChecked(false);
        ui->btn_menu_formateurs->setChecked(false);
        ui->btn_menu_stagiaires->setChecked(false);
        ui->btn_menu_statistiques->setChecked(false);

        bouton->setChecked(true);
        ui->stackedWidget->setCurrentIndex(index);
    };

    connect(
        ui->btn_menu_dashboard,
        &QPushButton::clicked,
        this,
        [this, activerMenu]()
        {
            activerMenu(ui->btn_menu_dashboard, 0);
        }
        );

    connect(
        ui->btn_menu_formateurs,
        &QPushButton::clicked,
        this,
        [this, activerMenu]()
        {
            activerMenu(ui->btn_menu_formateurs, 1);
        }
        );

    connect(
        ui->btn_menu_stagiaires,
        &QPushButton::clicked,
        this,
        [this, activerMenu]()
        {
            activerMenu(ui->btn_menu_stagiaires, 2);
        }
        );


    connect(
        ui->btn_menu_statistiques,
        &QPushButton::clicked,
        this,
        [this, activerMenu]()
        {
            afficherStatistiquesStagiaires();
            afficherStatistiquesFormateurs();
            activerMenu(ui->btn_menu_statistiques, 3);
        }
        );

    connect(
        ui->btn_refresh_statistiques,
        &QPushButton::clicked,
        this,
        [this]()
        {
            afficherStatistiquesStagiaires();
            afficherStatistiquesFormateurs();
        }
        );


    // =================================================
    // STYLE GENERAL
    // =================================================

    setStyleSheet(R"(

    /* =============================================
       FENETRE GENERALE
       ============================================= */

    QMainWindow {
        background-color: #F4F7FB;
    }

    QWidget {
        font-family: "Segoe UI";
        font-size: 13px;
        color: #1F2937;
    }


    /* =============================================
       GROUP BOX
       ============================================= */

    QGroupBox {
        background-color: white;
        border: 1px solid #D8E0EA;
        border-radius: 9px;
        margin-top: 14px;
        padding: 16px 10px 10px 10px;
        font-weight: 600;
    }

    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        left: 14px;
        padding-left: 7px;
        padding-right: 7px;
        color: #1E3A5F;
        background-color: #F4F7FB;
        font-weight: 700;
    }


    /* =============================================
       CHAMPS
       ============================================= */

    QLineEdit,
    QComboBox,
    QDateEdit,
    QSpinBox {
        background-color: white;
        border: 1px solid #CBD5E1;
        border-radius: 6px;
        padding: 6px 9px;
        min-height: 24px;
    }


    /* =============================================
       BOUTONS PAR DEFAUT
       Bleu
       ============================================= */

    QPushButton {
        background-color: #2563EB;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 7px 15px;
        min-height: 24px;
        font-weight: 600;
    }

    QPushButton:hover {
        background-color: #1D4ED8;
    }


    /* =============================================
       AJOUTER
       #249F9F
       ============================================= */

    QPushButton#btn_ajouter,
    QPushButton#btn_ajouter_stagiaire {
        background-color: #249F9F;
        color: white;
    }

    QPushButton#btn_ajouter:hover,
    QPushButton#btn_ajouter_stagiaire:hover {
        background-color: #1C8585;
    }


    /* =============================================
       RECHERCHER
       #249F9F
       ============================================= */

    QPushButton#btn_rechercher_formateur,
    QPushButton#btn_rechercher_stagiaire {
        background-color: #249F9F;
        color: white;
    }

    QPushButton#btn_rechercher_formateur:hover,
    QPushButton#btn_rechercher_stagiaire:hover {
        background-color: #1C8585;
    }


    /* =============================================
       TRIER
       #249F9F
       ============================================= */

    QPushButton#btn_trier_formateur,
    QPushButton#btn_trier_stagiaire {
        background-color: #249F9F;
        color: white;
    }

    QPushButton#btn_trier_formateur:hover,
    QPushButton#btn_trier_stagiaire:hover {
        background-color: #1C8585;
    }


    /* =============================================
       GENERER PDF
       #249F9F
       ============================================= */

    QPushButton#btn_pdf_formateur,
    QPushButton#btn_pdf_stagiaire {
        background-color: #249F9F;
        color: white;
    }

    QPushButton#btn_pdf_formateur:hover,
    QPushButton#btn_pdf_stagiaire:hover {
        background-color: #1C8585;
    }


/* =============================================
   MODIFIER - BLEU #388BD9
   ============================================= */

QPushButton#btn_modifier,
QPushButton#btn_modifier_stagiaire {
    background-color: #388BD9;
    color: white;
}

QPushButton#btn_modifier:hover,
QPushButton#btn_modifier_stagiaire:hover {
    background-color: #307CC4;
}

QPushButton#btn_modifier:pressed,
QPushButton#btn_modifier_stagiaire:pressed {
    background-color: #286DAD;
}


    /* =============================================
       SUPPRIMER
       ROUGE
       ============================================= */

    QPushButton#btn_supprimer,
    QPushButton#btn_supprimer_stagiaire {
        background-color: #DC2626;
        color: white;
    }

    QPushButton#btn_supprimer:hover,
    QPushButton#btn_supprimer_stagiaire:hover {
        background-color: #B91C1C;
    }


    /* =============================================
       ACTUALISER + REINITIALISER
       GRIS BLEU
       ============================================= */

    QPushButton#btn_actualiser,
    QPushButton#btn_actualiser_stagiaire,
    QPushButton#btn_reset_recherche_formateur,
    QPushButton#btn_reset_recherche_stagiaire {
        background-color: #64748B;
        color: white;
    }

    QPushButton#btn_actualiser:hover,
    QPushButton#btn_actualiser_stagiaire:hover,
    QPushButton#btn_reset_recherche_formateur:hover,
    QPushButton#btn_reset_recherche_stagiaire:hover {
        background-color: #475569;
    }


/* =============================================
   QR CODE + ENVOYER EMAIL
   VERT #249F9F
   ============================================= */

QPushButton#btn_qr_stagiaire,
QPushButton#btn_email_stagiaire {
    background-color: #249F9F;
    color: white;
}

QPushButton#btn_qr_stagiaire:hover,
QPushButton#btn_email_stagiaire:hover {
    background-color: #1C8585;
}

QPushButton#btn_qr_stagiaire:pressed,
QPushButton#btn_email_stagiaire:pressed {
    background-color: #176F6F;
}


    /* =============================================
       TABLEAUX
       ============================================= */

    QTableView {
        background-color: white;
        alternate-background-color: #F8FAFC;

        border: 1px solid #D8E0EA;
        border-radius: 8px;

        gridline-color: #E5E7EB;

        selection-background-color: #D9F2F2;
        selection-color: #1E3A5F;
    }


    /* =============================================
       EN-TETE TABLEAU
       ============================================= */

    QHeaderView::section {
        background-color: #1E3A5F;
        color: white;

        border: none;
        border-right: 1px solid #365472;

        padding: 8px;

        font-weight: 600;
    }

)");


    // =================================================
    // VALEURS PAR DEFAUT
    // =================================================

    ui->ui_date_embauche->setDate(
        QDate::currentDate()
        );

    ui->ui_date_stagiaire->setDate(
        QDate::currentDate()
        );

    ui->ui_age_stagiaire->setRange(
        16,
        100
        );

    ui->ui_age_stagiaire->setValue(
        18
        );


    // =================================================
    // RECHERCHE STAGIAIRE
    // =================================================

    ui->ui_recherche_age_stagiaire->setRange(
        0,
        100
        );

    ui->ui_recherche_age_stagiaire->setValue(
        0
        );


    // =================================================
    // RECHERCHE FORMATEUR
    // =================================================

    ui->ui_recherche_date_formateur->setDate(
        QDate(2000, 1, 1)
        );

    ui->ui_recherche_date_formateur->setMaximumDate(
        QDate::currentDate()
        );


    // =================================================
    // CHARGER LES TABLEAUX
    // =================================================

    rafraichirTableau();

    rafraichirTableauStagiaire();


    // =================================================
    // CHARGER LES STATISTIQUES
    // =================================================

    afficherStatistiquesFormateurs();

    afficherStatistiquesStagiaires();
}


// =====================================================
// DESTRUCTEUR
// =====================================================

MainWindow::~MainWindow()
{
    delete ui;
}


// =====================================================
// =====================================================
//                  FORMATEUR
// =====================================================
// =====================================================


// =====================================================
// RAFRAICHIR TABLEAU FORMATEUR
// =====================================================

void MainWindow::rafraichirTableau()
{
    QSqlQueryModel *model =
        tmpformateur.afficher();

    if (model != nullptr)
    {
        ui->tab_formateur->setModel(
            model
            );

        ui->tab_formateur
            ->resizeColumnsToContents();

        ui->tab_formateur
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_formateur
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
}


// =====================================================
// VIDER FORMULAIRE FORMATEUR
// =====================================================

void MainWindow::viderFormulaire()
{
    ui->ui_nom->clear();

    ui->ui_prenom->clear();

    ui->ui_email->clear();

    ui->ui_tel->clear();

    ui->ui_specialite->clear();

    ui->ui_id_suppr->clear();

    ui->ui_date_embauche->setDate(
        QDate::currentDate()
        );

    m_idSelectionne = -1;
}


// =====================================================
// AJOUTER FORMATEUR
// =====================================================

void MainWindow::on_btn_ajouter_clicked()
{
    QString nom =
        ui->ui_nom
            ->text()
            .trimmed();

    QString prenom =
        ui->ui_prenom
            ->text()
            .trimmed();

    QString email =
        ui->ui_email
            ->text()
            .trimmed();

    QString tel =
        ui->ui_tel
            ->text()
            .trimmed();

    QString spec =
        ui->ui_specialite
            ->text()
            .trimmed();

    QDate dateE =
        ui->ui_date_embauche
            ->date();


    if (nom.isEmpty() ||
        prenom.isEmpty() ||
        email.isEmpty() ||
        tel.isEmpty() ||
        spec.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Saisie invalide",
            "Veuillez remplir tous les champs du formateur."
            );

        return;
    }


    QRegularExpression regexNom(
        "^[A-Za-zÀ-ÿ\\s'-]+$"
        );


    if (!regexNom.match(nom).hasMatch() ||
        !regexNom.match(prenom).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Nom invalide",
            "Le nom et le prénom doivent contenir uniquement des lettres."
            );

        return;
    }


    QRegularExpression regexEmail(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
        );


    if (!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "Veuillez saisir une adresse email valide."
            );

        return;
    }


    QRegularExpression regexTel(
        "^[0-9]{8}$"
        );


    if (!regexTel.match(tel).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Téléphone invalide",
            "Le numéro de téléphone doit contenir exactement 8 chiffres."
            );

        return;
    }


    if (dateE > QDate::currentDate())
    {
        QMessageBox::warning(
            this,
            "Date invalide",
            "La date d'embauche ne peut pas être dans le futur."
            );

        return;
    }


    Formateur f(
        nom,
        prenom,
        email,
        tel,
        spec,
        dateE
        );


    if (f.ajouter())
    {
        rafraichirTableau();

        afficherStatistiquesFormateurs();

        viderFormulaire();

        QMessageBox::information(
            this,
            "Succès",
            "Formateur ajouté avec succès."
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Échec de l'ajout du formateur.\n\n"
            "Vérifiez notamment que l'adresse email n'existe pas déjà."
            );
    }
}


// =====================================================
// MODIFIER FORMATEUR
// =====================================================

void MainWindow::on_btn_modifier_clicked()
{
    if (m_idSelectionne <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Cliquez d'abord sur une ligne du tableau "
            "pour sélectionner un formateur."
            );

        return;
    }


    QString nom =
        ui->ui_nom
            ->text()
            .trimmed();

    QString prenom =
        ui->ui_prenom
            ->text()
            .trimmed();

    QString email =
        ui->ui_email
            ->text()
            .trimmed();

    QString tel =
        ui->ui_tel
            ->text()
            .trimmed();

    QString spec =
        ui->ui_specialite
            ->text()
            .trimmed();

    QDate dateE =
        ui->ui_date_embauche
            ->date();


    if (nom.isEmpty() ||
        prenom.isEmpty() ||
        email.isEmpty() ||
        tel.isEmpty() ||
        spec.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Saisie invalide",
            "Veuillez remplir tous les champs."
            );

        return;
    }


    QRegularExpression regexNom(
        "^[A-Za-zÀ-ÿ\\s'-]+$"
        );


    if (!regexNom.match(nom).hasMatch() ||
        !regexNom.match(prenom).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Nom invalide",
            "Le nom et le prénom doivent contenir uniquement des lettres."
            );

        return;
    }


    QRegularExpression regexEmail(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
        );


    if (!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "Veuillez saisir une adresse email valide."
            );

        return;
    }


    QRegularExpression regexTel(
        "^[0-9]{8}$"
        );


    if (!regexTel.match(tel).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Téléphone invalide",
            "Le numéro de téléphone doit contenir exactement 8 chiffres."
            );

        return;
    }


    if (dateE > QDate::currentDate())
    {
        QMessageBox::warning(
            this,
            "Date invalide",
            "La date d'embauche ne peut pas être dans le futur."
            );

        return;
    }


    Formateur f(
        nom,
        prenom,
        email,
        tel,
        spec,
        dateE
        );


    if (f.modifier(m_idSelectionne))
    {
        rafraichirTableau();

        afficherStatistiquesFormateurs();

        viderFormulaire();

        QMessageBox::information(
            this,
            "Succès",
            "Formateur modifié avec succès."
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Échec de la modification du formateur."
            );
    }
}


// =====================================================
// SUPPRIMER FORMATEUR
// =====================================================

void MainWindow::on_btn_supprimer_clicked()
{
    int id =
        ui->ui_id_suppr
            ->text()
            .toInt();


    if (id <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Veuillez sélectionner un formateur dans le tableau."
            );

        return;
    }


    QMessageBox::StandardButton confirmation =
        QMessageBox::question(
            this,
            "Confirmation",
            "Voulez-vous vraiment supprimer ce formateur ?",
            QMessageBox::Yes | QMessageBox::No
            );


    if (confirmation != QMessageBox::Yes)
    {
        return;
    }


    if (tmpformateur.supprimer(id))
    {
        rafraichirTableau();

        afficherStatistiquesFormateurs();

        viderFormulaire();

        QMessageBox::information(
            this,
            "Succès",
            "Formateur supprimé avec succès."
            );
    }
    else
    {
        QMessageBox::warning(
            this,
            "Suppression impossible",
            "Impossible de supprimer ce formateur.\n\n"
            "Il est actuellement affecté à un ou plusieurs cours."
            );
    }
}


// =====================================================
// ACTUALISER FORMATEUR
// =====================================================

void MainWindow::on_btn_actualiser_clicked()
{
    rafraichirTableau();

    afficherStatistiquesFormateurs();

    viderFormulaire();
}


// =====================================================
// CLIC TABLEAU FORMATEUR
// =====================================================

void MainWindow::on_tab_formateur_clicked(
    const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }


    QAbstractItemModel *modele =
        ui->tab_formateur
            ->model();


    if (modele == nullptr)
    {
        return;
    }


    int ligne =
        index.row();


    m_idSelectionne =
        modele->index(ligne, 0)
            .data()
            .toInt();


    ui->ui_id_suppr->setText(
        QString::number(
            m_idSelectionne
            )
        );


    ui->ui_nom->setText(
        modele->index(ligne, 1)
            .data()
            .toString()
        );


    ui->ui_prenom->setText(
        modele->index(ligne, 2)
            .data()
            .toString()
        );


    ui->ui_email->setText(
        modele->index(ligne, 3)
            .data()
            .toString()
        );


    ui->ui_tel->setText(
        modele->index(ligne, 4)
            .data()
            .toString()
        );


    ui->ui_specialite->setText(
        modele->index(ligne, 5)
            .data()
            .toString()
        );


    QDate date =
        modele->index(ligne, 6)
            .data()
            .toDate();


    if (date.isValid())
    {
        ui->ui_date_embauche->setDate(
            date
            );
    }
}


// =====================================================
// RECHERCHE FORMATEUR
// =====================================================

void MainWindow::on_btn_rechercher_formateur_clicked()
{
    QString nom =
        ui->ui_recherche_nom_formateur
            ->text()
            .trimmed();


    QString specialite =
        ui->ui_recherche_specialite_formateur
            ->text()
            .trimmed();


    QDate dateMin =
        ui->ui_recherche_date_formateur
            ->date();


    QSqlQueryModel *model =
        tmpformateur.rechercher(
            nom,
            specialite,
            dateMin
            );


    if (model != nullptr)
    {
        ui->tab_formateur->setModel(
            model
            );

        ui->tab_formateur
            ->resizeColumnsToContents();

        ui->tab_formateur
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_formateur
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Erreur lors de la recherche des formateurs."
            );
    }
}


// =====================================================
// RESET RECHERCHE FORMATEUR
// =====================================================

void MainWindow::on_btn_reset_recherche_formateur_clicked()
{
    ui->ui_recherche_nom_formateur->clear();

    ui->ui_recherche_specialite_formateur->clear();

    ui->ui_recherche_date_formateur->setDate(
        QDate(2000, 1, 1)
        );

    ui->ui_tri_formateur->setCurrentIndex(0);

    ui->ui_ordre_formateur->setCurrentIndex(0);

    rafraichirTableau();

    ui->tab_formateur->clearSelection();

    m_idSelectionne = -1;
}


// =====================================================
// TRI FORMATEUR
// =====================================================

void MainWindow::on_btn_trier_formateur_clicked()
{
    QString critere =
        ui->ui_tri_formateur
            ->currentText();

    QString ordre =
        ui->ui_ordre_formateur
            ->currentText();


    QSqlQueryModel *model =
        tmpformateur.trier(
            critere,
            ordre
            );


    if (model != nullptr)
    {
        ui->tab_formateur->setModel(
            model
            );

        ui->tab_formateur
            ->resizeColumnsToContents();

        ui->tab_formateur
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_formateur
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Erreur lors du tri des formateurs."
            );
    }
}


// =====================================================
// STATISTIQUES FORMATEURS
// =====================================================

void MainWindow::afficherStatistiquesFormateurs()
{
    QLayoutItem *item;


    while (
        (item =
         ui->layout_graphique_formateur
             ->takeAt(0))
        != nullptr
        )
    {
        if (item->widget() != nullptr)
        {
            item->widget()->deleteLater();
        }

        delete item;
    }


    QSqlQueryModel *model =
        tmpformateur.statistiquesParSpecialite();


    if (model == nullptr)
    {
        return;
    }


    int total = 0;


    for (int i = 0;
         i < model->rowCount();
         ++i)
    {
        total +=
            model->index(i, 1)
                .data()
                .toInt();
    }


    QPieSeries *series =
        new QPieSeries();


    for (int i = 0;
         i < model->rowCount();
         ++i)
    {
        QString specialite =
            model->index(i, 0)
                .data()
                .toString()
                .trimmed();


        int nombre =
            model->index(i, 1)
                .data()
                .toInt();


        if (specialite.isEmpty())
        {
            specialite =
                "Non définie";
        }


        if (nombre > 0 &&
            total > 0)
        {
            double pourcentage =
                (
                    static_cast<double>(nombre)
                    / total
                    )
                * 100.0;


            QString etiquette =
                specialite
                + " : "
                + QString::number(nombre)
                + " ("
                + QString::number(
                    pourcentage,
                    'f',
                    1
                    )
                + "%)";


            QPieSlice *slice =
                series->append(
                    etiquette,
                    nombre
                    );


            slice->setLabelVisible(
                true
                );
        }
    }


    QChart *chart =
        new QChart();


    chart->addSeries(
        series
        );


    chart->setTitle(
        "Nombre de formateurs par spécialité"
        );


    chart->legend()
        ->setVisible(true);


    chart->legend()
        ->setAlignment(
            Qt::AlignBottom
            );


    QChartView *chartView =
        new QChartView(
            chart
            );


    chartView->setRenderHint(
        QPainter::Antialiasing
        );


    ui->layout_graphique_formateur
        ->addWidget(
            chartView
            );


    delete model;
}


// =====================================================
// PDF FORMATEUR
// =====================================================

void MainWindow::on_btn_pdf_formateur_clicked()
{
    QString cheminFichier =
        QFileDialog::getSaveFileName(
            this,
            "Enregistrer la liste des formateurs",
            "Liste_Formateurs.pdf",
            "Fichiers PDF (*.pdf)"
            );


    if (cheminFichier.isEmpty())
    {
        return;
    }


    if (!cheminFichier.endsWith(
            ".pdf",
            Qt::CaseInsensitive))
    {
        cheminFichier +=
            ".pdf";
    }


    bool test =
        tmpformateur.genererPdf(
            cheminFichier
            );


    if (test)
    {
        QMessageBox::information(
            this,
            "PDF généré",
            "Le document PDF des formateurs a été généré avec succès.\n\n"
            "Emplacement :\n"
                + cheminFichier
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de générer le PDF des formateurs."
            );
    }
}


// =====================================================
// =====================================================
//                  STAGIAIRE
// =====================================================
// =====================================================


// =====================================================
// RAFRAICHIR TABLEAU STAGIAIRE
// =====================================================

void MainWindow::rafraichirTableauStagiaire()
{
    QSqlQueryModel *model =
        tmpstagiaire.afficher();


    if (model != nullptr)
    {
        ui->tab_stagiaire->setModel(
            model
            );

        ui->tab_stagiaire
            ->resizeColumnsToContents();

        ui->tab_stagiaire
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_stagiaire
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
}


// =====================================================
// VIDER FORMULAIRE STAGIAIRE
// =====================================================

void MainWindow::viderFormulaireStagiaire()
{
    ui->ui_nom_stagiaire->clear();

    ui->ui_prenom_stagiaire->clear();

    ui->ui_email_stagiaire->clear();

    ui->ui_tel_stagiaire->clear();

    ui->ui_age_stagiaire->setValue(
        18
        );

    ui->ui_niveau_stagiaire->setCurrentIndex(
        0
        );

    ui->ui_id_suppr_stagiaire->clear();

    ui->ui_date_stagiaire->setDate(
        QDate::currentDate()
        );

    m_idStagiaireSelectionne = -1;
}


// =====================================================
// AJOUTER STAGIAIRE
// =====================================================

void MainWindow::on_btn_ajouter_stagiaire_clicked()
{
    QString nom =
        ui->ui_nom_stagiaire
            ->text()
            .trimmed();

    QString prenom =
        ui->ui_prenom_stagiaire
            ->text()
            .trimmed();

    QString email =
        ui->ui_email_stagiaire
            ->text()
            .trimmed();

    QString tel =
        ui->ui_tel_stagiaire
            ->text()
            .trimmed();

    int age =
        ui->ui_age_stagiaire
            ->value();

    QString niv =
        ui->ui_niveau_stagiaire
            ->currentText();

    QDate dateI =
        ui->ui_date_stagiaire
            ->date();


    if (nom.isEmpty() ||
        prenom.isEmpty() ||
        email.isEmpty() ||
        tel.isEmpty() ||
        niv.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Saisie invalide",
            "Veuillez remplir tous les champs du stagiaire."
            );

        return;
    }


    QRegularExpression regexNom(
        "^[A-Za-zÀ-ÿ\\s'-]+$"
        );


    if (!regexNom.match(nom).hasMatch() ||
        !regexNom.match(prenom).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Nom invalide",
            "Le nom et le prénom doivent contenir uniquement des lettres."
            );

        return;
    }


    QRegularExpression regexEmail(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
        );


    if (!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "Veuillez saisir une adresse email valide."
            );

        return;
    }


    QRegularExpression regexTel(
        "^[0-9]{8}$"
        );


    if (!regexTel.match(tel).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Téléphone invalide",
            "Le numéro de téléphone doit contenir exactement 8 chiffres."
            );

        return;
    }


    if (age < 16 ||
        age > 100)
    {
        QMessageBox::warning(
            this,
            "Âge invalide",
            "L'âge doit être compris entre 16 et 100 ans."
            );

        return;
    }


    if (niv != "Débutant" &&
        niv != "Intermédiaire" &&
        niv != "Avancé")
    {
        QMessageBox::warning(
            this,
            "Niveau invalide",
            "Le niveau doit être Débutant, Intermédiaire ou Avancé."
            );

        return;
    }


    if (dateI > QDate::currentDate())
    {
        QMessageBox::warning(
            this,
            "Date invalide",
            "La date d'inscription ne peut pas être dans le futur."
            );

        return;
    }


    Stagiaire s(
        nom,
        prenom,
        email,
        tel,
        age,
        niv,
        dateI
        );


    if (s.ajouter())
    {
        rafraichirTableauStagiaire();

        afficherStatistiquesStagiaires();

        viderFormulaireStagiaire();


        QMessageBox::information(
            this,
            "Succès",
            "Stagiaire ajouté avec succès."
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Échec de l'ajout du stagiaire.\n\n"
            "Vérifiez notamment que l'adresse email n'existe pas déjà."
            );
    }
}


// =====================================================
// MODIFIER STAGIAIRE
// =====================================================

void MainWindow::on_btn_modifier_stagiaire_clicked()
{
    if (m_idStagiaireSelectionne <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Cliquez d'abord sur une ligne du tableau "
            "pour sélectionner un stagiaire."
            );

        return;
    }


    QString nom =
        ui->ui_nom_stagiaire
            ->text()
            .trimmed();

    QString prenom =
        ui->ui_prenom_stagiaire
            ->text()
            .trimmed();

    QString email =
        ui->ui_email_stagiaire
            ->text()
            .trimmed();

    QString tel =
        ui->ui_tel_stagiaire
            ->text()
            .trimmed();

    int age =
        ui->ui_age_stagiaire
            ->value();

    QString niv =
        ui->ui_niveau_stagiaire
            ->currentText();

    QDate dateI =
        ui->ui_date_stagiaire
            ->date();


    Stagiaire s(
        nom,
        prenom,
        email,
        tel,
        age,
        niv,
        dateI
        );


    if (s.modifier(
            m_idStagiaireSelectionne))
    {
        rafraichirTableauStagiaire();

        afficherStatistiquesStagiaires();

        viderFormulaireStagiaire();


        QMessageBox::information(
            this,
            "Succès",
            "Stagiaire modifié avec succès."
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Échec de la modification du stagiaire."
            );
    }
}


// =====================================================
// SUPPRIMER STAGIAIRE
// =====================================================

void MainWindow::on_btn_supprimer_stagiaire_clicked()
{
    int id =
        ui->ui_id_suppr_stagiaire
            ->text()
            .toInt();


    if (id <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Veuillez sélectionner un stagiaire dans le tableau."
            );

        return;
    }


    QMessageBox::StandardButton confirmation =
        QMessageBox::question(
            this,
            "Confirmation",
            "Voulez-vous vraiment supprimer ce stagiaire ?",
            QMessageBox::Yes | QMessageBox::No
            );


    if (confirmation != QMessageBox::Yes)
    {
        return;
    }


    if (tmpstagiaire.supprimer(id))
    {
        rafraichirTableauStagiaire();

        afficherStatistiquesStagiaires();

        viderFormulaireStagiaire();


        QMessageBox::information(
            this,
            "Succès",
            "Stagiaire supprimé avec succès."
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Échec de la suppression du stagiaire."
            );
    }
}


// =====================================================
// ACTUALISER STAGIAIRE
// =====================================================

void MainWindow::on_btn_actualiser_stagiaire_clicked()
{
    rafraichirTableauStagiaire();

    afficherStatistiquesStagiaires();

    viderFormulaireStagiaire();
}


// =====================================================
// CLIC TABLEAU STAGIAIRE
// =====================================================

void MainWindow::on_tab_stagiaire_clicked(
    const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }


    QAbstractItemModel *modele =
        ui->tab_stagiaire
            ->model();


    if (modele == nullptr)
    {
        return;
    }


    int ligne =
        index.row();


    m_idStagiaireSelectionne =
        modele->index(ligne, 0)
            .data()
            .toInt();


    ui->ui_id_suppr_stagiaire->setText(
        QString::number(
            m_idStagiaireSelectionne
            )
        );


    ui->ui_nom_stagiaire->setText(
        modele->index(ligne, 1)
            .data()
            .toString()
        );


    ui->ui_prenom_stagiaire->setText(
        modele->index(ligne, 2)
            .data()
            .toString()
        );


    ui->ui_email_stagiaire->setText(
        modele->index(ligne, 3)
            .data()
            .toString()
        );


    ui->ui_tel_stagiaire->setText(
        modele->index(ligne, 4)
            .data()
            .toString()
        );


    ui->ui_age_stagiaire->setValue(
        modele->index(ligne, 5)
            .data()
            .toInt()
        );


    ui->ui_niveau_stagiaire
        ->setCurrentText(
            modele->index(ligne, 6)
                .data()
                .toString()
            );


    QDate date =
        modele->index(ligne, 7)
            .data()
            .toDate();


    if (date.isValid())
    {
        ui->ui_date_stagiaire
            ->setDate(
                date
                );
    }
}


// =====================================================
// RECHERCHE STAGIAIRE
// =====================================================

void MainWindow::on_btn_rechercher_stagiaire_clicked()
{
    QString nom =
        ui->ui_recherche_nom_stagiaire
            ->text()
            .trimmed();


    QString niveau =
        ui->ui_recherche_niveau_stagiaire
            ->currentText();


    int ageMin =
        ui->ui_recherche_age_stagiaire
            ->value();


    if (niveau == "Tous")
    {
        niveau.clear();
    }


    QSqlQueryModel *model =
        tmpstagiaire.rechercher(
            nom,
            niveau,
            ageMin
            );


    if (model != nullptr)
    {
        ui->tab_stagiaire->setModel(
            model
            );

        ui->tab_stagiaire
            ->resizeColumnsToContents();

        ui->tab_stagiaire
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_stagiaire
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Erreur lors de la recherche des stagiaires."
            );
    }
}


// =====================================================
// RESET RECHERCHE STAGIAIRE
// =====================================================

void MainWindow::on_btn_reset_recherche_stagiaire_clicked()
{
    ui->ui_recherche_nom_stagiaire->clear();

    ui->ui_recherche_niveau_stagiaire->setCurrentIndex(0);

    ui->ui_recherche_age_stagiaire->setValue(0);

    ui->ui_tri_stagiaire->setCurrentIndex(0);

    ui->ui_ordre_stagiaire->setCurrentIndex(0);

    rafraichirTableauStagiaire();

    ui->tab_stagiaire->clearSelection();

    m_idStagiaireSelectionne = -1;
}


// =====================================================
// TRI STAGIAIRE
// =====================================================

void MainWindow::on_btn_trier_stagiaire_clicked()
{
    QString critere =
        ui->ui_tri_stagiaire
            ->currentText();

    QString ordre =
        ui->ui_ordre_stagiaire
            ->currentText();


    QSqlQueryModel *model =
        tmpstagiaire.trier(
            critere,
            ordre
            );


    if (model != nullptr)
    {
        ui->tab_stagiaire->setModel(
            model
            );

        ui->tab_stagiaire
            ->resizeColumnsToContents();

        ui->tab_stagiaire
            ->setSelectionBehavior(
                QAbstractItemView::SelectRows
                );

        ui->tab_stagiaire
            ->setSelectionMode(
                QAbstractItemView::SingleSelection
                );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Erreur lors du tri des stagiaires."
            );
    }
}


// =====================================================
// STATISTIQUES STAGIAIRE
// =====================================================

void MainWindow::afficherStatistiquesStagiaires()
{
    // Nettoyer l'ancien graphique
    QLayoutItem *item;

    while (
        (item =
         ui->layout_graphique_stagiaire
             ->takeAt(0))
        != nullptr
        )
    {
        if (item->widget() != nullptr)
        {
            item->widget()->deleteLater();
        }

        delete item;
    }


    QSqlQueryModel *model =
        tmpstagiaire.statistiquesParNiveau();


    if (model == nullptr)
    {
        return;
    }


    QBarSet *barSet =
        new QBarSet(
            "Stagiaires"
            );


    QStringList categories;

    int maximum =
        0;


    for (int i = 0;
         i < model->rowCount();
         ++i)
    {
        QString niveau =
            model->index(i, 0)
                .data()
                .toString()
                .trimmed();


        int nombre =
            model->index(i, 1)
                .data()
                .toInt();


        if (niveau.isEmpty())
        {
            niveau =
                "Non défini";
        }


        categories
            << niveau;


        *barSet
            << nombre;


        if (nombre > maximum)
        {
            maximum =
                nombre;
        }
    }


    QBarSeries *series =
        new QBarSeries();


    series->append(
        barSet
        );


    series->setLabelsVisible(
        true
        );


    series->setLabelsPosition(
        QAbstractBarSeries::LabelsOutsideEnd
        );


    QChart *chart =
        new QChart();


    chart->addSeries(
        series
        );


    chart->setTitle(
        "Nombre de stagiaires par niveau"
        );


    chart->setAnimationOptions(
        QChart::SeriesAnimations
        );


    chart->legend()
        ->setVisible(false);


    QBarCategoryAxis *axisX =
        new QBarCategoryAxis();


    axisX->append(
        categories
        );


    chart->addAxis(
        axisX,
        Qt::AlignBottom
        );


    series->attachAxis(
        axisX
        );


    QValueAxis *axisY =
        new QValueAxis();


    axisY->setMin(
        0
        );


    axisY->setMax(
        maximum > 0
            ? maximum + qMax(2, maximum / 5)
            : 10
        );


    axisY->setLabelFormat(
        "%d"
        );


    axisY->setTickCount(
        6
        );


    chart->addAxis(
        axisY,
        Qt::AlignLeft
        );


    series->attachAxis(
        axisY
        );


    QChartView *chartView =
        new QChartView(
            chart
            );


    chartView->setRenderHint(
        QPainter::Antialiasing
        );


    ui->layout_graphique_stagiaire
        ->addWidget(
            chartView
            );


    delete model;
}


// =====================================================
// PDF STAGIAIRE
// =====================================================

void MainWindow::on_btn_pdf_stagiaire_clicked()
{
    QString cheminFichier =
        QFileDialog::getSaveFileName(
            this,
            "Enregistrer la liste des stagiaires",
            "Liste_Stagiaires.pdf",
            "Fichiers PDF (*.pdf)"
            );


    if (cheminFichier.isEmpty())
    {
        return;
    }


    if (!cheminFichier.endsWith(
            ".pdf",
            Qt::CaseInsensitive))
    {
        cheminFichier +=
            ".pdf";
    }


    bool test =
        tmpstagiaire.genererPdf(
            cheminFichier
            );


    if (test)
    {
        QMessageBox::information(
            this,
            "PDF généré",
            "Le document PDF a été généré avec succès.\n\n"
            "Emplacement :\n"
                + cheminFichier
            );
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de générer le document PDF."
            );
    }
}


// =====================================================
// =====================================================
//              QR CODE STAGIAIRE
// =====================================================
// =====================================================

void MainWindow::on_btn_qr_stagiaire_clicked()
{
    if (m_idStagiaireSelectionne <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Veuillez d'abord sélectionner un stagiaire "
            "dans le tableau."
            );

        return;
    }


    QString contenu =
        tmpstagiaire.contenuQrCode(
            m_idStagiaireSelectionne
            );


    if (contenu.isEmpty())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de récupérer les informations "
            "du stagiaire."
            );

        return;
    }


    QByteArray donneesQr =
        contenu.toUtf8();


    qrcodegen::QrCode qr =
        qrcodegen::QrCode::encodeText(
            donneesQr.constData(),
            qrcodegen::QrCode::Ecc::MEDIUM
            );


    const int tailleModule =
        8;

    const int marge =
        4;

    int tailleQr =
        qr.getSize();


    int tailleImage =
        (
            tailleQr
            + (2 * marge)
            )
        * tailleModule;


    QImage image(
        tailleImage,
        tailleImage,
        QImage::Format_RGB32
        );


    image.fill(
        Qt::white
        );


    QPainter painterQr(
        &image
        );


    painterQr.setPen(
        Qt::NoPen
        );


    painterQr.setBrush(
        Qt::black
        );


    for (int y = 0;
         y < tailleQr;
         ++y)
    {
        for (int x = 0;
             x < tailleQr;
             ++x)
        {
            if (qr.getModule(x, y))
            {
                int positionX =
                    (x + marge)
                    * tailleModule;

                int positionY =
                    (y + marge)
                    * tailleModule;


                painterQr.drawRect(
                    positionX,
                    positionY,
                    tailleModule,
                    tailleModule
                    );
            }
        }
    }


    painterQr.end();


    QDialog dialog(
        this
        );


    dialog.setWindowTitle(
        "QR Code Stagiaire"
        );


    dialog.setMinimumSize(
        500,
        600
        );


    QVBoxLayout *layout =
        new QVBoxLayout(
            &dialog
            );


    QLabel *titre =
        new QLabel(
            "QR Code du stagiaire",
            &dialog
            );


    titre->setAlignment(
        Qt::AlignCenter
        );


    QFont policeTitre =
        titre->font();


    policeTitre.setPointSize(
        16
        );


    policeTitre.setBold(
        true
        );


    titre->setFont(
        policeTitre
        );


    layout->addWidget(
        titre
        );


    QLabel *infos =
        new QLabel(
            "ID Stagiaire : "
                + QString::number(
                    m_idStagiaireSelectionne
                    )
                + "\n"
                + ui->ui_nom_stagiaire->text()
                + " "
                + ui->ui_prenom_stagiaire->text(),
            &dialog
            );


    infos->setAlignment(
        Qt::AlignCenter
        );


    layout->addWidget(
        infos
        );


    QLabel *labelQr =
        new QLabel(
            &dialog
            );


    labelQr->setAlignment(
        Qt::AlignCenter
        );


    labelQr->setPixmap(
        QPixmap::fromImage(
            image
            )
        );


    layout->addWidget(
        labelQr,
        0,
        Qt::AlignCenter
        );


    QPushButton *btnEnregistrer =
        new QPushButton(
            "Enregistrer le QR Code",
            &dialog
            );


    layout->addWidget(
        btnEnregistrer
        );


    QPushButton *btnFermer =
        new QPushButton(
            "Fermer",
            &dialog
            );


    layout->addWidget(
        btnFermer
        );


    connect(
        btnEnregistrer,
        &QPushButton::clicked,
        &dialog,
        [&]()
        {
            QString nomFichier =
                "QR_Stagiaire_"
                + QString::number(
                    m_idStagiaireSelectionne
                    )
                + ".png";


            QString chemin =
                QFileDialog::getSaveFileName(
                    &dialog,
                    "Enregistrer le QR Code",
                    nomFichier,
                    "Images PNG (*.png)"
                    );


            if (chemin.isEmpty())
            {
                return;
            }


            if (!chemin.endsWith(
                    ".png",
                    Qt::CaseInsensitive))
            {
                chemin +=
                    ".png";
            }


            if (image.save(
                    chemin,
                    "PNG"))
            {
                QMessageBox::information(
                    &dialog,
                    "Succès",
                    "Le QR Code a été enregistré avec succès.\n\n"
                    "Emplacement :\n"
                        + chemin
                    );
            }
            else
            {
                QMessageBox::critical(
                    &dialog,
                    "Erreur",
                    "Impossible d'enregistrer le QR Code."
                    );
            }
        }
        );


    connect(
        btnFermer,
        &QPushButton::clicked,
        &dialog,
        &QDialog::accept
        );


    dialog.exec();
}


// =====================================================
// =====================================================
//              MAILING STAGIAIRE
// =====================================================
// =====================================================

void MainWindow::on_btn_email_stagiaire_clicked()
{
    // =================================================
    // 1. VERIFIER LA SELECTION
    // =================================================

    if (m_idStagiaireSelectionne <= 0)
    {
        QMessageBox::warning(
            this,
            "Sélection requise",
            "Veuillez d'abord sélectionner un stagiaire "
            "dans le tableau."
            );

        return;
    }


    // =================================================
    // 2. RECUPERER LES DONNEES ORACLE
    // =================================================

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "NOM, "
        "PRENOM, "
        "EMAIL "
        "FROM STAGIAIRE "
        "WHERE IDSTAGIAIRE = :id"
        );


    query.bindValue(
        ":id",
        m_idStagiaireSelectionne
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur SQL",
            "Impossible de récupérer les informations "
            "du stagiaire.\n\n"
                + query.lastError().text()
            );

        return;
    }


    if (!query.next())
    {
        QMessageBox::warning(
            this,
            "Stagiaire introuvable",
            "Le stagiaire sélectionné n'existe pas "
            "dans la base de données."
            );

        return;
    }


    // =================================================
    // 3. INFORMATIONS STAGIAIRE
    // =================================================

    QString nom =
        query.value(0)
            .toString()
            .trimmed();


    QString prenom =
        query.value(1)
            .toString()
            .trimmed();


    QString email =
        query.value(2)
            .toString()
            .trimmed();


    // =================================================
    // 4. VERIFIER EMAIL
    // =================================================

    if (email.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Email manquant",
            "Aucune adresse email n'est enregistrée "
            "pour ce stagiaire."
            );

        return;
    }


    QRegularExpression regexEmail(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
        );


    if (!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "L'adresse email du stagiaire est invalide :\n"
                + email
            );

        return;
    }


    // =================================================
    // 5. SUJET
    // =================================================

    QString sujet =
        "Centre de Formation - Information";


    // =================================================
    // 6. MESSAGE PERSONNALISE
    // =================================================

    QString message =
        "Bonjour "
        + prenom
        + " "
        + nom
        + ",\n\n"
          "Nous vous contactons concernant votre inscription "
          "au Centre de Formation.\n\n"
          "Nous restons à votre disposition pour toute "
          "information complémentaire.\n\n"
          "Cordialement,\n"
          "Centre de Formation";


    // =================================================
    // 7. CONSTRUIRE LE MAILTO
    // =================================================

    QUrl url;

    url.setScheme(
        "mailto"
        );

    url.setPath(
        email
        );


    QUrlQuery parametres;


    parametres.addQueryItem(
        "subject",
        sujet
        );


    parametres.addQueryItem(
        "body",
        message
        );


    url.setQuery(
        parametres
        );


    // =================================================
    // 8. OUVRIR APPLICATION EMAIL
    // =================================================

    bool ouverture =
        QDesktopServices::openUrl(
            url
            );


    if (!ouverture)
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'ouvrir votre application "
            "de messagerie."
            );

        return;
    }
}
