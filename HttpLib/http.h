/** \flie ********************************************************************
 *
 *	post和get请求接口
 *
 * \author  : ZZC
 * \date    : 2020年8月28日14:25:41
 * \version : 1.0
 * \brief   : 提供post和get请求接口
 ****************************************************************************/

#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
class http : public QObject
{
	Q_OBJECT
public:
	explicit http(QObject* parent = nullptr);

	/** \fn postSyn
	 *
	 *	 post请求
	 *
	 *	\param url :网址
	 *	\param headerdata :请求头数据
	 *  \param requestData:请求数据
	 *  \param replydata:返回数据
	 */
	bool postSyn(QString url, QMap<QString, QString> headerdata, QByteArray requestData, QByteArray& replydata);

	/** \fn getSyn
	 *
	 *	 get请求
	 *
	 *	\param url :网址
	 *	\param headerdata :请求头数据
	 *  \param replydata:返回数据
	 */
	bool getSyn(QString url, QMap<QString, QString> headerdata, QByteArray& replydata);
signals:

public slots:
};
#endif // HTTP_H
