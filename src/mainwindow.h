#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include<QDirModel>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;
    int divisio(QString divisio_str);
     int div_id=0;
     int class_id=0;
     int fam_id=0;
     int gen_id=0;
     int spec_id=0;
     int indi_id=0;
     int classis(QString classis_str,int classis_div_id);
     int familia(QString familia_str,int familia_classis_id);
     int genus(QString genus_str,int genus_familia_id);
     int species(QString lat_name, QString rus_name, QString other_names, int species_genus_id);
     int individual(QString collection_date,QString region,QString district,QString location, QString habitat, QString collectors, QString who_identified,QString note,QString catalogiong_date, QString box_no,int species_genus_id);

private slots:




     void on_pushButton_clicked();

private:

    Ui::MainWindow *ui;
    QSqlTableModel *m_csvModel;
    QSqlQuery *outtable;

    void setupModel();
    void fillTableView();

    
};

#endif // MAINWINDOW_H
