/***********************************************************************************************************************
 * @author Hackman Lo
 * @file render_job.h
 * @description 
 * @created_at 2023-10-20
***********************************************************************************************************************/

#ifndef AGENT_RENDER_JOB_H
#define AGENT_RENDER_JOB_H

#include "job_base.h"
#include <mutex>
class render_job : public std::enable_shared_from_this<render_job>{
public:
    void run();
protected:
    std::shared_ptr<render_job> self_holder_ = nullptr;
    std::shared_ptr<std::thread> workder_ = nullptr;
    static std::mutex running_;
};


#endif //AGENT_RENDER_JOB_H
