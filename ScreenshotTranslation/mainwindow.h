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
    QLabel* m_label_ocrLanguage = nullptr;
    QSpacerItem* m_placeholderLeft = nullptr;
    QLabel* m_label_status=nullptr;
    QSpacerItem* m_placeholderRight = nullptr;
    QLabel* m_label_transLanguage=nullptr;

    QWidget* content = nullptr;
    QHBoxLayout* m_hlayTop = nullptr;
    QHBoxLayout* m_hlayBottom = nullptr;
    QVBoxLayout* m_vlay = nullptr;
    Ocr* m_ocr = nullptr;
    ScreenshotWidget* m_shot=nullptr;
    Language m_ocrLanguage{Language::none};
    Language m_transLanguage{Language::none};
    Translate* m_translate;

    void initUI();
    void installGlobalEventFilter();
    void setOcrLanguage(Language language);
    void setTransLanguage(Language language);
    Language ocrLanguage();
    void Close();
    void setStatus(QString status);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void ocrLanguageNotChanged();
};
#endif // MAINWINDOW_H
