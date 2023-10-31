/***********************************************************************************************************************
 * @author Hackman Lo
 * @file sync_job.h
 * @description 
 * @created_at 2023-10-20
***********************************************************************************************************************/

#ifndef AGENT_SYNC_JOB_H
#define AGENT_SYNC_JOB_H
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include "packages/protos/rendering_job.pb.h"

class sync_job : public std::enable_shared_from_this<sync_job>{
public:
    using sync_job_callback = std::function<void(agent::package::renderer::render_project&, bool)>;
    sync_job(agent::package::renderer::render_project file, sync_job_callback callback);

    void run();
private:
    sync_job_callback callback_;
    std::shared_ptr<sync_job> self_holder_ = nullptr;
    agent::package::renderer::render_project file_;
    std::atomic_uint32_t remains_;
    std::atomic_uint32_t running_count_;
    std::shared_ptr<std::thread> working_thread_;

    std::mutex notify_mutex_;
    std::unique_lock<std::mutex> notify_lock_;
    std::condition_variable notify_;

};


#endif //AGENT_SYNC_JOB_H
