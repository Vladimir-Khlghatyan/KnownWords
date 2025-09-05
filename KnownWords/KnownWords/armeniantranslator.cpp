#include "armeniantranslator.hpp"
#include <QJsonDocument>
#include <QJsonArray>
ArmenianTranslator::ArmenianTranslator(QWidget* parent)
    : baseClass(parent)
    , m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &ArmenianTranslator::onReply);
}

void ArmenianTranslator::translateToArmenian(const QString& text)
{
    if (text.trimmed().isEmpty()) {
        return;
    }

    QUrl url("https://translate.googleapis.com/translate_a/single");
    QUrlQuery query;
    query.addQueryItem("client", "gtx");
    query.addQueryItem("sl", "en");
    query.addQueryItem("tl", "hy");
    query.addQueryItem("dt", "t");
    query.addQueryItem("q", text);

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    m_manager->get(request);
}

void ArmenianTranslator::onReply(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOccurred("Network error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        emit errorOccurred("JSON parse error: " + parseError.errorString());
        return;
    }

    QString translated;
    if (doc.isArray() && !doc.array().isEmpty())
    {
        QJsonArray arr1 = doc.array().first().toArray();
        if (!arr1.isEmpty())
        {
            QJsonArray arr2 = arr1.first().toArray();
            if (!arr2.isEmpty()) {
                translated = arr2.first().toString();
            }
        }
    }

    if (translated.isEmpty()) {
        emit errorOccurred("Failed to parse translation.");
    } else {
        emit translationReady(translated);
    }
}
