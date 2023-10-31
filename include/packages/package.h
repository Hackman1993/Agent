/***********************************************************************************************************************
 * @author Hackman Lo
 * @file package.h
 * @description 
 * @created_at 2023-10-17
***********************************************************************************************************************/

#ifndef OBELISK_PACKAGE_H
#define OBELISK_PACKAGE_H
#include <boost/asio.hpp>
#define PACKAGE_NEW_RENDER_PROJECT 0
#define PACKAGE_AGENT_REGISTER 0x02
#define PACKAGE_AGENT_DUPLICATED_ID 0x03
#define PACKAGE_DISPATCHABLE_UPDATED 0x04

#define PACKAGE_RENDER_JOB_DISPATCH 0x1001


struct package_header {
    std::uint32_t operation_ = 0;
    std::uint32_t identifier_ = htonl(0x19930323);
    std::uint32_t data_length_ = 0;
};
class package_base{
public:
    virtual ~package_base() = default;
    const char* data(){ return boost::asio::buffer_cast<const char*>(data_.data()); };
    std::size_t size(){ return data_.size(); };
protected:
    boost::asio::streambuf data_;
};

template <class T>
class package : public package_base{
public:

    package(package_header header){
        header.data_length_ = 0;
        header.operation_ = htonl(header.operation_);
        data_.sputn(reinterpret_cast<const char *>(&header), sizeof(package_header));
    }
    package(std::uint32_t operation){
        package_header header;
        header.data_length_ = 0;
        header.operation_ = htonl(operation);
        data_.sputn(reinterpret_cast<const char *>(&header), sizeof(package_header));
    }

    package(std::uint32_t operation, T& message) : package(package_header{operation},message) {}
    package(package_header header, T& message) {
        auto data_length = message.ByteSizeLong();
        header.data_length_ =htonl(data_length);
        header.operation_ = htonl(header.operation_);

        data_.prepare(data_length + sizeof(package_header));
        data_.sputn(reinterpret_cast<const char *>(&header), sizeof(package_header));

        auto *data_ptr = boost::asio::buffer_cast<const unsigned char*>(data_.data());
        message.SerializeToArray((void*)(&data_ptr[sizeof(package_header)]), data_length);
        data_.commit(data_length);
    }
};


#endif //OBELISK_PACKAGE_H
