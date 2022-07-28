#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_
#include <boost/asio.hpp>
namespace asio
{
    using namespace boost::asio;
    using boost::system::error_code;
}

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context &)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

struct File_info
{
    typedef unsigned long long Size_type;
    Size_type filesize;
    size_t filename_size;
    File_info() : filesize(0), filename_size(0) {}
};

#endif