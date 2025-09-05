#ifndef ARMENIANTRANSLATOR_HPP
#define ARMENIANTRANSLATOR_HPP

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

class ArmenianTranslator : public QWidget
{
    using baseClass = QWidget;
    Q_OBJECT

public:
    explicit ArmenianTranslator(QWidget* parent = nullptr);
    void translateToArmenian(const QString& text);

signals:
    void translationReady(const QString& translatedText);
    void errorOccurred(const QString& errorString);

private slots:
    void onReply(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_manager;
};

#endif // ARMENIANTRANSLATOR_HPP
