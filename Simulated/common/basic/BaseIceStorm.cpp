#include "BaseIceStorm.h"

#include <sstream>

std::string BaseIceStorm::m_iceStormIp = "127.0.0.1";
int BaseIceStorm::m_iceStormPort = 10000;

/** 
* icestorm订阅方法
*
*@param object:对象实例
*@param identity:对象的identity
*@param deliverModel:订阅模式，值为twoway,oneway,datagram,batch_oneway,batch_datagram
*@param reliability:qos的reliability，值为空或为ordered
*@param retryCount:deliverModel为twoway时，设置此值表示icestorm重新发送次数
* @return boolean:true表示成功，false表示失败
*/
bool BaseIceStorm::Subscriber(Ice::CommunicatorPtr &communicatorPtr, Ice::ObjectPrx &objectPrx, std::string &strTopic, std::string &strDeliverModel, std::string &strReliability, std::string &strRetryCount)
{
	bool blnValue = true;
	try
	{
		IceStorm::TopicPrx topic = GetTopicProxy(communicatorPtr, strTopic);
		if(topic != 0)
		{	
			//设置qos
			IceStorm::QoS qos;
			if(strDeliverModel == "twoway")
			{
				if(strReliability != "")
				{
					qos["reliability"] = "ordered";
				}
				if(strRetryCount != "")
				{
					qos["retryCount"] = strRetryCount;
				}
			}
		
		
			//设置订阅者传输方式，分为twoway,oneway,batch_oneway,双向传输，不用设置，默认为twoway
			if (strDeliverModel == "twoway" || strDeliverModel == "oneway" || strDeliverModel == "batch_oneway")
			{
				if(strDeliverModel == "oneway")
				{
					objectPrx = objectPrx->ice_oneway();
				}
				else if(strDeliverModel == "batch_oneway")
				{
					objectPrx = objectPrx->ice_batchOneway();
				}
					
		
				//订阅
				int bz = 0;
				while(bz++ < 3)
				{
					try
					{
						topic->subscribeAndGetPublisher(qos, objectPrx);
						blnValue = true;
						break;
						/*Ice::ObjectPrx pub = 
						if(pub != NULL)
						{
							blnValue = true;
						}
						else
						{
							blnValue = false;
						}*/
					}
					catch(const IceStorm::AlreadySubscribed &ex)
					{
						//blnValue = false;
					}
					catch(const IceStorm::BadQoS &ex)
					{
						blnValue = false;
					}
				}
			}
			else
			{
				blnValue = false;
			}
		}
		else
		{
			blnValue = false;
		}
	}
	catch(Ice::Exception &ex)
	{
		blnValue = false;
	}
	catch(...)
	{
		blnValue = false;
	}
	
	
	return blnValue;
}

//haofy add 2012-09-12
bool BaseIceStorm::Subscriber(Ice::CommunicatorPtr &communicatorPtr, Ice::ObjectPrx &objectPrx, std::string &strTopic, std::string &strDeliverModel, std::string &strReliability, std::string &strRetryCount,Ice::ObjectPrx &proxyPrx)
{
	bool blnValue = true;
	try
	{
		IceStorm::TopicPrx topic = GetTopicProxy(communicatorPtr, strTopic);
		if(topic != 0)
		{	
			//设置qos
			IceStorm::QoS qos;
			if(strDeliverModel == "twoway")
			{
				if(strReliability != "")
				{
					qos["reliability"] = "ordered";
				}
				if(strRetryCount != "")
				{
					qos["retryCount"] = strRetryCount;
				}
			}
		
		
			//设置订阅者传输方式，分为twoway,oneway,batch_oneway,双向传输，不用设置，默认为twoway
			if (strDeliverModel == "twoway" || strDeliverModel == "oneway" || strDeliverModel == "batch_oneway")
			{
				if(strDeliverModel == "oneway")
				{
					objectPrx = objectPrx->ice_oneway();
				}
				else if(strDeliverModel == "batch_oneway")
				{
					objectPrx = objectPrx->ice_batchOneway();
				}
					
		
				//订阅
				int bz = 0;
				while(bz++ < 3)
				{
					try
					{
						proxyPrx=topic->subscribeAndGetPublisher(qos, objectPrx);
						blnValue = true;
						break;
						/*Ice::ObjectPrx pub = 
						if(pub != NULL)
						{
							blnValue = true;
						}
						else
						{
							blnValue = false;
						}*/
					}
					catch(const IceStorm::AlreadySubscribed &ex)
					{
						//blnValue = false;
					}
					catch(const IceStorm::BadQoS &ex)
					{
						blnValue = false;
					}
				}
			}
			else
			{
				blnValue = false;
			}
		}
		else
		{
			blnValue = false;
		}
	}
	catch(Ice::Exception &ex)
	{
		blnValue = false;
	}
	catch(...)
	{
		blnValue = false;
	}
	
	
	return blnValue;
}

/** 
* icestorm取消订阅
*
* @return boolean:true表示成功，false表示失败
*/
bool BaseIceStorm::UnSubscriber(Ice::CommunicatorPtr &communicatorPtr, Ice::ObjectPrx &objectPrx, std::string &strTopic)
{
	bool blnValue = true;
	try
	{
		IceStorm::TopicPrx topic = GetTopicProxy(communicatorPtr, strTopic);
		if(topic != 0)
		{
			topic->unsubscribe(objectPrx);
		}
		else
		{
			blnValue = false;
		}
	}catch(const Ice::LocalException &ex)
	{
		blnValue = false;
	}
	catch(Ice::Exception &ex)
	{
		blnValue = false;
	}
	catch(...)
	{
		blnValue = false;
	}

	return blnValue;
}

/** 
* 得到icestorm发布者代理
*
*@param deliverModel:订阅模式，值为twoway,oneway,datagram,batch_oneway,batch_datagram
* @return Ice.ObjectPrx
*/
Ice::ObjectPrx BaseIceStorm::GetPublisher(Ice::CommunicatorPtr &communicatorPtr, std::string &strTopic, std::string &strDeliverModel)
{
	try
	{
		IceStorm::TopicPrx topic = GetTopicProxy(communicatorPtr, strTopic);
		if(topic != 0)
		{
			Ice::ObjectPrx objectPrx = topic->getPublisher();
			if(strDeliverModel.compare("oneway") == 0)
			{
				objectPrx = objectPrx->ice_oneway();
			}
			else if(strDeliverModel.compare("batch_oneway") == 0)
			{
				objectPrx = objectPrx->ice_batchOneway();
			}
	
			return objectPrx;
		}
	}
	catch(Ice::Exception &ex)
	{
	}
	catch(...)
	{
	
	}

	return NULL;
}

/** 
* 得到icestorm主题对象代理
*
* @return TopicPrx:主题对象代理
*/
IceStorm::TopicPrx BaseIceStorm::GetTopicProxy(Ice::CommunicatorPtr &communicatorPtr, std::string &strTopic)
{
	std::stringstream stream;
	stream << "server-icestorm/TopicManager:default -h " << m_iceStormIp << " -p " << m_iceStormPort;
	std::string proxyStr = stream.str();
	IceStorm::TopicManagerPrx manager = IceStorm::TopicManagerPrx::checkedCast(
		communicatorPtr->stringToProxy(proxyStr));
	if(manager == 0)
	{
		throw "得到 IceStorm::TopicManager 失败!";
		return NULL;
	}

	IceStorm::TopicPrx topic;
	try
	{
		topic = manager->retrieve(strTopic);
	}
	catch(const IceStorm::NoSuchTopic &ex)
	{
		try
		{
			topic = manager->create(strTopic);
		}
		catch(const IceStorm::TopicExists &ex)
		{
			return NULL;
		}
	}

	return topic;
}

/** 
* 配置IceStorm服务器的ip和port
*
* @return 
*/
void BaseIceStorm::SetIceStormIpAndPort( const std::string& ip, int port )
{
	m_iceStormIp = ip;
	m_iceStormPort = port;
}
