/***********************************************************************************************************************
 * @author Hackman Lo
 * @file render_job.cpp
 * @description 
 * @created_at 2023-10-20
***********************************************************************************************************************/
#include <thread>
#include "render_job.h"

void render_job::run() {
    self_holder_ = shared_from_this();
    workder_ = std::make_shared<std::thread>([&]{

    });
}
