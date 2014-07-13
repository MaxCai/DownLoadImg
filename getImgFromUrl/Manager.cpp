#include "Manager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QApplication>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDebug>
#include <QRegExp>
#include <QPixmap>
#include <QStringList>

void ImgGetter::get(char *url)
{
        m_pNetManager->get(QNetworkRequest(QUrl(url)));
}

void ImgGetter::getImgUrl(const QString &string)
{
	QRegExp exp("background-image\\s*:\\s*url\\s*[(](.+)[)]", Qt::CaseInsensitive);
	exp.setMinimal(true);
	if(exp.indexIn(string) >= 0)
	{
                m_imgUrlList.append( exp.cap(1) );
                qDebug() <<"get img url----"<< exp.cap(1);
	}
}

void ImgGetter::loadImg(QNetworkReply *reply)
{
	QPixmap pixmap;
        const static QString pix_name("tuchong_");
        static int cnt = 0;
	if(pixmap.loadFromData(reply->readAll()) == false)
		qDebug()<< "load pixmap failed!";
        if(pixmap.save(pix_name + QString::number(cnt++) + ".png") == false)
		qDebug()<< "save pixmap failed!";
}

void ImgGetter::searchImg(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString string = codec->toUnicode(reply->readAll());


    QRegExp exp("data-pic=\"http://photos.tuchong.com/.+/f/.+jpg", Qt::CaseInsensitive);
    exp.setMinimal(true);

    QRegExp imgHrefExp("http://photos.tuchong.com/.+/f/.+jpg");

    if(exp.indexIn(string) >= 0)
    {
        imgHrefExp.indexIn(exp.cap(0));
        qDebug()<<imgHrefExp.cap(0);
        m_imgUrlList.append(imgHrefExp.cap(0));
    }

    //<a class="icon-batch-large batch-arrow-left" href="http://cheland-zhilin.tuchong.com/6486801/" rel="prev" title="????"></a>
    //<a class="icon-batch-large batch-arrow-right" href="http://cheland-zhilin.tuchong.com/6486717/" rel="next" title="????"></a></div>
    QRegExp exp_left("\"icon-batch-large batch-arrow-left\" href=\".+\"", Qt::CaseInsensitive);
    exp_left.setMinimal(true);

    QRegExp exp_right("\"icon-batch-large batch-arrow-right\" href=\".+\"", Qt::CaseInsensitive);
    exp_right.setMinimal(true);

    QRegExp hrefExp_sel("http://.+/");

    Href href;
    switch(cur_direction)
    {
    case Href::ORIGNIN:
        if(exp_left.indexIn(string) >= 0)
        {
            if(hrefExp_sel.indexIn(exp_left.cap(0)) >= 0)
            {
                qDebug()<<"left----"<<hrefExp_sel.cap(0);
                href.direction = Href::LEFT;
                href.href_name = hrefExp_sel.cap(0);
                m_imgHref.prepend(href);
            }
        }

        if(exp_right.indexIn(string) >= 0)
        {
            if(hrefExp_sel.indexIn(exp_right.cap(0)) >= 0)
            {
                qDebug()<<"right----"<<hrefExp_sel.cap(0);
                href.direction = Href::RIGHT;
                href.href_name = hrefExp_sel.cap(0);
                m_imgHref.prepend(href);
            }
        }

        break;
    case Href::LEFT:
        if(exp_left.indexIn(string) >= 0)
        {
            if(hrefExp_sel.indexIn(exp_left.cap(0)) >= 0)
            {
                qDebug()<<"left----"<<hrefExp_sel.cap(0);
                href.direction = Href::LEFT;
                href.href_name = hrefExp_sel.cap(0);
                m_imgHref.prepend(href);
            }
        }
        break;
    case Href::RIGHT:
        if(exp_right.indexIn(string) >= 0)
        {
            if(hrefExp_sel.indexIn(exp_right.cap(0)) >= 0)
            {
                qDebug()<<"right----"<<hrefExp_sel.cap(0);
                href.direction = Href::RIGHT;
                href.href_name = hrefExp_sel.cap(0);
                m_imgHref.prepend(href);
            }
        }
        break;
    }
}

void ImgGetter::searchPixmapHome(const QString &string)
{
    QRegExp exp("data-location=\"content\"\\s+href=\".+\"", Qt::CaseInsensitive);
    exp.setMinimal(true);

    QRegExp hrefExp("http://.+/");

    int pos = 0;
    int count = 0;

    state = GET_IMG_HREF;

    Href href;

    while(true)
    {
        pos = exp.indexIn(string, pos);
        if(pos < 0)
            break;

        count++;
        pos += exp.matchedLength();

        //qDebug()<<"cap 0---"<<exp.cap(0);
        hrefExp.indexIn(exp.cap(0));

        qDebug()<< "search href "<< hrefExp.cap(0);
        //m_pNetManager->get(QNetworkRequest(QUrl(hrefExp.cap(0))));
        href.href_name = hrefExp.cap(0);
        href.direction = Href::ORIGNIN;
        m_imgHref.append(href);
    }
    qDebug()<<"cap cnt "<<count;

    emit hrefGot();
}

void ImgGetter::getHref()
{
    static int cnt = 0;

    if(++cnt > 100) //too many pixmap
    {
        qDebug() << "too many pixmap";
        state = GET_LOAD_IMG;
        emit imgGot();
    }

    if(m_imgHref.isEmpty())
    {
        state = GET_LOAD_IMG;
        emit imgGot();

        return;
    }

    cur_direction = m_imgHref.last().direction;
    m_pNetManager->get(QNetworkRequest(QUrl(m_imgHref.last().href_name)));
    m_imgHref.removeLast();
}

void ImgGetter::getImg()
{
    if(m_imgUrlList.isEmpty())
        return;

    qDebug() << "----" << m_imgUrlList.last();
    m_pNetManager->get(QNetworkRequest(QUrl(m_imgUrlList.last())));
    m_imgUrlList.removeLast();
}

void ImgGetter::replyFinished(QNetworkReply *reply)
{
	if(reply == NULL || reply->error() != QNetworkReply::NoError)
		return;

        switch(state)
        {
        case GET_HOME:
            {

                QTextCodec *codec = QTextCodec::codecForName("UTF-8");
                QString string = codec->toUnicode(reply->readAll());

                getImgUrl(string);
                searchPixmapHome(string);
            }
            break;
        case GET_IMG_HREF:
            {
                searchImg(reply);

                emit hrefGot();
                break;
            }
        case GET_LOAD_IMG:
            {

                loadImg(reply);
                emit imgGot();
                break;
            }
        }
	reply->deleteLater();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

//	if(argc <= 1)
//		return 0;


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

        char *host = "http://tuchong.com/";
	ImgGetter imgGetter;
        imgGetter.get(host);

	return app.exec();
}
