
#include <psd/document.h>
#include <CLI/CLI.hpp>
#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool {
public:
  explicit ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
    for (size_t index = 0;
                index < num_threads;
                index++) {
      threads_.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            cv_.wait(lock, [this] {
              return !tasks_.empty() || stop_;
            });
            if (stop_ && tasks_.empty()) {
              return;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
          }
          task();
        }
      });
    }
  }
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      stop_ = true;
    }
    cv_.notify_all();

    for (auto& thread : threads_) {
      thread.join();
    }
  }
  void Enqueue(std::function<void()> task) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      tasks_.emplace(std::move(task));
    }
    cv_.notify_one();
  }
private:
  std::vector<std::thread> threads_;
  std::queue<std::function<void()> > tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;

  bool stop_ = false;
};
class Img2PSD {
public:

  static void SetupCLI(CLI::App &app) {
    app.add_option("dir-1", dir_1_)->required();
    app.add_option("dir-2", dir_2_)->required();
    app.add_option("--output", output_)->default_val("output");
  }

  static void Execute(CLI::App &app) {

    std::filesystem::create_directory(output_);
    indicators::show_console_cursor(false);

    auto bar = CreateProgressBar();

    ThreadPool thread_pool;

    for (auto index = 0u;
              index < GetImageCount();
              index++) {
      auto image_1 = GetImage1(index);
      auto image_2 = GetImage2(index);

      thread_pool.Enqueue([=, &bar] {
        PSD::Document<> document;

        document.Push(PSD::Layer<>("Background") .SetImage(Image::Decode(image_1)));
        document.Push(PSD::Layer<>("Layer 1")    .SetImage(Image::Decode(image_2)));

        document.Save(GetOutputPath(image_1, index));

        bar.tick();
      });
    }
  }
private:
  static inline std::string dir_1_;
  static inline std::string dir_2_;
  static inline std::string output_;

  static indicators::ProgressBar CreateProgressBar() {
    using namespace indicators;
    return ProgressBar(
      option::BarWidth(50),
      option::Start("("),
      option::Fill("•"),
      option::Lead("·"),
      option::Remainder(" "),
      option::End(")"),
      option::PrefixText("Creating documents... "),
      option::ShowElapsedTime(true),
      option::ShowRemainingTime(true),
      option::MaxProgress(GetImageCount())
    );
  }
  static auto GetImage(const std::string &dir, std::uint64_t index) {
    auto current = 0u;
    for (decltype(auto) entry : std::filesystem::directory_iterator(dir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".png") {

        if (current++ == index) {
          return entry.path();
        }
      }
    }
    throw std::runtime_error("no image found");
  }
  static std::filesystem::path GetImage1(std::uint64_t index) {
    return GetImage(dir_1_, index);
  }
  static std::filesystem::path GetImage2(std::uint64_t index) {
    return GetImage(dir_2_, index);
  }

  static std::filesystem::path GetOutputPath(std::string base_path, std::uint64_t index) {
    auto file_name = std::filesystem::path(base_path).filename();
    file_name.replace_extension(".psd");

    return output_ / file_name;
  }
  static auto GetImageCount(const std::string &dir) {
    auto count = 0u;

    for (decltype(auto) entry : std::filesystem::directory_iterator(dir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".png") {
        count++;
      }
    }
    if (!count) {
      throw std::runtime_error("no image found");
    }
    return count;
  }
  static std::uint64_t GetImageCount() {
    auto   count  = GetImageCount(dir_1_);
    return count == GetImageCount(dir_2_) 
      ? count 
      : throw std::runtime_error("directory count mismatch");
  }
};

int main(int argc, char **argv) {
  CLI::App app;

  Img2PSD::SetupCLI(app);
  CLI11_PARSE(app, argc, argv); 

  Img2PSD::Execute(app);

  return 0;
}
