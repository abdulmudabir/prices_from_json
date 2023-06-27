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

class ThreadPool {
  public:
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
        // submitted a Job, but how do you know that it's done?

        // do we need to get a future to this promise?
    }
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
