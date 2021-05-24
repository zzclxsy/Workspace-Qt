#include "HttpLib/http.h"

http::http(QObject* parent) : QObject(parent)
{
}

bool http::postSyn(QString url, QMap<QString, QString> headerdata, QByteArray requestData, QByteArray& replydata)
{
	//发送请求的对象
	QNetworkAccessManager manager;
	//请求对象
	QNetworkRequest request;
	request.setUrl(url);

	QMapIterator<QString, QString> it(headerdata);  //迭代器
	while (it.hasNext())                            //遍历Map
	{
		it.next();
		request.setRawHeader(it.key().toLatin1(), it.value().toLatin1());
	}

	QNetworkReply* reply = manager.post(request, requestData);
	QEventLoop l;
	//一旦服务器返回，reply会发出信号
	connect(reply, &QNetworkReply::finished, &l, &QEventLoop::quit);
	l.exec();
	//死循环，reply发出信号， 结束循环

	if (reply != nullptr && reply->error() == QNetworkReply::NoError)
	{
		replydata = reply->readAll(); //读取服务 器返回的数据
		return true;
	}
	else
	{
		qDebug() << "请求失败";
		return false;
	}
}

bool http::getSyn(QString url, QMap<QString, QString> headerdata, QByteArray& replydata)
{
	//发送请求的对象
	QNetworkAccessManager manager;
	//请求对象
	QNetworkRequest request;
	request.setUrl(url);

	QMapIterator<QString, QString> it(headerdata);  //迭代器
	while (it.hasNext())                            //遍历Map
	{
		it.next();
		request.setRawHeader(it.key().toLatin1(), it.value().toLatin1());
	}
	QNetworkReply* reply = manager.get(request);
	QEventLoop l;
	//一旦服务器返回，reply会发出信号
	connect(reply, &QNetworkReply::finished, &l, &QEventLoop::quit);
	l.exec();

	if (reply != nullptr && reply->error() == QNetworkReply::NoError)
	{
		replydata = reply->readAll(); //读取服务 器返回的数据
		return true;
	}
	else
	{
		qDebug() << "请求失败";
		return false;
	}
}
