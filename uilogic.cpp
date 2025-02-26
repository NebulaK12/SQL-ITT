#include "uilogic.h"
#include <QApplication>

UiLogic::UiLogic(QObject *parent) : QObject(parent) {}

void UiLogic::updateCustomPayloadState(QComboBox *comboBox, QLineEdit *lineEdit)
{
    if (comboBox->currentText() == "[Custom]") {
        lineEdit->setEnabled(true);
        lineEdit->setPlaceholderText("Enter your SQL Injection payload...");
    } else {
        lineEdit->setEnabled(false);
        lineEdit->clear();
    }
}

void UiLogic::toggleDarkMode()
{
    static bool darkModeEnabled = false;

    if (!darkModeEnabled) {
        qApp->setStyleSheet("QMainWindow { background-color: #121212; color: #E0E0E0; }");
    } else {
        qApp->setStyleSheet("");
    }

    darkModeEnabled = !darkModeEnabled;
}

void UiLogic::showAboutDialog()
{
    QMessageBox::information(nullptr, "About SQLi Tester",
                             "SQLi Tester v1.0\n\n"
                             "Developed for ethical penetration testing purposes.\n"
                             "Use this tool responsibly and only on systems you have permission to test.\n\n"
                             "Created by: NebulaK12\n"
                             "License: Open Source\n");
}
