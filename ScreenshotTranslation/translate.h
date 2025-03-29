#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QObject>
#include <string>
#include "config.hpp"
#include <QString>

using std::string;

class Translate:public QObject
{
    Q_OBJECT
public:
    Translate();
    ~Translate();

    void doTranslate(const string& text, Language languageTranslateFrom, Language languageTranslateTo=Language::zh);

signals:
    void translateFinished(QString result);
    void setStatus(QString status);
};

#endif // TRANSLATE_H
