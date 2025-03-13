#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autofontsizelabel.h"
#include <QHBoxLayout>
#include "ocr.h"
#include "screenshotwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AutoFontSizeLabel* m_label_textOCR = nullptr;
    AutoFontSizeLabel* m_label_textTran = nullptr;
    QWidget* content = nullptr;
    QHBoxLayout* m_hlay = nullptr;
    Ocr* m_ocr = nullptr;
    ScreenshotWidget* m_shot=nullptr;
    QString m_ocrLanguage;

    void initUI();
    void installGlobalEventFilter();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MAINWINDOW_H
