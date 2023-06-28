/**
 * @brief A pool of threads waiting to execute tasks.
 *
 * For performance reasons, the batch of 30-min Quotes can be
 * processed by thread workers concurrently in a thread pool.
 */
#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace fx {

/**
 * @brief As the queue of Jobs starts filling up, threads work
 *  on concurrently finishing up the Jobs.
 *
 */
class ThreadPool {
  public:
    /**
     * @brief Workers (Consumers) monitor the job-queue or the shutdown directive
     *  before proceeding towards either dequeing jobs and running them
     *  or closing shop.
     *
     * @param numThreads Count of workers for tasks.
     */
    ThreadPool(std::size_t numThreads) {
        for (auto i = 0u; i < numThreads; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    Job job;
                    {
                        std::unique_lock lk(jobMutex_);
                        cv_.wait(lk, [this]() {
                            return shutdown_.load() || ! jobQueue_.empty();
                        });

                        if (shutdown_.load() &&
                                jobQueue_.empty()) {
                                    break;
                        }

                        job = std::move(jobQueue_.front());
                        jobQueue_.pop();
                    }
                    job();
                }
            });
        }
    }
    void Shutdown() {
        shutdown_.store(true);
        cv_.notify_all();

        for (auto& worker : workers_) {
            worker.join();
        }
    }
    /**
     * @brief The Producer routine. Jobs submitted by the quote-parser
     *  are placed on the job-queue and a Consumer thread is notified each time
     *  a job is enqueued.
     */
    template <class F, typename... Args>
    auto Submit(F&& f, Args&&... args) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto sp = std::make_shared<Job>(func);
        {
            std::unique_lock lk(jobMutex_);
            jobQueue_.emplace([sp]() {
                (*sp)();
            });
        }
        cv_.notify_one();
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
  private:
    std::vector<std::thread> workers_;
    using Job = std::function<void()>;
    std::queue<Job> jobQueue_;
    std::mutex jobMutex_;
    std::condition_variable cv_;
    std::atomic_bool shutdown_{false};
};

}   // namespace fx
#endif
