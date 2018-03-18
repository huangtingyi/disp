//#include "stdafx.h"
#include "IoService.h"


IoService::IoService() : m_stop(false)
{
}

IoService::~IoService()
{
}

void IoService::Start()
{
    m_iosThread.reset(new tthread::thread(&IosProc, this));
}

void IoService::Run()
{
    while (!m_stop)
    {
        std::deque<TaskPtr> tasks;
        {
            tthread::lock_guard<tthread::mutex> guard(m_mutex);

            if (m_tasks.empty())
            {
                m_cv.wait(m_mutex);
				if ( m_stop )
				{
					break;
				}
            }
            if (m_tasks.empty())
            {
                continue;
            }
            tasks = std::move(m_tasks);
        }

        for (auto it = tasks.begin(); it != tasks.end()&&!m_stop; ++it)
        {
            (*(*it))();
        }
    }
}

void IoService::Stop()
{
    m_stop = true;
    m_cv.notify_all();

    if ( m_iosThread.get() == 0 )
    {
        return ;
    }

    m_iosThread->join();
}

void IoService::Post(TaskPtr& task)
{
    bool empty = true;
    {
        tthread::lock_guard<tthread::mutex> guard(m_mutex);
        empty = m_tasks.empty();
        m_tasks.push_back(std::move(task));
    }
    if (empty)
    {
        m_cv.notify_all();
    }
}

void IoService::IosProc(void* arg)
{
    IoService* ios = (IoService*)arg;
    ios->Run();
}