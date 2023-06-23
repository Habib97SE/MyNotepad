#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_As_triggered();

    void on_actionFind_triggered();

    void on_actionFind_Replace_triggered();

    void on_actionFont_size_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_textContent_textChanged();

    void on_actionPrint_triggered();

private:
    Ui::MainWindow *ui;
    QString filePath;
    bool hasUnsavedChanges;
};
#endif // MAINWINDOW_H
