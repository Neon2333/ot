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

    QString doTranslate(const string& text, config::Language languageTranslateFrom, config::Language languageTranslateTo=config::Language::zh);

signals:
    void translateFinished(QString result);
};

#endif // TRANSLATE_H
