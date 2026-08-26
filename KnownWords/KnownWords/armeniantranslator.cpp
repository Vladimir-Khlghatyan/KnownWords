#include "armeniantranslator.hpp"
#include "define.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#ifdef _USE_GOOGLE_TRANSLATE_
#include <QJsonArray>
#endif
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

#ifdef _USE_GOOGLE_TRANSLATE_
    // Google's unofficial gtx endpoint - disabled 2026-08-26, Google started returning
    // HTTP 429 "automated queries" blocks for this client. Define _USE_GOOGLE_TRANSLATE_
    // in define.hpp to re-enable if it becomes reliable again.
    QUrl url("https://translate.googleapis.com/translate_a/single");
    QUrlQuery query;
    query.addQueryItem("client", "gtx");
    query.addQueryItem("sl", "en");
    query.addQueryItem("tl", "hy");
    query.addQueryItem("dt", "t");
    query.addQueryItem("q", text);
#else
    QUrl url("https://api.mymemory.translated.net/get");
    QUrlQuery query;
    query.addQueryItem("q", text);
    query.addQueryItem("langpair", "en|hy");
#endif

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

#ifdef _USE_GOOGLE_TRANSLATE_
    // Matching parse branch for the Google gtx response above: [[[ "translated", ... ]], ...]
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
#else
    const QJsonObject root = doc.object();
    const QString translated = root.value("responseData").toObject().value("translatedText").toString();
    const int responseStatus = root.value("responseStatus").toInt();

    if (translated.isEmpty() || responseStatus != 200 || translated.startsWith("MYMEMORY WARNING", Qt::CaseInsensitive)) {
        emit errorOccurred("Failed to parse translation.");
    } else {
        emit translationReady(translated);
    }
#endif
}
