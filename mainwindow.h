#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkhandler.h"
#include "logger.h"
#include "uilogic.h"

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
    void toggleDarkMode();
    void showAboutDialog();

private:
    Ui::MainWindow *ui;
    NetworkHandler *networkHandler;
    Logger *logger;
    UiLogic *uiLogic;
};

#endif // MAINWINDOW_H
