#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Mutex and condition variable for synchronization
std::mutex mtx;
std::condition_variable cv;
bool kernel_done = false;

// Simulate kernel-mode functions
void kernelFunction1() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate work
    std::cout << "Kernel Function 1 completed." << std::endl;
}

void kernelFunction2() {
    std::this_thread::sleep_for(std::chrono::seconds(3));  // Simulate work
    std::cout << "Kernel Function 2 completed." << std::endl;
}

// Function to be called by user-mode functions, ensuring kernel functions are done
void waitForKernelFunctions() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return kernel_done; });
}

// User-mode functions
void userFunction1() {
    std::cout << "User Function 1 started." << std::endl;
    waitForKernelFunctions();  // Wait for kernel functions to complete
    std::cout << "User Function 1 executed after kernel functions." << std::endl;
}

void userFunction2() {
    std::cout << "User Function 2 started." << std::endl;
    // This function can execute other code while waiting for kernel functions to complete
    std::cout << "User Function 2 executed and can continue running." << std::endl;
}

int main() {
    // Launch kernel-mode functions in separate threads
    std::thread kernelThread1(kernelFunction1);
    std::thread kernelThread2(kernelFunction2);

    // Launch user-mode functions in separate threads
    std::thread userThread1(userFunction1);
    std::thread userThread2(userFunction2);

    // Wait for kernel functions to complete
    kernelThread1.join();
    kernelThread2.join();

    // Notify user functions that kernel functions are done
    {
        std::lock_guard<std::mutex> lock(mtx);
        kernel_done = true;
    }
    cv.notify_all();

    // Wait for user functions to complete
    userThread1.join();
    userThread2.join();

    return 0;
}
