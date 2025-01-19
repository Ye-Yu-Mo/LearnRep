#include <iostream>
#include <vector>
#include <stdexcept>

class BitmapFileSystem {
public:
    BitmapFileSystem(size_t totalBlocks) : totalBlocks(totalBlocks), bitmap(totalBlocks, false) {}

    // 分配一个空闲块
    int allocateBlock() {
        for (size_t i = 0; i < totalBlocks; ++i) {
            if (!bitmap[i]) {  // 找到一个未被使用的块
                bitmap[i] = true;  // 分配该块
                return i;  // 返回块的索引
            }
        }
        throw std::runtime_error("No free blocks available");
    }

    // 释放一个已分配的块
    void freeBlock(int blockIndex) {
        if (blockIndex < 0 || blockIndex >= totalBlocks) {
            throw std::out_of_range("Invalid block index");
        }
        if (!bitmap[blockIndex]) {
            throw std::logic_error("Block is already free");
        }
        bitmap[blockIndex] = false;  // 释放该块
    }

    // 查找一个空闲块
    int findFreeBlock() const {
        for (size_t i = 0; i < totalBlocks; ++i) {
            if (!bitmap[i]) {
                return i;  // 找到空闲块
            }
        }
        return -1;  // 没有空闲块
    }

    // 打印位示图
    void printBitmap() const {
        for (bool bit : bitmap) {
            std::cout << (bit ? '1' : '0') << " ";
        }
        std::cout << std::endl;
    }

private:
    size_t totalBlocks;        // 总块数
    std::vector<bool> bitmap;  // 位图，表示每个块的状态
};

int main() {
    BitmapFileSystem fs(10);  // 创建一个有10个块的文件系统

    std::cout << "Initial Bitmap:" << std::endl;
    fs.printBitmap();  // 打印初始的位示图

    // 分配块
    int block1 = fs.allocateBlock();
    std::cout << "Allocated block: " << block1 << std::endl;
    fs.printBitmap();

    int block2 = fs.allocateBlock();
    std::cout << "Allocated block: " << block2 << std::endl;
    fs.printBitmap();

    // 释放块
    fs.freeBlock(block1);
    std::cout << "Freed block: " << block1 << std::endl;
    fs.printBitmap();

    // 查找空闲块
    int freeBlock = fs.findFreeBlock();
    std::cout << "First free block found at: " << freeBlock << std::endl;
    fs.printBitmap();

    return 0;
}
