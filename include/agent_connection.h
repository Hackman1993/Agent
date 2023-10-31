/***********************************************************************************************************************
 * @author Hackman Lo
 * @file agent_connection.h
 * @description 
 * @created_at 2023-10-18
***********************************************************************************************************************/

#ifndef AGENT_AGENT_CONNECTION_H
#define AGENT_AGENT_CONNECTION_H
#include <obelisk.h>
#include "packages/package.h"
class agent_connection : public obelisk::core::details::socket_base{
public:
    agent_connection(boost::asio::ip::tcp::socket &socket);
    agent_connection(boost::asio::io_context& ios);
    virtual void send_package(package_base& pkg);

    template <typename T>
    void send_package(std::uint32_t operation, T& message){
        package<T> pkg(operation, message);
        send_package(pkg);
    }

    void on_package(std::uint32_t option, std::function<void (std::shared_ptr<agent_connection> connection, std::string_view)>);

protected:
    void e_connected() override;

    void e_data_sent(std::size_t bytes_transferred) override;
    bool e_data_received(std::size_t bytes_transferred) override final;
    std::unordered_map<std::uint32_t, std::function<void (std::shared_ptr<agent_connection> connection, std::string_view)>> event_map_;
    std::shared_ptr<package_header> header_ = nullptr;
};


#endif //AGENT_AGENT_CONNECTION_H
