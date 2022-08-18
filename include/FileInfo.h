#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <string>
#include <functional>
#include "Pointerable.hpp"

class impl_fileinfo;

struct File_info;

/**
 * @brief 文件读取回调
 * @param  int =当前包序号
 * @param  size_t = 本次读取字节数 
 * @param size_t =总共读取字节数
 * @return bool 读取是否继续
 */
using fileReadCallback = std::function<bool(int packindex, size_t perRead, size_t totalread)>;

/**
 * 文件信息读取类
 */
class FileInfo final : private virtual Pointerable<impl_fileinfo>, public virtual None_Copyable
{
public:
    /**
     *
     * @param path 文件路径
     */
    explicit FileInfo(const std::string &path) noexcept;

    ~FileInfo();


    /**
     *
     * @return 返回文件尺寸
     */
    size_t getFileSize() const noexcept;

    /**
     *
     * @return 返回文件名
     */
    std::string getFileName() const noexcept;

    /**
     * 读取文件内容
     * @param buffer 读取到的缓存区
     * @param bufferSize 缓存区尺寸
     * @param callback 读取回调
     * @return 是否读取成功
     */
    bool readFile(char *buffer, size_t bufferSize, fileReadCallback callback);
};

#endif