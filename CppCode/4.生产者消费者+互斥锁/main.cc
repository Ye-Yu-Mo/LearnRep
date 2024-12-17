#include "BlockQueue.hpp"
#include "Task.hpp"
#include <ctime>
#include <cstdint>

class ThreadData
{
public:
    BlockQueue<Task> *bq;
    std::string name;
};

void *consumer(void *args)
{
    ThreadData *td = (ThreadData *)args;
    while (true)
    {
        Task task;
        td->bq->Pop(&task);
        task();
        std::cout << td->name << " " << task.PrintResult();
    }
    return nullptr;
}

void *producter(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        int data1 = rand() % 100;
        int data2 = rand() % 100;
        char op = operators[rand() % operators.size()];
        Task task(data1, data2, op);
        std::cout << "task: " << task.PrintTask() << std::endl;
        bq->Push(task);
        sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((uint16_t)time(nullptr) ^ getpid() ^ pthread_self());
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c[3], p[2]; // 三个消费者，两个生产者

    ThreadData *td0 = new ThreadData();
    td0->bq = bq;
    td0->name = "[thread-1]";
    pthread_create(&c[0], nullptr, consumer, td0);

    ThreadData *td1 = new ThreadData();
    td1->bq = bq;
    td1->name = "[thread-2]";
    pthread_create(&c[1], nullptr, consumer, td1);

    ThreadData *td2 = new ThreadData();
    td2->bq = bq;
    td2->name = "[thread-3]";
    pthread_create(&c[2], nullptr, consumer, td2);

    pthread_create(&p[0], nullptr, producter, bq);
    pthread_create(&p[1], nullptr, producter, bq);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(c[2], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    return 0;
}