#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QBoxLayout>
#include <QPrinter>
#include <QPrintDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasUnsavedChanges(false)
{
    ui->setupUi(this);
    MainWindow::setWindowIcon(QIcon("C:\\Users\\hheza\\OneDrive\\Documents\\MyNotepad\\OIP.jpg"));
    connect(ui->textContent, &QTextEdit::textChanged, this, &MainWindow::on_textContent_textChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSave_triggered()
{
    QString content = ui->textContent->toPlainText();
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
        QFileInfo fileInfo(filePath);
        QString fileTitle = fileInfo.fileName();
        MainWindow::setWindowTitle(fileTitle);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to write to the file."));
    }
}


void MainWindow::on_actionOpen_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Text Files (*.txt)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();
            ui->textContent->setPlainText(fileContent);
            QFileInfo fileInfo(filePath);
            QString fileTitle = fileInfo.fileName();
            MainWindow::setWindowTitle(fileTitle);
        } else {
            // Failed to open the file
            QMessageBox::warning(this, tr("Error"), tr("Failed to open the file."));
        }
    }
}



void MainWindow::on_actionNew_triggered()
{
    ui->textContent->clear();
    MainWindow::setWindowTitle("");
}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << ui->textContent->toPlainText();
            hasUnsavedChanges = false;
            file.close();
            QFileInfo fileInfo(filePath);
            QString fileTitle = fileInfo.fileName();
            MainWindow::setWindowTitle(fileTitle);
        }
    }
}


void MainWindow::on_actionFind_triggered()
{
    bool ok;
    QString searchedText = QInputDialog::getText(this, tr("Input Dialog"), tr("Enter your name:"), QLineEdit::Normal, "", &ok);
    if (!ok || searchedText.isEmpty()) {
         QMessageBox(QMessageBox::Warning, "Error", "Error while getting your input", QMessageBox::Ok);
         return;
    }
    QTextCursor cursor(ui->textContent->document());
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow);
    int count= 0;
    // Loop until no more occurrences are found
    while (cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor)) {
        if (cursor.selectedText() == searchedText) {
            // Apply the highlight format to the selected text
            cursor.mergeCharFormat(highlightFormat);
        }
        count++;
    }
    if (count == 0)
        QMessageBox(QMessageBox::Information, "No match found", "No match were found.", QMessageBox::Ok);

}


void MainWindow::on_actionFind_Replace_triggered()
{
    bool ok;
    QString oldPhrase = QInputDialog::getText(this, tr("Old Phrase"), tr("Enter old: "), QLineEdit::Normal, "", &ok);
    QString newPhrase = QInputDialog::getText(this, tr("New Phrase"), tr("Enter New: "), QLineEdit::Normal, "", &ok);
    if (!ok || oldPhrase.isEmpty() || newPhrase.isEmpty()) {
        QMessageBox(QMessageBox::Warning, "Error", "Error while getting your input", QMessageBox::Ok);
        return;
   }
   QTextCursor cursor(ui->textContent->document());
   QTextCharFormat highlightFormat;
   highlightFormat.setBackground(Qt::yellow);
   int count = 0;
   while (!cursor.isNull() && !cursor.atEnd()) {
       cursor = ui->textContent->document()->find(oldPhrase, cursor);
       if (!cursor.isNull()) {
           cursor.insertText(newPhrase);
       }
       count++;
   }
   // if number of occurences is zero, then show appropriate message.
   if (count == 0)
       QMessageBox(QMessageBox::Information, "No match found", "No match were found.", QMessageBox::Ok);
}


void MainWindow::on_actionFont_size_triggered()
{
    int fontSizeInInteger;
    bool exitLoop = false;
    while (!exitLoop) {
        try {
            QFont font = ui->textContent->font();

            bool ok;
            QString fontSize= QInputDialog::getText(this, tr("Font size"), tr("Enter the font size: "), QLineEdit::Normal, QString::fromStdString(std::to_string(ui->textContent->fontPointSize())), &ok);
            fontSizeInInteger = fontSize.toInt();
            exitLoop = true;
        }  catch (...) {
            QMessageBox(QMessageBox::Warning, "Error", "Something went wrong while reading your input.", QMessageBox::Ok);
        }
    }
    QFont font = ui->textContent->font();
    font.setPointSize(fontSizeInInteger);
    ui->textContent->setFont(font);
}


void MainWindow::on_actionBold_triggered()
{
    // Toggle between bolded or not
    QTextCharFormat format;
    format.setFontWeight(ui->textContent->fontWeight()== QFont::Bold ? QFont::Normal : QFont::Bold);
    ui->textContent->setCurrentCharFormat(format);
}


void MainWindow::on_actionItalic_triggered()
{
    QTextCharFormat format;
    format.setFontItalic(ui->textContent->fontItalic() ? false : true);
    ui->textContent->setCurrentCharFormat(format);
}


void MainWindow::on_textContent_textChanged()
{
    if (!hasUnsavedChanges) {
            QString fileTitle = MainWindow::windowTitle();
            MainWindow::setWindowTitle(fileTitle + "*");
            hasUnsavedChanges = true;
        }
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QTextDocument* document = ui->textContent->document();
        document->print(&printer);
    }
}

