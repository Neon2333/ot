#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autofontsizelabel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include<QSpacerItem>
#include "ocr.h"
#include "screenshotwidget.h"
#include "translate.h"
#include "config.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AutoFontSizeLabel* m_label_textOCR = nullptr;
    AutoFontSizeLabel* m_label_textTran = nullptr;
    QLabel* m_label_status=nullptr;
    QSpacerItem* m_placeholder = nullptr;
    QLabel* m_label_ocrLanguage = nullptr;
    QWidget* content = nullptr;
    QHBoxLayout* m_hlayTop = nullptr;
    QHBoxLayout* m_hlayBottom = nullptr;
    QVBoxLayout* m_vlay = nullptr;
    Ocr* m_ocr = nullptr;
    ScreenshotWidget* m_shot=nullptr;
    config::Language m_ocrLanguage{config::Language::none};
    Translate* m_translate;

    void initUI();
    void installGlobalEventFilter();
    void setOcrLanguage(config::Language language);
    config::Language ocrLanguage();
    void close();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void ocrLanguageNotChanged();
};
#endif // MAINWINDOW_H
