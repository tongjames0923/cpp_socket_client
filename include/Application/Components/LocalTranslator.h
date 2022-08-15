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

/// @brief �����ļ���װ��
class LocalTranslator final :  public virtual None_Copyable,public virtual AutoAlivePointerable<impl_LocalTranslator,imp_TranslatorDeleter>
{
public:
    /// @brief ���ݷ��ͻص�
    using callback_data_sent =
    std::function< bool(char* bf,
    size_t should, size_t
    sent)>;
    /// @brief �ļ���ȡ�ص�
    using callback_file_read =std:: function<void(int pks, size_t read)>;

    /// @brief 
    /// @param filePath �ļ�·��
    /// @param ip  ����ip
    /// @param port ���Ͷ˿�
    LocalTranslator(const char *filePath, const char *ip, unsigned int port);
    ~LocalTranslator();
    /// @brief ����socket
    /// @return 
    bool Connect();
    /// @brief �ļ����ܴ�С
    /// @return 
    size_t getTotalFileSize() const;
    /// @brief �ѷ��͵����ݳ���
    /// @return 
    size_t getSent()const;
    /// @brief ��ǰ���͵��ļ���
    /// @return 
    std::string getFileName() const;
    /// @brief ���Ͱ�ʧ��ʱ��Ļص�
    /// @param cb 
    void setOnSentFail(callback_data_sent cb);
    /// @brief ���÷��Ͱ��ɹ�ʱ��Ļص�
    /// @param cb 
    void setOnSentSuccess(callback_data_sent cb);
    /// @brief ���ö�ȡ�ļ�ʱ�Ļص�
	/// @param cb 
    void setOnFileRead(callback_file_read cb);

    /// @brief ����
    /// @return ����ɹ���������
    size_t runIt();
};



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
