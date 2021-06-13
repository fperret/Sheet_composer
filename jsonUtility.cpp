#include "Utility.h"

void logJson(const QJsonObject &p_jsonObject)
{
    qDebug() << "=================================";
    for (QJsonObject::const_iterator l_it = p_jsonObject.begin(); l_it != p_jsonObject.end(); ++l_it)
    {
        qDebug() << l_it.key() << " / " << l_it.value();
    }
    qDebug() << "---------------------------------";
}

bool saveJsonObject(const QJsonObject &p_jsonObject, const std::string &p_fileName)
{
    // Try to open the file
    QFile l_saveFile(QString(p_fileName.c_str()));
    if (!l_saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open " << p_fileName.c_str() << " when saving.";
        return false;
    }

    // Write the json in the file
    QJsonDocument l_jsonDoc(p_jsonObject);
    if (l_saveFile.write(l_jsonDoc.toJson()) == -1) {
        qWarning() << "Could not write to " << p_fileName.c_str() << " when saving.";
        return false;
    }

    return true;
}

QJsonObject loadJsonObjectFromFile(const std::string &p_fileName)
{
    QFile l_loadFile(QString(p_fileName.c_str()));

    // Use ReadWrite to create the file if it does not exists
    if (!l_loadFile.open(QIODevice::ReadWrite)) {
        qWarning() << "Could not open " << p_fileName.c_str() << " when loading.";
        return QJsonObject();
    }
    QByteArray l_jsonData = l_loadFile.readAll();
    QJsonDocument l_jsonDoc(QJsonDocument::fromJson(l_jsonData));
    //l_jsonObject = l_jsonDoc.object();
    return l_jsonDoc.object();
}
