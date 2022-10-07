#ifndef SOCKET_CLIENT_ERROR_HPP
#define SOCKET_CLIENT_ERROR_HPP

#include <stdexcept>

#define _if_error(conditions,msg) if(conditions) throw std::runtime_error(msg);

#endif //SOCKET_CLIENT_ERROR_HPP
