/***********************************************************************************************************************
 * @author Hackman Lo
 * @file agent_connection.cpp
 * @description 
 * @created_at 2023-10-18
***********************************************************************************************************************/

#include "../../include/agent_connection.h"

agent_connection::agent_connection(boost::asio::ip::tcp::socket &socket) : socket_base(socket) {

}

agent_connection::agent_connection(boost::asio::io_context &ios) : socket_base(ios) {

}

void agent_connection::send_package(package_base &pkg) {
    outstream_.sputn(pkg.data(), pkg.size());
    send();
}

void agent_connection::e_connected() {

}

void agent_connection::e_data_sent(std::size_t bytes_transferred) {

}

bool agent_connection::e_data_received(std::size_t bytes_transferred) {
    while(true){
        if(!header_ && instream_.size() < sizeof(package_header)) return true;
        if(header_ && instream_.size() < header_->data_length_) return true;

        if(!header_){
            header_ = std::make_shared<package_header>();
            instream_.sgetn((char*)header_.get(), sizeof (package_header));
            header_->data_length_ = ntohl(header_->data_length_);
            header_->identifier_ = ntohl(header_->identifier_);
            header_->operation_ = ntohl(header_->operation_);
            if(header_->identifier_ != 0x19930323){
                std::cout << "Invalid Identifier, Shutting Down" << std::endl;
                return false;
            }
        }
        if(header_->data_length_ <= instream_.size()){
            if(event_map_.contains(header_->operation_)){
                event_map_[header_->operation_](std::dynamic_pointer_cast<agent_connection>(shared_from_this()), std::string_view(boost::asio::buffer_cast<const char*>(instream_.data()), header_->data_length_));
            }
            else
                std::cout << "Package Without Any Handler Registered:" << header_->operation_ << std::endl;
            instream_.consume(header_->data_length_);
            header_ = nullptr;
        }
    }
}

void agent_connection::on_package(std::uint32_t option, std::function<void(std::shared_ptr<agent_connection> connection, std::string_view)> callback) {
    event_map_.emplace(option, callback);
}
