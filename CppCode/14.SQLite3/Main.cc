#include "Sqlite3.hpp"
#include <cassert>

int select_stu_callback(void *arg, int col_count, char **result, char **fields_name)
{
    std::vector<std::string> *array = (std::vector<std::string> *)arg;
    array->push_back(result[0]);
    return 0;
}
int main()
{
    SqliteHelper helper("./test.db");
    // 创建或打开数据库文件
    assert(helper.open());
    // 创建表
    const char *ct = "CREATE TABLE IF NOT EXISTS stu (sn INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(32), age INT);";
    assert(helper.exec(ct, nullptr, nullptr));
    // 数据操作语句
    // const char *insert_sql = "INSERT INTO stu (sn, name, age) VALUES (1, 'Morty', 16), (2, 'Summer', 18), (3, 'Rick', 58);";
    // assert(helper.exec(insert_sql, nullptr, nullptr));

    // const char *update_sql = "UPDATE stu SET name='S.Morty' WHERE sn=1;";
    // assert(helper.exec(update_sql, nullptr, nullptr));

    // const char *delete_sql = "DELETE FROM stu WHERE sn=2;";
    // assert(helper.exec(delete_sql, nullptr, nullptr));

    const char *select_sql = "SELECT name FROM stu;";
    std::vector<std::string> arr;
    assert(helper.exec(select_sql, &select_stu_callback, &arr));
    for (auto &name : arr)
    {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    // 关闭数据库
    helper.close();
    return 0;
}