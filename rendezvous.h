#ifndef RENDEZVOUS_H
#define RENDEZVOUS_H

#include <QMainWindow>
#include "formation.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class rendezVous; }
QT_END_NAMESPACE

class rendezVous : public QMainWindow
{
    Q_OBJECT

public:
    rendezVous(QWidget *parent = nullptr);
    ~rendezVous();
    int calculateTotalParticipants();
    bool checkIDInDatabase(int id);

private slots:
    void on_addBtn_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_commandLinkButton_2_clicked();

    void on_pushButton_16_clicked();

    void on_exportbutton_clicked();

    void on_pushButton_12_clicked();





    void on_calendarWidget_activated(const QDate &date);

    void on_listeA_clicked();

    void on_deleteA_clicked();

    void on_updateA_clicked();

    void on_ajoutA_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_arduino_clicked();

private:
    Ui::rendezVous *ui;
    formation *fr ;
    Arduino A;
    QByteArray data;

};
#endif // RENDEZVOUS_H
