#ifndef UTILITY_H
#define UTILITY_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

void logJson(const QJsonObject &p_jsonObject);

bool saveJsonObject(const QJsonObject &p_jsonObject, const std::string &p_fileName);

QJsonObject loadJsonObjectFromFile(const std::string &p_fileName);

#endif // UTILITY_H
