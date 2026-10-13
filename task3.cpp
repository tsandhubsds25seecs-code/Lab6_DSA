#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ---------------- Print Request ----------------
struct PrintRequest {
    int    id;
    double arrivalTime;
    double processingTime;
};

// ---------------- Circular Queue (dynamic) ----------------
class CircularQueue {
    PrintRequest* items;
    int cap;
    int front;
    int rear;
    int Count;

public:
    CircularQueue(int c = 4) {
        cap   = c;
        items = new PrintRequest[cap];
        front = cap - 1;
        rear  = cap - 1;
        Count = 0;
    }
    ~CircularQueue() { delete[] items; }

    bool isEmpty() { return Count == 0; }
    bool isFull()  { return Count == cap; }
    int  size()    { return Count; }
    int  capacity(){ return cap; }

    // ---------- resize (preserves FIFO order) ----------
    void resize(int newCap) {
        PrintRequest* newItems = new PrintRequest[newCap];
        for (int i = 0; i < Count; i++)
            newItems[i] = items[(front + 1 + i) % cap];
        delete[] items;
        items = newItems;
        cap   = newCap;
        front = newCap - 1;
        rear  = Count - 1;
    }

    void enqueue(PrintRequest r) {
        if (isFull()) {
            cout << "   >> Queue FULL - growing " << cap
                 << " -> " << cap * 2 << "\n";
            resize(cap * 2);
        }
        rear = (rear + 1) % cap;
        items[rear] = r;
        Count++;
    }

    PrintRequest dequeue() {
        front = (front + 1) % cap;
        PrintRequest r = items[front];
        Count--;
        if (cap > 4 && Count < cap / 4) {
            cout << "   >> Queue SPARSE - shrinking " << cap
                 << " -> " << cap / 2 << "\n";
            resize(cap / 2);
        }
        return r;
    }

    PrintRequest getFront() {
        return items[(front + 1) % cap];
    }

    void display(string label) {
        cout << label << "  [size=" << Count
             << ", cap=" << cap << "]  ->  ";
        if (isEmpty()) {
            cout << "(empty)";
        } else {
            for (int i = 0; i < Count; i++) {
                PrintRequest r = items[(front + 1 + i) % cap];
                cout << "R" << r.id
                     << "(a=" << r.arrivalTime
                     << ",p=" << r.processingTime << ") ";
            }
        }
        cout << "\n";
    }
};

// ---------------- Main ----------------
int main() {
    cout << "===== TASK 3: Print Server Simulation =====\n";
    cout << "Commands:\n";
    cout << "  add   - enqueue a new print request\n";
    cout << "  serve - dequeue the front request\n";
    cout << "  show  - display the current queue\n";
    cout << "  quit  - end and show summary\n\n";

    CircularQueue q(4);
    int    nextId      = 1;
    double totalWait   = 0;
    int    servedCount = 0;

    string cmd;
    while (true) {
        cout << "\n> ";
        cin >> cmd;

        // ------ QUIT ------
        if (cmd == "quit") break;

        // ------ ADD ------
        if (cmd == "add") {
            double arr, proc;
            cout << "   Enter arrival time: ";
            cin >> arr;
            cout << "   Enter processing time: ";
            cin >> proc;

            PrintRequest r;
            r.id             = nextId++;
            r.arrivalTime    = arr;
            r.processingTime = proc;

            q.enqueue(r);
            q.display("Enqueue R" + to_string(r.id));
        }
        // ------ SERVE ------
        else if (cmd == "serve") {
            if (q.isEmpty()) {
                cout << "   Queue is empty - nothing to serve.\n";
                continue;
            }
            PrintRequest r = q.dequeue();
            totalWait += r.arrivalTime;    // simplified wait
            servedCount++;
            q.display("Dequeue R" + to_string(r.id) +
                      " (arrived@" + to_string((int)r.arrivalTime) + ")");
        }
        // ------ SHOW ------
        else if (cmd == "show") {
            q.display("Current queue");
            if (!q.isEmpty()) {
                PrintRequest f = q.getFront();
                cout << "   front is R" << f.id
                     << " (arrived@" << f.arrivalTime << ")\n";
            }
        }
        // ------ UNKNOWN ------
        else {
            cout << "   Unknown command. Try: add / serve / show / quit\n";
        }
    }

    // ------ Summary ------
    cout << "\n----- Summary -----\n";
    cout << "Total served      : " << servedCount << "\n";
    cout << "Total waiting time: " << totalWait << "\n";
    cout << fixed << setprecision(2);
    if (servedCount > 0)
        cout << "Average waiting   : "
             << totalWait / servedCount << " time units\n";
    else
        cout << "No requests were served.\n";

    return 0;
}