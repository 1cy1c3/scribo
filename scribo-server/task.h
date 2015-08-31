/**
 * @file task.h
 * @brief Represents the parallel execution unit
 */

#ifndef TASK_H
#define TASK_H

#include <QRunnable>
#include <QObject>
#include <QRunnable>
#include <QDebug>

/**
 * Represents the parallel execution unit
 *
 * @author Rune Krauss
 */
class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    /**
     * @brief Initializes the task object regarding threads
     */
    Task();

signals:
    /**
     * @brief Notifies the main thread when it is done
     * @param number Number of the task
     */
    void result(int number);

protected:
    /**
     * @brief Starts the task
     */
    void run();
};

#endif // TASK_H
