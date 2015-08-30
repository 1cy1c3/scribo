/** @file task.cpp
 * Starts parallel processing units.
 * The units saves its progress in the stack.
 */

#include "task.h"

/**
 * Constructor of the tasks, initializes them.
 */
Task::Task()
{
    qDebug() << "Task initialized...";
}

/**
 * When the thread pool kicks up, it's going to hit this run and it's going to do this time consuming task.
 * After it's done, sends the results back to the main thread.
 * This runs in a separate thread and there are no any control regarding this thread.
 * This may just stay in the queue for several ms depending on how busy the server is.
 */
void Task::run()
{
    // Time consumer

    qDebug() << "Task started...";

    int iNumber = 0;
    for(int i = 0; i < 100; i++)
    {
        iNumber += 1;
    }

    qDebug() << "Task done...";
    emit result(iNumber);
}
