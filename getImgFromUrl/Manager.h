#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkReply;

class ImgGetter : public QObject
{
	Q_OBJECT
public:
	ImgGetter(QObject *parent = 0)
		:QObject(parent),
		m_pNetManager(new QNetworkAccessManager(this))
	{
		connect(m_pNetManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
	}
	~ImgGetter(){}

	void get(char *url);
private:
	void getImgUrl(QNetworkReply *reply);
	void loadImg(QNetworkReply *reply);

public slots:
	void replyFinished(QNetworkReply *);

private:
	QNetworkAccessManager *m_pNetManager;
	QString m_imgUrl;
};

#endif