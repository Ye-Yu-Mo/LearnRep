#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Function prototypes
void fcfs(const vector<int> &requests, int head);                 // 先来先服务（FCFS）
void sstf(const vector<int> &requests, int head);                 // 最短寻道时间优先（SSTF）
void scan(const vector<int> &requests, int head, int disk_size);  // 扫描算法（SCAN）
void cscan(const vector<int> &requests, int head, int disk_size); // 循环扫描算法（C-SCAN）

int main()
{
    // 测试数据
    int disk_size = 200;                                        // 磁盘的总范围（最大柱面号）
    int head = 50;                                              // 磁头的初始位置
    vector<int> requests = {95, 180, 34, 119, 11, 123, 62, 64}; // 磁盘请求序列

    // 打印测试数据
    cout << "--- 磁盘调度算法 ---\n";
    cout << "磁盘大小: " << disk_size << "\n";
    cout << "初始磁头位置: " << head << "\n";
    cout << "磁盘请求序列: ";
    for (int req : requests)
    {
        cout << req << " ";
    }
    cout << "\n\n";

    // 运行各个算法并输出结果
    cout << "1. 先来先服务 (FCFS):\n";
    fcfs(requests, head);

    cout << "\n2. 最短寻道时间优先 (SSTF):\n";
    sstf(requests, head);

    cout << "\n3. 扫描算法 (SCAN):\n";
    scan(requests, head, disk_size);

    cout << "\n4. 循环扫描算法 (C-SCAN):\n";
    cscan(requests, head, disk_size);

    return 0;
}

// 先来先服务（FCFS）算法
void fcfs(const vector<int> &requests, int head)
{
    int seek_time = 0;       // 总寻道时间
    int current_head = head; // 当前磁头位置

    cout << "服务顺序: ";
    for (int req : requests)
    {
        cout << req << " ";
        seek_time += abs(current_head - req); // 计算磁头移动的距离
        current_head = req;                   // 更新磁头位置
    }
    cout << "\n总寻道时间: " << seek_time << " 单位" << endl;
}

// 最短寻道时间优先（SSTF）算法
void sstf(const vector<int> &requests, int head)
{
    vector<int> remaining_requests = requests; // 剩余的请求序列
    int seek_time = 0;                         // 总寻道时间
    int current_head = head;                   // 当前磁头位置

    cout << "服务顺序: ";
    while (!remaining_requests.empty())
    {
        // 找到离当前磁头位置最近的请求
        auto closest = min_element(remaining_requests.begin(), remaining_requests.end(),
                                   [current_head](int a, int b)
                                   {
                                       return abs(a - current_head) < abs(b - current_head);
                                   });
        cout << *closest << " ";
        seek_time += abs(current_head - *closest); // 计算磁头移动的距离
        current_head = *closest;                   // 更新磁头位置
        remaining_requests.erase(closest);         // 从序列中移除已处理的请求
    }
    cout << "\n总寻道时间: " << seek_time << " 单位" << endl;
}

// 扫描算法（SCAN）
void scan(const vector<int> &requests, int head, int disk_size)
{
    vector<int> left, right; // 左侧（小于当前磁头位置）和右侧（大于等于当前磁头位置）的请求
    int seek_time = 0;       // 总寻道时间
    int current_head = head; // 当前磁头位置

    // 按方向分组请求
    for (int req : requests)
    {
        if (req < head)
            left.push_back(req);
        else
            right.push_back(req);
    }

    // 按顺序排列
    sort(left.begin(), left.end(), greater<int>()); // 左侧按降序排列
    sort(right.begin(), right.end());               // 右侧按升序排列

    cout << "服务顺序: ";
    // 先处理右侧的请求
    for (int req : right)
    {
        cout << req << " ";
        seek_time += abs(current_head - req);
        current_head = req;
    }
    // 再处理左侧的请求
    for (int req : left)
    {
        cout << req << " ";
        seek_time += abs(current_head - req);
        current_head = req;
    }

    cout << "\n总寻道时间: " << seek_time << " 单位" << endl;
}

// 循环扫描算法（C-SCAN）
void cscan(const vector<int> &requests, int head, int disk_size)
{
    vector<int> left, right; // 左侧和右侧的请求
    int seek_time = 0;       // 总寻道时间
    int current_head = head; // 当前磁头位置

    // 按方向分组请求
    for (int req : requests)
    {
        if (req < head)
            left.push_back(req);
        else
            right.push_back(req);
    }

    // 按顺序排列
    sort(left.begin(), left.end());   // 左侧按升序排列
    sort(right.begin(), right.end()); // 右侧按升序排列

    cout << "服务顺序: ";
    // 先处理右侧的请求
    for (int req : right)
    {
        cout << req << " ";
        seek_time += abs(current_head - req);
        current_head = req;
    }
    // 到达磁盘尾部后跳转到磁盘头部
    if (!left.empty())
    {
        seek_time += abs(current_head - (disk_size - 1)); // 移动到磁盘尾部
        current_head = 0;                                 // 跳转到磁盘头部
        cout << "0 (环绕) ";
        for (int req : left)
        {
            cout << req << " ";
            seek_time += abs(current_head - req);
            current_head = req;
        }
    }

    cout << "\n总寻道时间: " << seek_time << " 单位" << endl;
}
