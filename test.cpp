#include "counter.hpp"
#include <cassert>
#include <thread>
#include <functional>


/* executes the inc() and then get() increments_number times
 * and checks that the inc() has increased the previous value
 * */
void run_and_test_inc_get(Counter *c, int *last_get_result, size_t increments_number) {
    int prev_get_result = 0;
    for (size_t i = 0; i < increments_number; i++) {
        inc(c);
        *last_get_result = get(c);
        assert(*last_get_result > prev_get_result);
    }
}


/* executes the run_and_test_inc_get with random parameters number_of_repeat times in two threads
 * and checks that the last result of get in at least one thread
 * is equal to the total number of increments
 * */
void test_sequential_inc_get(size_t number_of_repeat) {
    for (size_t i = 0; i < number_of_repeat; i++) {
        int last_get_result_thread1 = 0;
        int last_get_result_thread2 = 0;
        int inc_number1 = get_random_number(1, 15);
        int inc_number2 = get_random_number(1, 15);
        auto counter_capacity = get_random_number<size_t>(1, 5);

        Counter counter(counter_capacity);

        std::thread thread1(std::bind(run_and_test_inc_get, &counter, &last_get_result_thread1, inc_number1));
        std::thread thread2(std::bind(run_and_test_inc_get, &counter, &last_get_result_thread2, inc_number2));
        thread1.join();
        thread2.join();

        int sum_inc_number = inc_number1 + inc_number2;
        assert(last_get_result_thread1 == sum_inc_number || last_get_result_thread2 == sum_inc_number);
    }
}


void scenario1(Counter *c, std::vector<int> *get_results) {
    inc(c);
    get_results->push_back(get(c));
}

void scenario2(Counter *c, std::vector<int> *get_results) {
    get_results->push_back(get(c));
    inc(c);
    inc(c);
    get_results->push_back(get(c));
}


/* executes the scenario1 in thread1 and scenario2 in thread2 number_of_repeat times
 * checks the correctness of the results of operations get() in the first thread and the second thread
 * */
void test_scenario(size_t number_of_repeat) {
    std::vector<int> possible_operations_results1 = {1, 0, 3};
    std::vector<int> possible_operations_results2 = {1, 1, 3};
    std::vector<int> possible_operations_results3 = {2, 0, 3};
    std::vector<int> possible_operations_results4 = {2, 1, 3};
    std::vector<int> possible_operations_results5 = {3, 0, 2};
    std::vector<int> possible_operations_results6 = {3, 0, 3};
    std::vector<int> possible_operations_results7 = {3, 1, 3};

    for (size_t i = 0; i < number_of_repeat; i++) {
        std::vector<int> get_results_thread1;
        std::vector<int> get_results_thread2;
        auto counter_capacity = get_random_number<size_t>(1, 5);

        Counter counter(counter_capacity);

        std::thread thread1(std::bind(scenario1, &counter, &get_results_thread1));
        std::thread thread2(std::bind(scenario2, &counter, &get_results_thread2));
        thread1.join();
        thread2.join();

        std::vector<int> results;
        results.push_back(get_results_thread1[0]);
        results.push_back(get_results_thread2[0]);
        results.push_back(get_results_thread2[1]);

        assert(results == possible_operations_results1 ||
               results == possible_operations_results2 ||
               results == possible_operations_results3 ||
               results == possible_operations_results4 ||
               results == possible_operations_results5 ||
               results == possible_operations_results6 ||
               results == possible_operations_results7
        );
    }
}


int main() {
    test_sequential_inc_get(10000);
    test_scenario(30000);
    return 0;
}
