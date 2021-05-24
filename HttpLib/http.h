/** \flie ********************************************************************
 *
 *	post��get����ӿ�
 *
 * \author  : ZZC
 * \date    : 2020��8��28��14:25:41
 * \version : 1.0
 * \brief   : �ṩpost��get����ӿ�
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
	 *	 post����
	 *
	 *	\param url :��ַ
	 *	\param headerdata :����ͷ����
	 *  \param requestData:��������
	 *  \param replydata:��������
	 */
	bool postSyn(QString url, QMap<QString, QString> headerdata, QByteArray requestData, QByteArray& replydata);

	/** \fn getSyn
	 *
	 *	 get����
	 *
	 *	\param url :��ַ
	 *	\param headerdata :����ͷ����
	 *  \param replydata:��������
	 */
	bool getSyn(QString url, QMap<QString, QString> headerdata, QByteArray& replydata);
signals:

public slots:
};
#endif // HTTP_H
