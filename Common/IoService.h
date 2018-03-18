//////////////////////////////////////////////////////////////////////////////
/// @file       IoService.h                                                      
/// @copyright  Copyright (C), 2014, 深圳市国泰安信息技术有限公司        
/// @author     何可丁                                                        
/// @brief      任务调度器                     
/// @version    v1.0.0                                                        
/// @date       2014-7-21                                                   
/// @note                                                                         
//////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include <memory>
#include <functional>
#include <deque>
#include "TinyThread.h"

typedef std::function<void (void)> Task;
typedef std::unique_ptr<Task> TaskPtr;

/**调用顺序：
   1.调用Start()接口；
   2.执行任务时，使用Post()接口；
   3.调用Stop()接口；
*/

/// 任务调度器
class IoService
{
public:
    /// 构造
    IoService();
    /// 析构
    virtual ~IoService();

    /// 启动
    void Start();

    /// 运行
    void Run();
    /// 停止
    void Stop();
    /// 投递任务
    /// @param task 任务
    void Post(TaskPtr& task);

private:
    static void IosProc(void* arg);

private:
    tthread::mutex              m_mutex; ///< 锁
    tthread::condition_variable m_cv;    ///< 条件变量
    bool                        m_stop;  ///< 停止标志
    std::deque<TaskPtr>         m_tasks; ///< 任务列表

    std::unique_ptr<tthread::thread>	m_iosThread;  ///< 执行任务线程
};

#endif // MAIN_THREAD_H