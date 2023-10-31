/***********************************************************************************************************************
 * @author Hackman Lo
 * @file sync_job.cpp
 * @description 
 * @created_at 2023-10-20
***********************************************************************************************************************/

#include "sync_job.h"
#include <thread>
#include <utility>
#include <filesystem>
#include <boost/process.hpp>
void sync_job::run() {
    self_holder_ = shared_from_this();
    remains_ = file_.files_size();
    working_thread_ = std::make_shared<std::thread>([&](){
        std::vector<std::shared_ptr<std::thread>> threads_;
        for(int i = 0; i < running_count_; ++i){
            if(threads_.size() >= 10){
                notify_.wait(notify_lock_, [&](){ return running_count_ < 10;});
            }
            running_count_ += 1;
            auto &file = file_.files(i);
            threads_.emplace_back(std::make_shared<std::thread>([&](){
                std::filesystem::path file_path("./temp_file/project/" + file_.project_id() + "/" + file.relative_path());
                auto parent_path = file_path.parent_path().string();
                std::filesystem::create_directories(file_path.parent_path());
                bool success = false;
                if(!std::filesystem::exists(file_path)){
                    int retry_count_ = 0;
                    while(retry_count_ < 3){
                        boost::process::child get_process("curl " + file.download_url() + " --output " + std::filesystem::absolute(file_path).string());
                        boost::system::error_code err;
                        get_process.wait(err);
                        if(err){
                            retry_count_++;
                            std::filesystem::remove(file_path);
                        }
                        if(get_process.exit_code() == 0)
                        {
                            success = true;
                            break;
                        }
                    }
                }
                running_count_ -= 1;
                remains_ -= success? 1:0;
                notify_.notify_one();
            }));
        }
        for(auto &thread : threads_){
            thread->join();
        }
        callback_(file_, remains_ == 0);
    });
}

sync_job::sync_job(agent::package::renderer::render_project file, sync_job::sync_job_callback callback): file_(std::move(file)), callback_(std::move(callback)), notify_lock_(notify_mutex_) {
}
