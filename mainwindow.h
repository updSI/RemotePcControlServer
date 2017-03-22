#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QObject>
#include <QtEndian>
#include <QProcess>
#include <QLocalServer>
#include <QSystemTrayIcon>
#include <QMenu>
#include "Server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage(QString msg);
    void messageClicked();

private:
    Ui::MainWindow *ui;
    Server *m_server;

    QSystemTrayIcon *m_systemTrayIcon;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
