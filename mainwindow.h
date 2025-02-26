#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>  // Fix missing include
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCustomPayloadState(int index);
    void testSQLInjection();
    void exportLog();
    QString fetchNormalResponse(const QString &url);
    void toggleDarkMode();
    void showAboutDialog();
    void saveLog(const QString &request, const QString &response, const QString &normalResponse);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString logFilePath = "SQLi_Log.txt";
    bool darkModeEnabled = false;
};

#endif // MAINWINDOW_H
