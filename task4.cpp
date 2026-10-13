#include <iostream>
using namespace std;

class WaitingRoom {
    int* q;          // dynamic array of patient IDs
    int  cap;        // current capacity
    int  head;       // index of the front patient
    int  Count;      // number of patients currently waiting
    int  nextId;     // auto-incrementing ID counter

public:
    // ---------- Constructor ----------
    WaitingRoom(int c = 4) {
        cap    = c;
        q      = new int[cap];
        head   = 0;
        Count  = 0;
        nextId = 1;
    }

    // ---------- Destructor ----------
    ~WaitingRoom() {
        delete[] q;
    }

    // ---------- RegisterPatient ----------
    void RegisterPatient() {
        // grow if full
        if (Count == cap) {
            int newCap = cap * 2;
            int* nq = new int[newCap];
            for (int i = 0; i < Count; i++)
                nq[i] = q[(head + i) % cap];
            delete[] q;
            q     = nq;
            cap   = newCap;
            head  = 0;
            cout << "   (queue was full - grown to " << cap << ")\n";
        }

        int tail = (head + Count) % cap;
        q[tail]  = nextId;
        cout << "Registered patient with ID = " << nextId << "\n";
        nextId++;
        Count++;
    }

    // ---------- ServePatient ----------
    void ServePatient() {
        if (Count == 0) {
            cout << "No patient waiting - nothing to serve.\n";
            return;
        }
        int id = q[head];
        head = (head + 1) % cap;
        Count--;
        cout << "Serving patient with ID = " << id << "\n";
    }

    // ---------- CancelAll ----------
    void CancelAll() {
        head  = 0;
        Count = 0;
        cout << "All appointments cancelled.\n";
    }

    // ---------- CanDoctorGoHome ----------
    bool CanDoctorGoHome() {
        return Count == 0;
    }

    // ---------- ShowAllPatient (sorted) ----------
    void ShowAllPatient() {
        if (Count == 0) {
            cout << "No patients waiting.\n";
            return;
        }

        // Copy IDs into a temporary array
        int* ids = new int[Count];
        for (int i = 0; i < Count; i++)
            ids[i] = q[(head + i) % cap];

        // Bubble sort (simple and beginner-friendly)
        for (int i = 0; i < Count - 1; i++)
            for (int j = 0; j < Count - 1 - i; j++)
                if (ids[j] > ids[j + 1]) {
                    int t     = ids[j];
                    ids[j]    = ids[j + 1];
                    ids[j + 1] = t;
                }

        cout << "Waiting patients (sorted): ";
        for (int i = 0; i < Count; i++)
            cout << ids[i] << " ";
        cout << "\n";

        delete[] ids;
    }

    // ---------- Helper: how many are waiting ----------
    int size() { return Count; }
};

// ---------------- Main ----------------
int main() {
    cout << "===== TASK 4: Emergency Ward Waiting Room =====\n";
    cout << "Commands:\n";
    cout << "  reg    - register a new patient\n";
    cout << "  serve  - serve the front patient\n";
    cout << "  cancel - cancel all appointments\n";
    cout << "  home   - check if doctor can go home\n";
    cout << "  show   - show all waiting patients (sorted)\n";
    cout << "  quit   - exit\n\n";

    WaitingRoom w(4);       // start with capacity 4

    string cmd;
    while (true) {
        cout << "\n> ";
        cin >> cmd;

        if (cmd == "quit") {
            break;
        }
        else if (cmd == "reg") {
            w.RegisterPatient();
        }
        else if (cmd == "serve") {
            w.ServePatient();
        }
        else if (cmd == "cancel") {
            w.CancelAll();
        }
        else if (cmd == "home") {
            if (w.CanDoctorGoHome())
                cout << "Yes - doctor can go home.\n";
            else
                cout << "No - " << w.size() << " patient(s) still waiting.\n";
        }
        else if (cmd == "show") {
            w.ShowAllPatient();
        }
        else {
            cout << "Unknown command. Try: reg / serve / cancel / home / show / quit\n";
        }
    }

    cout << "\nExiting waiting room system.\n";
    return 0;
}