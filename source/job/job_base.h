/***********************************************************************************************************************
 * @author Hackman Lo
 * @file job_base.h
 * @description 
 * @created_at 2023-10-20
***********************************************************************************************************************/

#ifndef AGENT_JOB_BASE_H
#define AGENT_JOB_BASE_H
#include <string>
#include <functional>
#include <utility>
#include <thread>
#include <memory>
class job_base : public std::enable_shared_from_this<job_base>{

public:
    using job_callback_t = std::function<void (const std::string, std::uint32_t)>;
    virtual ~job_base() = default;
    virtual void run() = 0;
protected:
    std::shared_ptr<job_base> self_holder_ = nullptr;
    job_base(std::string id, job_callback_t callback, bool unique = false) : id_(std::move(id)), callback_(std::move(callback)), is_unique_(unique) {}
    std::string id_;
    bool is_unique_;
    std::shared_ptr<std::thread> worker_;
    job_callback_t callback_;
};


#endif //AGENT_JOB_BASE_H
