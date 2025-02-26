#ifndef UILOGIC_H
#define UILOGIC_H

#include <QObject>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

class UiLogic : public QObject
{
    Q_OBJECT

public:
    explicit UiLogic(QObject *parent = nullptr);
    void updateCustomPayloadState(QComboBox *comboBox, QLineEdit *lineEdit);
    void toggleDarkMode();
    void showAboutDialog();
};

#endif // UILOGIC_H
