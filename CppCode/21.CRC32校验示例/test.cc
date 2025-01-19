#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <stdexcept>

using namespace std;

inline double str_to_double(const string &str)
{
    if (str.empty())
        return 0.0;
    try
    {
        return std::stod(str);
    }
    catch (...)
    {
        throw std::invalid_argument("Invalid string to double conversion: " + str);
    }
}

// 用来维护校验和的订单簿
class ChecksumOrderbook
{
public:
    ChecksumOrderbook(size_t depth = 10)
        : _depth(depth)
    {
        generate_table();
    }

    uint32_t getChecksum()
    {
        std::string asks_str;
        cout << "Ask :" << endl;
        for (const auto &ask : asks)
        {
            cout << ask[0] << " " << ask[1] << endl;
            asks_str += format_order(ask);
        }

        std::string bids_str;
        cout << "Bid :" << endl;
        for (const auto &bid : bids)
        {
            cout << bid[0] << " " << bid[1] << endl;
            bids_str += format_order(bid);
        }

        std::string combined_str = asks_str + bids_str;

        // 调试输出
        std::cout << "Combined string: " << combined_str << std::endl;

        // 计算CRC32校验和
        return update(0, combined_str.data(), combined_str.size());
    }

    void addAsk(double price, double qty)
    {
        auto it = asks.begin();
        while (it != asks.end() && str_to_double((*it)[0]) < price)
        {
            ++it;
        }
        asks.insert(it, {format_price(price), format_qty(qty)});
        if (asks.size() > _depth)
        {
            asks.resize(_depth);
        }
    }

    void addBid(double price, double qty)
    {
        auto it = bids.begin();
        while (it != bids.end() && str_to_double((*it)[0]) > price)
        {
            ++it;
        }
        bids.insert(it, {format_price(price), format_qty(qty)});
        if (bids.size() > _depth)
        {
            bids.resize(_depth);
        }
    }

    void clear()
    {
        asks.clear();
        bids.clear();
    }

private:
    void generate_table()
    {
        uint32_t polynomial = 0xEDB88320;
        for (uint32_t i = 0; i < 256; i++)
        {
            uint32_t c = i;
            for (size_t j = 0; j < 8; j++)
            {
                if (c & 1)
                {
                    c = polynomial ^ (c >> 1);
                }
                else
                {
                    c >>= 1;
                }
            }
            table[i] = c;
        }
    }

    uint32_t update(uint32_t initial, const void *buf, size_t len)
    {
        uint32_t c = initial ^ 0xFFFFFFFF;
        const uint8_t *u = static_cast<const uint8_t *>(buf);
        for (size_t i = 0; i < len; ++i)
        {
            c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
        }
        return c ^ 0xFFFFFFFF;
    }

    std::string format_order(const vector<string> &order)
    {
        std::string price = order[0];
        std::string qty = order[1];

        // 删除小数点并去除前导零
        price.erase(price.find('.'), 1);
        price.erase(0, price.find_first_not_of('0'));
        qty.erase(qty.find('.'), 1);
        qty.erase(0, qty.find_first_not_of('0'));

        return price + qty;
    }

    std::string format_price(double price)
    {
        std::ostringstream ss_price;
        ss_price << std::fixed << std::setprecision(1) << price;
        // ss_price << price;
        return ss_price.str();
    }

    std::string format_qty(double qty)
    {
        std::ostringstream ss_qty;
        ss_qty << std::fixed << std::setprecision(8) << qty;
        // ss_qty << qty;
        return ss_qty.str();
    }

private:
    uint32_t table[256];
    size_t _depth;
    std::vector<vector<string>> asks;
    std::vector<vector<string>> bids;
};

int main()
{
    // 示例订单簿数据
    std::vector<vector<double>> asks = {
        {3410.97, 0.0479839},
        {3410.98, 29.3831},
        {3411.08, 1.579},
        {3411.16, 14.6908},
        {3411.38, 29.3797},
        {3411.45, 0.25},
        {3411.47, 0.148416},
        {3411.59, 0.9365},
        {3411.92, 1.17294},
        {3411.94, 0.9365}};

    std::vector<vector<double>> bids = {
        {3410.76, 0.16379},
        {3410.75, 0.72913},
        {3410.68, 29.3856},
        {3410.51, 14.6935},
        {3410.23, 1.1865},
        {3410.19, 0.143905},
        {3410.18, 29.3899},
        {3410.05, 0.846272},
        {3410.03, 1.17294},
        {3409.88, 0.9365}};

    ChecksumOrderbook ob;
    for (auto ask : asks)
    {
        ob.addAsk(ask[0], ask[1]);
    }
    for (auto bid : bids)
    {
        ob.addBid(bid[0], bid[1]);
    }

    // 计算CRC32校验和
    uint32_t checksum = ob.getChecksum();

    // 输出校验和
    std::cout << "Calculated checksum: " << checksum << std::endl;

    // 预期校验和（文档中的示例值）
    uint32_t expected_checksum = 2837312393;
    std::cout << "Expected checksum: " << expected_checksum << std::endl;

    // 检查校验和是否匹配
    if (checksum == expected_checksum)
    {
        std::cout << "Checksum matches!" << std::endl;
    }
    else
    {
        std::cout << "Checksum does not match!" << std::endl;
    }

    return 0;
}
