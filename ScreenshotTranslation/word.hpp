#ifndef WORD_H
#define WORD_H
#include <QString>
#include <vector>
#include <array>

using std::vector;
using std::array;

inline static bool isWord = false;

struct Word
{
    //spelling
    QString spelling;
    //meanings
    vector<array<QString,4>> meanings;
    //partOfSpeech
    QString partOfSpeech;
    //phoetics
    QString phoetics;
    //audioURL
    QString audioURL;
    //synonyms
    QString synonyms;
    //antonyms
    QString antonyms;
};

inline static Word word;

#endif // WORD_H
