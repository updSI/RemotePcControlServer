#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("updSoftware");
    QCoreApplication::setOrganizationDomain("upd.si");
    QCoreApplication::setApplicationName("Remote PC Control");

    if(QSystemTrayIcon::isSystemTrayAvailable())
        qDebug() << "Tray available";

  //  minimizeAction = new QAction(tr("Mi&nimize"), this);
   // connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

   // maximizeAction = new QAction(tr("Ma&ximize"), this);
  //  connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Open"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
  //  trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    m_systemTrayIcon = new QSystemTrayIcon(QIcon(":Pictures/remote-control.png"), this);
    m_systemTrayIcon->setContextMenu(trayIconMenu);
    m_systemTrayIcon->setToolTip("Remote PC Control");

    connect(m_systemTrayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(m_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    m_systemTrayIcon->show();

  //  m_systemTrayIcon = new QSystemTrayIcon();
    //m_systemTrayIcon->showMessage("aaa", "lool");

    QString passwd = "";
    int port = 0;

    QSettings settings;
    if(settings.contains("RemotePCC/passwd"))
        passwd = settings.value("RemotePCC/passwd").toString();
    else
    {
        // generate random password
        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        const int randomStringLength = 12; // assuming you want random strings of 12 characters


        QString randomString;
        for(int i=0; i<randomStringLength; ++i)
        {
            int index = qrand() % possibleCharacters.length();
            QChar nextChar = possibleCharacters.at(index);
            randomString.append(nextChar);
        }

        settings.setValue("RemotePCC/passwd", randomString);
        passwd = randomString;

//        ui->lineEdit->setText(randomString);
    }

    if(settings.contains("RemotePCC/port"))
        port = settings.value("RemotePCC/port").toInt();
    else
    {
        // generate random port
        int newPort = qrand() % ((65000 + 1) - 1025) + 1025;

        settings.setValue("RemotePCC/port", newPort);
        port = newPort;

//        ui->spinBox->setValue(port);
    }

    if(settings.contains("RemotePCC/autostart"))
        ui->checkBox->setChecked(settings.value("RemotePCC/autostart").toBool());
    else
        settings.setValue("RemotePCC/autostart", true);

    if(settings.contains("RemotePCC/startinbgd"))
        ui->checkBox_2->setChecked(settings.value("RemotePCC/startinbgd").toBool());
    else
        settings.setValue("RemotePCC/startinbgd", true);

    m_server = new Server(settings.value("RemotePCC/passwd").toString(),
                          settings.value("RemotePCC/port").toInt());
    connect(m_server, SIGNAL(ShowTrayMsg(QString)), this, SLOT(showMessage(QString)));
    m_server->Start();

    ui->lineEdit->setText(passwd);
    ui->spinBox->setValue(port);

    ui->statusBar->showMessage("Status: OK");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::messageClicked()
{

//    QMessageBox::information(0, tr("Systray"),
  //                           tr("Sorry, I already gave what help I could.\n"
    //                            "Maybe you should try asking a human?"));
}

void MainWindow::showMessage(QString msg)
{
    m_systemTrayIcon->showMessage("Remote PC Control", msg, QSystemTrayIcon::Information, 2000);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        emit showNormal();
//        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1)
  //                                    % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
    //    showMessage();
        break;
    default:
        ;
    }
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    QSettings settings;
    settings.setValue("RemotePCC/passwd", arg1);

    m_server->SetPasswd(arg1);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    QSettings settings;
    settings.setValue("RemotePCC/port", arg1);

    m_server->SetPort(arg1);
    m_server->Restart();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("RemotePCC/autostart", checked);

    if(checked)
    {
        QSettings settings2("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                            QSettings::NativeFormat);
        settings2.setValue("RemotePCControl",
                           QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else
    {
        QSettings settings2("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                            QSettings::NativeFormat);
        settings2.remove("RemotePCControl");
    }
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("RemotePCC/startinbgd", checked);
}
