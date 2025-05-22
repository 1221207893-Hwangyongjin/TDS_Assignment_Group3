#include <iostream>
using namespace std;

// —— Custom stack, only stores the indices to be checked ——
class ADTstack {
private:
    int stackArr[100];
    int topstack;
public:
    ADTstack() : topstack(-1) {}
    bool empty() const { return topstack == -1; }
    bool full()  const { return topstack == 99; }
    void push(int x) {
        if (!full()) stackArr[++topstack] = x;
        else          cout << "Stack full, cannot push " << x << "\n";
    }
    int pop() {
        if (!empty()) return stackArr[topstack--];
        cout << "Stack empty on pop\n";
        return -1;
    }
};

// —— Stack-based Gnome Sort ——
void gnomeSortWithStack(int arr[], int n) {
    ADTstack st;
    // Start by comparing arr[1] with arr[0]
    st.push(1);

    while (!st.empty()) {
        int i = st.pop();

        // If i is out of bounds or back to 0, restart from 1
        if (i <= 0) {
            st.push(1);
            continue;
        }
        if (i >= n) {
            // If i exceeds right boundary, do not push new index
            continue;
        }

        // Compare and decide next step
        if (arr[i] >= arr[i - 1]) {
            // In correct order → move forward
            st.push(i + 1);
        } else {
            // Out of order → swap, then step back
            swap(arr[i], arr[i - 1]);
            st.push(i - 1);
        }
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << (i + 1 < n ? " " : "\n");
}

int main() {
    int arr[] = {34, 2, 10, -9};
    int n = sizeof(arr) / sizeof(arr[0]);

    gnomeSortWithStack(arr, n);
    cout << "Final sorted: ";
    printArray(arr, n);
    return 0;
}
