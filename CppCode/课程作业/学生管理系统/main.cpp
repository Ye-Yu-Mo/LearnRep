#include "student.hpp"
#include <cstdlib>
void showMenu()
{
    std::cout << "欢迎使用学生管理系统" << std::endl;
    std::cout << "1. 添加新学生" << std::endl;
    std::cout << "2. 显示学生列表" << std::endl;
    std::cout << "3. 删除学生" << std::endl;
    std::cout << "4. 修改学生信息" << std::endl;
    std::cout << "5. 统计学生人数和平均分" << std::endl;
    std::cout << "6. 按照GPA排序并展示列表" << std::endl;
    std::cout << "0. 退出" << std::endl;
}
int main()
{
    StudentManager::ptr manager = std::make_shared<StudentManager>();
    int choice = -1;

    while (true)
    {
        showMenu();
        std::cout << "请输入您的选择: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            std::string name, id, gender;
            size_t score, age;
            double gpa;
            std::cout << "请输入学生姓名: ";
            std::cin >> name;
            std::cout << "请输入学生 ID: ";
            std::cin >> id;
            std::cout << "请输入学生性别: ";
            std::cin >> gender;
            std::cout << "请输入学生成绩: ";
            std::cin >> score;
            std::cout << "请输入学生年龄: ";
            std::cin >> age;
            std::cout << "请输入学生GPA: ";
            std::cin >> gpa;
            manager->newStudent(name, id, gender, score, age, gpa);
            break;
        }
        case 2:
            manager->showStudentList();
            break;
        case 3:
        {
            std::string id;
            std::cout << "请输入要删除的学生 ID: ";
            std::cin >> id;
            if (manager->deleteStudent(id))
            {
                std::cout << "删除成功！" << std::endl;
            }
            else
            {
                std::cout << "未找到该学生！" << std::endl;
            }
            break;
        }
        case 4:
        {
            std::string id, name, gender;
            size_t score, age;
            double gpa;
            std::cout << "请输入要修改的学生 ID: ";
            std::cin >> id;
            std::cout << "请输入新学生姓名: ";
            std::cin >> name;
            std::cout << "请输入新学生性别: ";
            std::cin >> gender;
            std::cout << "请输入新学生成绩: ";
            std::cin >> score;
            std::cout << "请输入新学生年龄: ";
            std::cin >> age;
            std::cout << "请输入新学生GPA: ";
            std::cin >> gpa;
            if (manager->modifyStudent(id, name, gender, score, age, gpa))
            {
                std::cout << "修改成功！" << std::endl;
            }
            else
            {
                std::cout << "未找到该学生！" << std::endl;
            }
            break;
        }
        case 5:
            manager->statStudent();
            break;
        case 6:
            manager->sortAndShow();
            break;
        case 0:
            std::cout << "退出系统" << std::endl;
            return 0;
        default:
            std::cout << "无效的选择，请重新输入！" << std::endl;
        }
    }

    return 0;
}