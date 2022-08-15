//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H
#include "Pointerable.hpp"
#include <functional>
class impl_LocalTranslator;


struct imp_TranslatorDeleter
{
    constexpr imp_TranslatorDeleter() noexcept = default;
    void operator()(impl_LocalTranslator* p) const;
};

/// @brief 传输文件封装类
class LocalTranslator final :  public virtual None_Copyable,public virtual AutoAlivePointerable<impl_LocalTranslator,imp_TranslatorDeleter>
{
public:
    /// @brief 数据发送回调
    using callback_data_sent =
    std::function< bool(char* bf,
    size_t should, size_t
    sent)>;
    /// @brief 文件读取回调
    using callback_file_read =std:: function<void(int pks, size_t read)>;

    /// @brief 
    /// @param filePath 文件路径
    /// @param ip  发送ip
    /// @param port 发送端口
    LocalTranslator(const char *filePath, const char *ip, unsigned int port);
    ~LocalTranslator();
    /// @brief 连接socket
    /// @return 
    bool Connect();
    /// @brief 文件的总大小
    /// @return 
    size_t getTotalFileSize() const;
    /// @brief 已发送的数据长度
    /// @return 
    size_t getSent()const;
    /// @brief 当前发送的文件名
    /// @return 
    std::string getFileName() const;
    /// @brief 发送包失败时候的回调
    /// @param cb 
    void setOnSentFail(callback_data_sent cb);
    /// @brief 设置发送包成功时候的回调
    /// @param cb 
    void setOnSentSuccess(callback_data_sent cb);
    /// @brief 设置读取文件时的回调
	/// @param cb 
    void setOnFileRead(callback_file_read cb);

    /// @brief 运行
    /// @return 传输成功的总数据
    size_t runIt();
};



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
