#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>

#include "formateur.h"
#include "stagiaire.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


private slots:

    // ==================================================
    // FORMATEUR - CRUD
    // ==================================================

    void on_btn_ajouter_clicked();

    void on_btn_supprimer_clicked();

    void on_btn_modifier_clicked();

    void on_btn_actualiser_clicked();

    void on_tab_formateur_clicked(
        const QModelIndex &index
        );


    // ==================================================
    // FORMATEUR - METIERS
    // ==================================================

    void on_btn_rechercher_formateur_clicked();

    void on_btn_reset_recherche_formateur_clicked();

    void on_btn_trier_formateur_clicked();


    // ==================================================
    // FORMATEUR - PDF
    // ==================================================

    void on_btn_pdf_formateur_clicked();


    // ==================================================
    // STAGIAIRE - CRUD
    // ==================================================

    void on_btn_ajouter_stagiaire_clicked();

    void on_btn_supprimer_stagiaire_clicked();

    void on_btn_modifier_stagiaire_clicked();

    void on_btn_actualiser_stagiaire_clicked();

    void on_tab_stagiaire_clicked(
        const QModelIndex &index
        );


    // ==================================================
    // STAGIAIRE - METIERS
    // ==================================================

    void on_btn_rechercher_stagiaire_clicked();

    void on_btn_reset_recherche_stagiaire_clicked();

    void on_btn_trier_stagiaire_clicked();


    // ==================================================
    // STAGIAIRE - PDF
    // ==================================================

    void on_btn_pdf_stagiaire_clicked();


    // ==================================================
    // STAGIAIRE - QR CODE
    // ==================================================

    void on_btn_qr_stagiaire_clicked();


    // ==================================================
    // STAGIAIRE - MAILING
    // ==================================================

    void on_btn_email_stagiaire_clicked();


private:

    Ui::MainWindow *ui;


    // ==================================================
    // OBJETS
    // ==================================================

    Formateur tmpformateur;

    Stagiaire tmpstagiaire;


    // ==================================================
    // ID SELECTIONNES
    // ==================================================

    int m_idSelectionne = -1;

    int m_idStagiaireSelectionne = -1;


    // ==================================================
    // FORMATEUR
    // ==================================================

    void rafraichirTableau();

    void viderFormulaire();


    // ==================================================
    // STATISTIQUES FORMATEUR
    // ==================================================

    void afficherStatistiquesFormateurs();


    // ==================================================
    // STAGIAIRE
    // ==================================================

    void rafraichirTableauStagiaire();

    void viderFormulaireStagiaire();


    // ==================================================
    // STATISTIQUES STAGIAIRE
    // ==================================================

    void afficherStatistiquesStagiaires();
};

#endif // MAINWINDOW_H
