#include "Manager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QApplication>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDebug>
#include <QRegExp>
#include <QImage>
#include <QPixmap>

void ImgGetter::get(char *url)
{
	m_pNetManager->get(QNetworkRequest(QUrl(url)));
}

void ImgGetter::getImgUrl(QNetworkReply *reply)
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QString string = codec->toUnicode(reply->readAll());

	QRegExp exp("background-image\\s*:\\s*url\\s*[(](.+)[)]", Qt::CaseInsensitive);
	exp.setMinimal(true);
	if(exp.indexIn(string) >= 0)
	{
		m_imgUrl = exp.cap(1);
		qDebug() <<"get img url----"<< m_imgUrl;

		m_pNetManager->get(QNetworkRequest(QUrl(m_imgUrl)));
	}
}

void ImgGetter::loadImg(QNetworkReply *reply)
{
	QPixmap pixmap;
	if(pixmap.loadFromData(reply->readAll()) == false)
		qDebug()<< "load pixmap failed!";
	if(pixmap.save("tuchong_background.png") == false)
		qDebug()<< "save pixmap failed!";
}

void ImgGetter::replyFinished(QNetworkReply *reply)
{
	if(reply == NULL || reply->error() != QNetworkReply::NoError)
		return;

	if(m_imgUrl.isEmpty())
		getImgUrl(reply);
	else
		loadImg(reply);

	

	reply->deleteLater();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	if(argc <= 1)
		return 0;


	//ÉèÖÃ×Ö·û¼¯
//#ifdef WIN32
//	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
//	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
//	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//#else
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//#endif

	ImgGetter imgGetter;
	imgGetter.get(argv[1]);

	return app.exec();
}