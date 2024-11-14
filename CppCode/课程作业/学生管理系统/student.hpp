#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <algorithm>
#include <iomanip>

class Student
{
public:
    using ptr = std::shared_ptr<Student>;
    Student() {}
    Student(const std::string &name,
            const std::string &id,
            const std::string &gender,
            const size_t &score,
            const size_t &age)
        : _name(name), _id(id), _gender(gender), _score(score), _age(age) {}
    const std::string &getName() const { return _name; }
    const std::string &getId() const { return _id; }
    const std::string &getGender() const { return _gender; }
    size_t getScore() const { return _score; }
    size_t getAge() const { return _age; }
    void setName(const std::string &name) { _name = name; }
    void setGender(const std::string &gender) { _gender = gender; }
    void setScore(const size_t &score) { _score = score; }
    void setAge(const size_t &age) { _age = age; }
    virtual double getGPA() = 0;
    virtual void setGPA(double gpa) = 0;

protected:
    std::string _name;   ///< 学生姓名
    std::string _id;     ///< 学生 ID
    std::string _gender; ///< 学生性别
    size_t _score;       ///< 学生成绩
    size_t _age;         ///< 学生年龄
};

class MyStudent : public Student
{
public:
    MyStudent(const std::string &name,
              const std::string &id,
              const std::string &gender,
              const size_t &score,
              const size_t &age,
              const double &gpa)
        : Student(name, id, gender, score, age), _gpa(gpa) {}
    double getGPA() { return _gpa; }
    void setGPA(double gpa) { _gpa = gpa; }

protected:
    double _gpa;
};

class StudentManager
{
public:
    using ptr = std::shared_ptr<StudentManager>;
    /**
     * @brief 创建新学生并添加到学生列表
     * @param name 学生姓名
     * @param id 学生 ID
     * @param gender 学生性别
     * @param score 学生成绩
     * @param age 学生年龄
     * @return true 如果学生添加成功; false 如果添加失败
     */
    bool newStudent(const std::string &name,
                    const std::string &id,
                    const std::string &gender,
                    const size_t &score,
                    const size_t &age,
                    const double &gpa)
    {
        auto student = std::make_shared<MyStudent>(name, id, gender, score, age, gpa);
        _students.push_back(student);
        return true; // 假设总是成功
    }
    /// @brief 展示学生列表
    void showStudentList()
    {
        // 打印表头
        std::cout << std::left << std::setw(20) << "Name"
                  << std::setw(15) << "ID"
                  << std::setw(10) << "Gender"
                  << std::setw(10) << "Score"
                  << std::setw(10) << "GPA"
                  << std::setw(5) << "Age" << std::endl;

        // 打印分隔线
        std::cout << std::string(80, '-') << std::endl;

        // 打印学生信息
        for (const auto &student : _students)
        {
            std::cout << std::left << std::setw(20) << student->getName()
                      << std::setw(15) << student->getId()
                      << std::setw(10) << student->getGender()
                      << std::setw(10) << student->getScore()
                      << std::setw(10) << student->getGPA()
                      << std::setw(5) << student->getAge() << std::endl;
        }
    }
    /**
     * @brief 删除指定 ID 的学生
     * @param id 学生 ID
     * @return true 如果删除成功; false 如果未找到该学生
     */
    bool deleteStudent(const std::string &id)
    {
        auto it = std::remove_if(_students.begin(), _students.end(),
                                 [&id](const Student::ptr &student)
                                 {
                                     return student->getId() == id;
                                 });
        if (it != _students.end())
        {
            _students.erase(it, _students.end());
            return true;
        }
        return false;
    }
    /**
     * @brief 修改指定 ID 的学生信息
     * @param id 学生 ID
     * @param name 新学生姓名
     * @param gender 新学生性别
     * @param score 新学生成绩
     * @param age 新学生年龄
     * @return true 如果修改成功; false 如果未找到该学生
     */
    bool modifyStudent(const std::string &id,
                       const std::string &name,
                       const std::string &gender,
                       const size_t &score,
                       const size_t &age,
                       const double &gpa)
    {
        for (auto &student : _students)
        {
            if (student->getId() == id)
            {
                student->setName(name);
                student->setGender(gender);
                student->setScore(score);
                student->setAge(age);
                student->setGPA(gpa);
                return true;
            }
        }
        return false;
    }
    /// @brief 统计并展示学生人数和平均分
    void statStudent()
    {
        size_t totalStudents = _students.size();
        size_t totalScore = std::accumulate(_students.begin(), _students.end(), 0,
                                            [](size_t sum, const Student::ptr &student)
                                            {
                                                return sum + student->getScore();
                                            });
        double averageScore = totalStudents > 0 ? static_cast<double>(totalScore) / totalStudents : 0;

        std::cout << "Total Students: " << totalStudents
                  << ", Average Score: " << averageScore << std::endl;
    }

    /// @brief 对链表按照GPA排序并输出
    void sortAndShow()
    {
        _students.sort(compareByGPA);
        showStudentList();
    }

private:
    static bool compareByGPA(const Student::ptr &s1, const Student::ptr &s2)
    {
        return (s1->getGPA()) > (s2->getGPA());
    }

private:
    std::list<Student::ptr> _students;
};