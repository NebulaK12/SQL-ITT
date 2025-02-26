#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize handlers
    networkHandler = new NetworkHandler(this);
    logger = new Logger();
    uiLogic = new UiLogic(this);

    // Populate the payload dropdown
    ui->comboBox_Payload->addItem("1' OR '1'='1' --");
    ui->comboBox_Payload->addItem("1' UNION SELECT null, username, password FROM users --");
    ui->comboBox_Payload->addItem("1' AND SLEEP(5) --");
    ui->comboBox_Payload->addItem("[Custom]");

    ui->pushButton_ExportLog->setEnabled(false);

    // Connect UI signals to slots
    connect(ui->pushButton_Test, &QPushButton::clicked, this, &MainWindow::testSQLInjection);
    connect(ui->pushButton_ExportLog, &QPushButton::clicked, this, &MainWindow::exportLog);
    connect(ui->comboBox_Payload, &QComboBox::currentIndexChanged, this, &MainWindow::updateCustomPayloadState);
    connect(ui->pushButton_DarkMode, &QPushButton::clicked, this, &MainWindow::toggleDarkMode);
    connect(ui->pushButton_About, &QPushButton::clicked, this, &MainWindow::showAboutDialog);

    updateCustomPayloadState(ui->comboBox_Payload->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkHandler;
    delete logger;
    delete uiLogic;
}

void MainWindow::testSQLInjection()
{
    QString url = ui->lineEdit_URL->text();
    QString payload = (ui->comboBox_Payload->currentText() == "[Custom]") ?
                          ui->lineEdit_CustomPayload->text().trimmed() :
                          ui->comboBox_Payload->currentText();

    if (url.isEmpty() || payload.isEmpty()) {
        ui->textEdit_Response->setText("Error: Please enter a target URL and payload.");
        return;
    }

    QString response = networkHandler->testSQLInjection(url, payload);
    ui->textEdit_Response->setText(response);

    if (ui->checkBox_SaveLogs->isChecked()) {
        QString normalResponse = networkHandler->fetchNormalResponse(url);
        logger->saveLog(url, response, normalResponse);
    }

    ui->pushButton_ExportLog->setEnabled(true);
}

void MainWindow::exportLog()
{
    ui->textEdit_Response->setText(logger->exportLog());
}

void MainWindow::updateCustomPayloadState(int index)
{
    uiLogic->updateCustomPayloadState(ui->comboBox_Payload, ui->lineEdit_CustomPayload);
}

void MainWindow::toggleDarkMode()
{
    uiLogic->toggleDarkMode();
}

void MainWindow::showAboutDialog()
{
    uiLogic->showAboutDialog();
}
