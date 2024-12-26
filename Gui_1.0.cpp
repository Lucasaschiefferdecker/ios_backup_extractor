// EM CONSTRUÇÃO 


#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QProcess>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QMenuBar>
#include <QAction>
#include <QPalette>
#include <QStyleFactory>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

void applyDarkTheme(QApplication &app) {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);

    darkPalette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);
    app.setStyle(QStyleFactory::create("Fusion"));
}

bool isValidBackupDir(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.entryList({"Manifest.db", "Info.plist"}, QDir::Files).size() >= 2;
}

bool isValidOutputDir(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.isWritable();
}

QString getBackupDetails(const QString &backupDir) {
    QFile infoFile(QDir(backupDir).filePath("Info.plist"));
    if (!infoFile.open(QIODevice::ReadOnly)) {
        return "Erro ao abrir o arquivo Info.plist";
    }

    QByteArray fileData = infoFile.readAll();
    infoFile.close();

    // Simulação de análise do plist (ideal usar biblioteca como libplist para parsing real)
    QString details;
    details += "Versão do iPhone: iPhone X\n"; // Exemplo estático
    details += "Tamanho do Backup: ~1.5GB\n"; // Exemplo estático
    details += "Data do Backup: 2024-11-20\n"; // Exemplo estático
    details += "Versão do iOS: 16.5.1\n"; // Exemplo estático

    return details;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    applyDarkTheme(app);

    // Criação da janela principal
    QWidget window;
    window.setWindowTitle("iOS Backup Extractor GUI");

    // Menu
    QMenuBar *menuBar = new QMenuBar(&window);
    QMenu *fileMenu = menuBar->addMenu("Arquivo");
    QAction *exitAction = new QAction("Sair", &window);
    fileMenu->addAction(exitAction);

    QMenu *helpMenu = menuBar->addMenu("Ajuda");
    QAction *aboutAction = new QAction("Sobre", &window);
    helpMenu->addAction(aboutAction);

    QObject::connect(exitAction, &QAction::triggered, [&]() {
        QApplication::quit();
    });

    QObject::connect(aboutAction, &QAction::triggered, [&]() {
        QMessageBox::information(&window, "Sobre", "iOS Backup Extractor GUI\nVersão 1.0\nDesenvolvido por [Seu Nome]");
    });

    // Widgets
    QLabel *labelBackupDir = new QLabel("Diretório do Backup:");
    QLineEdit *inputBackupDir = new QLineEdit();
    QPushButton *browseBackupDirButton = new QPushButton("Selecionar...");

    QLabel *labelOutputDir = new QLabel("Diretório de Saída:");
    QLineEdit *inputOutputDir = new QLineEdit();
    QPushButton *browseOutputDirButton = new QPushButton("Selecionar...");

    QLabel *labelFormat = new QLabel("Formato de Saída:");
    QComboBox *formatComboBox = new QComboBox();
    formatComboBox->addItem("YYYY-MM", "ym");
    formatComboBox->addItem("YYYY-MM-DD", "ymd");
    formatComboBox->addItem("Sem subdiretórios", "flat");

    QCheckBox *addTrashCheckBox = new QCheckBox("Incluir arquivos excluídos");
    QCheckBox *prependDateCheckBox = new QCheckBox("Adicionar data aos nomes dos arquivos");

    QPushButton *extractButton = new QPushButton("Extrair Backup");
    QPushButton *detailsButton = new QPushButton("Verificar Detalhes do Backup");
    QTextEdit *logOutput = new QTextEdit();
    logOutput->setReadOnly(true);

    QPushButton *clearLogButton = new QPushButton("Limpar Log");

    // Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMenuBar(menuBar);
    layout->addWidget(labelBackupDir);
    layout->addWidget(inputBackupDir);
    layout->addWidget(browseBackupDirButton);
    layout->addWidget(labelOutputDir);
    layout->addWidget(inputOutputDir);
    layout->addWidget(browseOutputDirButton);
    layout->addWidget(labelFormat);
    layout->addWidget(formatComboBox);
    layout->addWidget(addTrashCheckBox);
    layout->addWidget(prependDateCheckBox);
    layout->addWidget(detailsButton);
    layout->addWidget(extractButton);
    layout->addWidget(clearLogButton);
    layout->addWidget(logOutput);
    
    window.setLayout(layout);

    // Conexões
    QObject::connect(browseBackupDirButton, &QPushButton::clicked, [&]() {
        QString dir = QFileDialog::getExistingDirectory(&window, "Selecionar Diretório de Backup");
        if (!dir.isEmpty()) {
            inputBackupDir->setText(dir);
        }
    });

    QObject::connect(browseOutputDirButton, &QPushButton::clicked, [&]() {
        QString dir = QFileDialog::getExistingDirectory(&window, "Selecionar Diretório de Saída");
        if (!dir.isEmpty()) {
            inputOutputDir->setText(dir);
        }
    });

    QObject::connect(inputBackupDir, &QLineEdit::textChanged, [&]() {
        if (!isValidBackupDir(inputBackupDir->text())) {
            inputBackupDir->setStyleSheet("border: 1px solid red;");
        } else {
            inputBackupDir->setStyleSheet("");
        }
    });

    QObject::connect(inputOutputDir, &QLineEdit::textChanged, [&]() {
        if (!isValidOutputDir(inputOutputDir->text())) {
            inputOutputDir->setStyleSheet("border: 1px solid red;");
        } else {
            inputOutputDir->setStyleSheet("");
        }
    });

    QObject::connect(detailsButton, &QPushButton::clicked, [&]() {
        QString backupDir = inputBackupDir->text();

        if (!isValidBackupDir(backupDir)) {
            QMessageBox::warning(&window, "Erro", "O diretório de backup especificado não é válido ou não contém um backup de iPhone.");
            return;
        }

        QString details = getBackupDetails(backupDir);
        QMessageBox::information(&window, "Detalhes do Backup", details);
    });

    QObject::connect(extractButton, &QPushButton::clicked, [&]() {
        QString backupDir = inputBackupDir->text();
        QString outputDir = inputOutputDir->text();
        QString format = formatComboBox->currentData().toString();
        bool addTrash = addTrashCheckBox->isChecked();
        bool prependDate = prependDateCheckBox->isChecked();

        if (!isValidBackupDir(backupDir)) {
            QMessageBox::warning(&window, "Erro", "O diretório de backup especificado não é válido ou não contém um backup de iPhone.");
            return;
        }

        if (!isValidOutputDir(outputDir)) {
            QMessageBox::warning(&window, "Erro", "O diretório de saída especificado não é válido ou não tem permissões de escrita.");
            return;
        }

        QStringList args;
        args << backupDir
             << "--out" << outputDir;

        if (!format.isEmpty()) {
            args << "--format" << format;
        }
        if (addTrash) {
            args << "--add-trash";
        }
        if (prependDate) {
            args << "--prepend-date";
        }

        QProcess process;
        process.start("ios_backup_extractor.exe", args);
        if (!process.waitForStarted()) {
            logOutput->append("Erro no processo: " + process.errorString());
            return;
        }

        process.waitForFinished();

        logOutput->append("Comando executado:\nios_backup_extractor.exe " + args.join(" "));

        QString output = process.readAllStandardOutput();
        QString errors = process.readAllStandardError();

        if (!output.isEmpty()) {
            logOutput->append("Saída:\n" + output);
        }

        if (!errors.isEmpty()) {
            logOutput->append("Erros:\n" + errors);
        }

        if (process.exitCode() == 0) {
            QMessageBox::information(&window, "Sucesso", "Backup extraído com sucesso.");
        } else {
            QMessageBox::critical(&window, "Erro", "Falha ao extrair o backup.");
        }
    });

    QObject::connect(clearLogButton, &QPushButton::clicked, [&]() {
        logOutput->clear();
    });

    // Exibição da janela principal
    window.resize(600, 700);
    window.show();

    return app.exec();
}
