#include <iostream>
#include <fstream> //file handling
#include <vector> //menyimpan banyak tugas
#include <string> //untuk memakai string
#include <sstream> //parsing

using namespace std;

//struktur data tugas
struct Task {
    string subject; //mapel
    string name; //judul tugas
    string deadline; //deadline
    bool done = false;
};

bool isValidDateFormat(const string& date) {
    int dd, mm, yyyy;
    char dash1, dash2;
    istringstream ss(date);
    
    // Format harus: dua angka, '-', dua angka, '-', empat angka
    if (!(ss >> dd >> dash1 >> mm >> dash2 >> yyyy)) return false;
    if (dash1 != '-' || dash2 != '-') return false;

    // Cek range tanggal valid
    if (dd < 1 || mm < 1 || mm > 12 || yyyy < 1900) return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31,
                          31, 30, 31, 30, 31 };

    // Tahun kabisat
    if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0))
        daysInMonth[1] = 29;

    if (dd > daysInMonth[mm - 1]) return false;

    return true;
}

vector<Task> tasks; //tugas disimpan di vektor

//baca data tugas dari file tasks.txt
void loadTasks() {
    tasks.clear();						//biar ga double, dikosongin dlu
    ifstream file("tasks.txt");			//buka file tasks.txt
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Task t;
        getline(ss, t.subject, '|');
        getline(ss, t.name, '|');
        getline(ss, t.deadline, '|');
        
		string doneStr;
        getline(ss, doneStr); // baca status selesai
        t.done = (doneStr == "1"); // jika "1");
		
	    tasks.push_back(t);				//tambah ke vektor
    }
    file.close();
}

//menyimpan semua file dari vektor ke tasks.txt
void saveTasks() {
    ofstream file("tasks.txt");
    for (int i = 0; i < tasks.size(); i++) {
	file << tasks[i].subject << "|"
	     << tasks[i].name << "|"
	     << tasks[i].deadline << "|"
	     << (tasks[i].done ? "1" : "0") // jika selesai, simpan "1", jika belum "0"
	     << endl;
	}
    file.close();
}

//menambah tugas baru ke daftar
void addTask() {
    Task t;
    cout << "Mata pelajaran: ";
    getline(cin, t.subject);
    cout << "Nama tugas: ";
    getline(cin, t.name);
    string deadline;
	while (true) {
	    cout << "Deadline (DD-MM-YYYY): ";
	    getline(cin, deadline);
	    if (isValidDateFormat(deadline)) break;
	    cout << "Format tanggal tidak valid. Coba lagi (DD-MM-YYYY).\n";
	}
	t.deadline = deadline;
    tasks.push_back(t);							//simpan ke vektor
    saveTasks();								//simpan ke tasks.txt
    cout << "Tugas berhasil ditambahkan!\n";
}

//menampilkan tugas yang ada
void showTasks() {
    if (tasks.empty()) {
        cout << "Belum ada tugas.\n";
        return;
    }
    cout << "\nDaftar Tugas:\n";
    for (int i = 0; i < tasks.size(); i++) {
        cout << i + 1 << ". [" << tasks[i].deadline << "] "
         	 << (tasks[i].done ? "[SELESAI] " : "[ ] ")
             << tasks[i].subject << " - " << tasks[i].name << endl;
    }
}

//input bahwa tugas sudah selesai
void markTaskDone() {
    if (tasks.empty()) {
        cout << "Belum ada tugas.\n";
        return;
    }
    showTasks();
    int index;
    while (true) {
    	cout << "\nMasukkan nomor tugas yang ingin ditandai sebagai selesai: ";
        if (!(cin >> index)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid. Masukkan angka.\n";
            continue;
        }
        cin.ignore(); // Buang newline
        if (index >= 1 && index <= tasks.size()) {
            tasks[index - 1].done = true; // Tandai tugas sebagai selesai
            saveTasks(); // Simpan ke file
            cout << "Tugas sudah ditandai selesai.\n";
            break;
        } else {
            cout << "Nomor tidak valid. Coba lagi.\n";
        }
    }
}

//menghapus semua data tugas
void clearAllTasks() {
    char confirm;
    cout << "Yakin ingin menghapus semua tugas? (y/n): ";
    cin >> confirm;
    cin.ignore();
    if (tolower(confirm) == 'y') {
        tasks.clear();
        saveTasks();
        cout << "Semua tugas berhasil dihapus.\n";
    } else {
        cout << "Penghapusan dibatalkan.\n";
    }
}

//menghapus tugas beerdasarkan input
void deleteTask() {
    if (tasks.empty()) {
        cout << "Belum ada tugas.\n";
        return;
    }
	showTasks();	//tampilkan daftar tugas
	cout << "\n0. Hapus semua tugas";
    int index;

    while (true) {
        cout << "\nMasukkan nomor tugas yang ingin dihapus (0 untuk hapus semua tugas): ";

        if (!(cin >> index )) {	//jika input bukan angka
            cin.clear(); // reset error state
            cin.ignore(10000, '\n'); // buang input error
            cout << "Input tidak valid. Masukkan angka.\n";
            continue;
        }

        cin.ignore(); // buang newline


		if (index == 0) {
			clearAllTasks();
			break;
		} else if (index >= 1 && index <= tasks.size()) {
            tasks.erase(tasks.begin() + index - 1); //hapus tugas yang dipilih
            saveTasks(); //update file
            cout << "Tugas dihapus.\n";
            break;
        } else {
            cout << "Nomor tidak valid. Coba lagi.\n";
        }
    }
}

int main() {
    loadTasks(); //ambil data dari tasks.txt saat program dijalankan

    while (true) {
    	//header
    	cout << "Doozy - Your Tasks Reminder <3\n";
    	cout << "by Azhara H, Anastasya I, Arzeti T - Kelas XI-11\n";
    	//menu
		cout << "\nMenu:\n1. Tambah Tugas\n2. Lihat Semua Tugas\n3. Hapus Tugas\n4. Tandai Tugas Selesai\n5. Keluar\n";
        int choice;
		cout << "Pilih: ";
		//input pilihan menu
		if (!(cin >> choice) || choice < 1 || choice > 5) {
		    cin.clear(); // reset error state
		    cin.ignore(10000, '\n'); // buang input error
		    cout << "Pilihan tidak valid. Masukkan angka antara 1 - 5.\n";
		    system("pause"); // tunggu user tekan tombol
		    system("cls");   // bersihkan layar
		    continue;		 // ke menu awal
		}
		cin.ignore(); // buang newline setelah input valid
		
		//proses menu
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                showTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
			    markTaskDone();
			    break;
            case 5:
                cout << "Keluar dari program.\n";
                return 0; //program selesai
            default:
                cout << "Pilihan tidak valid.\n";
        }
        
        //setelah selesai satu aksi, pause dan clear kembali pada menu awal
        system("pause");
   		system("cls");
    }
}
