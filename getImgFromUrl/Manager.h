#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkReply;
#include <QList>
#include <QString>
#include <QStringList>

struct Href
{
    QString href_name;

    enum DIRECTION
    {
        ORIGNIN,
        RIGHT,
        LEFT
    };

    DIRECTION direction;
};

class ImgGetter : public QObject
{
	Q_OBJECT
public:
	ImgGetter(QObject *parent = 0)
		:QObject(parent),
                m_pNetManager(new QNetworkAccessManager(this)),
                state(GET_HOME)
	{
		connect(m_pNetManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

                connect(this, SIGNAL(hrefGot()), this, SLOT(getHref()));
                connect(this, SIGNAL(imgGot()), this, SLOT(getImg()));
            }
	~ImgGetter(){}

        void get(char *url);
private:
        void getImgUrl(const QString &string);
	void loadImg(QNetworkReply *reply);

        void searchPixmapHome(const QString &string);

        void searchImg(QNetworkReply *reply);
signals:
        void hrefGot();
        void imgGot();
public slots:
	void replyFinished(QNetworkReply *);
        void getHref();
        void getImg();

private:
	QNetworkAccessManager *m_pNetManager;
        QList<Href> m_imgHref;
        QStringList m_imgUrlList;

        int state;
        int cur_direction;

        enum
        {
            GET_HOME,
            GET_IMG_HREF,
            GET_LOAD_IMG
        };
};

#endif
