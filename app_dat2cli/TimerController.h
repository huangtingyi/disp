#include <iostream>  
#include <time.h>  
#include <boost/asio.hpp>  
#include <boost/bind.hpp>    
#include <boost/function.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>    
  
using namespace std;  
  
//超时控制器类  
class TimerController   
{  
public:  
  
    /** 
     * 超时控制器构造函数 
     * @param ios 异步I/O对象 
     * @param callbackFunc 超时处理回调函数 
     * @param uiWaitSec 定时器间隔等待时间，单位：秒 
     */  
    explicit TimerController(boost::asio::io_service &ios, boost::function<void()> callbackFunc, unsigned int uiWaitSec) :  
            m_timer(ios, boost::posix_time::seconds(uiWaitSec))
    {  
        m_timeoutHandle = callbackFunc;  
        m_uiWaitSec = uiWaitSec;  
        m_timer.async_wait(boost::bind(&TimerController::onTime, this, boost::asio::placeholders::error));  
    }  
  
    /** 
     * 析构函数 
     */  
    ~TimerController()   
    {  
        m_timer.cancel();  
    }  
  
    /** 
     * 定时器响应函数 
     * @param error_code 定时器异常错误信息 
     */  
    void onTime(const boost::system::error_code& err)   
    {  
    	if(err)
        {
    		cout<<"canle:"<<errno<<endl;
            return;
        }
        m_timeoutHandle();  
        //m_timer.expires_at(m_timer.expires_at() + boost::posix_time::seconds(m_uiWaitSec));  
        m_timer.expires_from_now(boost::posix_time::seconds(m_uiWaitSec));
        m_timer.async_wait(boost::bind(&TimerController::onTime, this, boost::asio::placeholders::error));  
    }
    void resetTimer(unsigned int uiWaitSec)
    {
    	m_timer.cancel();
    	m_uiWaitSec = uiWaitSec;
			m_timer.expires_from_now(boost::posix_time::seconds(m_uiWaitSec));
			m_timer.async_wait(boost::bind(&TimerController::onTime, this, boost::asio::placeholders::error));   	
    }
  
private:  
    unsigned int                    m_uiWaitSec;                //定时间间隔等待时间  
    boost::asio::deadline_timer     m_timer;                    //asio定时器  
    boost::function<void()>         m_timeoutHandle;            //超时处理回调函数  
};  
