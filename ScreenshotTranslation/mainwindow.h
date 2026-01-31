#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autofontsizelabel.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include "ocr.h"
#include "screenshotwidget.h"
#include "translate.h"
#include "config.hpp"
#include "pronounce.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTextEdit* m_textedit_textOCR = nullptr;
    QWidget* m_widget_center = nullptr;
    QPushButton* m_btn_volume = nullptr;
    QPushButton* m_btn_justTranslate = nullptr;
    QPushButton* m_btn_command = nullptr;
    QLabel* m_label_phonetic = nullptr;
    Pronounce* m_pronounce = nullptr;
    // QLabel* m_label_mode = nullptr;
    QTextEdit* m_textedit_textTran = nullptr;
    QLabel* m_label_ocrLanguage = nullptr;
    QSpacerItem* m_placeholderLeft = nullptr;
    QLabel* m_label_status=nullptr;
    QSpacerItem* m_placeholderRight = nullptr;
    QLabel* m_label_transLanguage=nullptr;

    QWidget* content = nullptr;
    QHBoxLayout* m_hlayTop = nullptr;
    QVBoxLayout* m_hlayTopCenter = nullptr;
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


    //收缩
    enum class Model{shrink, expand};
    Model curModel=Model::expand;
    void switchModel();
    QRect expandRect;
    QRect shrinkRect;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void ocrLanguageNotChanged();
};
#endif // MAINWINDOW_H
