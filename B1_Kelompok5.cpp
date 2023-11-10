#include <iostream> //Menyediakan fungsi-fungsi dasar untuk masukan (input) dan keluaran (output) di C++.
#include <fstream> //Digunakan untuk berinteraksi dengan file memungkinkan membaca dari dan menulis ke file.
#include <ctime> //Berisi fungsi-fungsi yang berkaitan dengan waktu dan tanggal. Misalnya, time digunakan untuk mendapatkan waktu saat ini.
#include <string> //Untuk mengubah interger menjadi didalam csv agar bisa ditulis
#include <limits> // Mengandung konstanta-konstanta yang menyediakan batas-batas nilai untuk tipe data tertentu.
#include <algorithm> //Menyediakan fungsi-fungsi untuk berbagai operasi algoritma, seperti pencarian dan pengurutan.
#include <cctype> //Fungsi yang berkaitan dengan karakter. Misalnya, tolower digunakan untuk mengubah karakter menjadi huruf kecil.
#include <iomanip> // Untuk manipulator setw
using namespace std;

// struct
struct daftar
{
    int id_pasien;
    string nama_pasien, nama_poli, nama;
    int tanggal, bulan, waktu_mulai, waktu_selesai; // Tambahkan field baru
    daftar *next = NULL;
};
struct poli
{
    int id_poli, jumlah_pasien, tanggal, bulan, waktu_mulai, waktu_selesai;
    string nama_poli, jadwal_poli;
    poli *next;
};
struct user
{
    string nama, nama_poli, tanggal_lahir, alamat;
    long long no_bpjs;
    char jenis_kelamin;
    user *next;
};

// prototype global atau variable global
string username;
string PoliName;
int currentQueueNumber = 0;
user *head = NULL;
poli *headPoli = NULL;
daftar *current = NULL;
void loadData_Poli();

// ---- REGIS LOGIN ------

// FUNSGI UNTUK LOGIN ADMIN
bool admin_login()
{
    string username, password;
    bool loginSuccess = false;

    while (!loginSuccess)
    {
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        if (username == "admin" && password == "admin")
        {
            loginSuccess = true;
            cout << "Selamat Datang Admin" << endl;
            system("pause");
            system("cls");
        }
        else
        {
            cout << "Username atau password salah. Silakan coba lagi." << endl;
        }
    }

    return loginSuccess;
}

// FUNGSI UNTUK MENGECEK DATA REGIS SEPERTI NAMA DAN NO BPJS
bool cek_regis(long long bpjs_number, const string &name, const string &jenis)
{
    ifstream file("akun.csv");
    string line;

    while (getline(file, line))
    {
        size_t pos = line.find(','); // periksa posisi karakter koma
        if (pos != string::npos) 
        {
            long long stored_bpjs = stoll(line.substr(pos + 1, 13));
            string stored_name = line.substr(0, pos);

            if (jenis == "login" && stored_bpjs == bpjs_number && stored_name == name)
            {
                file.close();
                return true; // BPJS number and name match
            }
            else if (jenis == "regis" && stored_bpjs == bpjs_number)
            {
                file.close();
                return true; // BPJS number already exists
            }
        }
    }

    file.close();
    return false; // BPJS number (and optionally name) not found
}

// FUNGSI UNTUK REGIS DAN MENYIMPAN DATA REGIS NYA KE CSV
void regis_csv()
{
    cout << "Masukkan Nama Anda: ";
    cin >> ws;
    string regis_nama;
    getline(cin, regis_nama);

    long long bpjs_number = 0;
    bool valid_bpjs = false;

    while (!valid_bpjs)
    {
        cout << "Masukkan NO BPJS (10 digit angka terakhir): ";
        cin >> bpjs_number;
        cin.clear();
        cin.ignore();

        if (to_string(bpjs_number).length() == 10)
        {
            if (cek_regis(bpjs_number, regis_nama, "regis"))
            {
                cout << "Nomor BPJS atau Nama sudah ada. Silakan Masukkan Nomor BPJS dan Nama anda dengan benar!" << endl;
            }
            else
            {
                valid_bpjs = true;
            }
        }
        else
        {
            cout << "Nomor BPJS harus terdiri dari angka dan 10 digit." << endl;
            cin.clear();
            cin.ignore();
        }
    }

    user user_baru;
    user_baru.no_bpjs = bpjs_number;

    cout << "Masukkan Tanggal Lahir: ";
    cin >> ws;
    getline(cin, user_baru.tanggal_lahir);

    bool jenis_kelamin_valid = false;
    while (!jenis_kelamin_valid)
    {
        cout << "Masukkan Jenis Kelamin (L/P): ";
        cin >> user_baru.jenis_kelamin;
        user_baru.jenis_kelamin = tolower(user_baru.jenis_kelamin);
        cin.ignore();

        if (user_baru.jenis_kelamin == 'l' || user_baru.jenis_kelamin == 'p')
        {
            jenis_kelamin_valid = true;
        }
        else
        {
            cout << "Jenis Kelamin tidak valid. Harap masukkan 'L' atau 'P'." << endl;
        }
    }

    cout << "Masukkan Alamat: ";
    cin >> ws;
    getline(cin, user_baru.alamat);

    // Simpan data dalam file CSV
    ofstream file;
    file.open("akun.csv", ios::app);
    if (file.is_open())
    {
        file << regis_nama << ',' << setw(13) << setfill('0') << bpjs_number << ','
             << user_baru.tanggal_lahir << ',' << user_baru.jenis_kelamin << ',' << user_baru.alamat << '\n';
        file.close();
    }
    else
    {
        cout << "Gagal membuka file CSV untuk penulisan." << endl;
    }

    cout << "Registrasi berhasil. Anda bisa login dengan BPJS dan nama Anda." << endl;
}

// FUNGSI UNTUK LOGIN USER (PASIEN)
bool loginPasien()
{
    cout << "Masukkan nama: ";
    string login_usr;
    fflush(stdin);
    getline(cin, login_usr);
    cout << "Masukkan BPJS (10 digit terakhir): ";
    long long int login_bpjs;

    // Check if the input is successful
    if (!(cin >> login_bpjs))
    {
        cin.clear();
        cin.ignore();
        cout << "Invalid input. Please enter a valid BPJS number." << endl;
        system("pause");
        system("cls");
        return false;
    }

    bool login = cek_regis(login_bpjs, login_usr, "login");

    if (login)
    {
        cout << "Anda Berhasil Login\n"
             << endl;
        username = login_usr;
        system("pause");
        system("cls");
        return true;
    }
    else
    {
        cout << "Login Gagal, Silahkan registrasi atau login kembali!" << endl;
        system("pause");
        system("cls");
        return false;
    }
}


// UNTUK MEMUAT ULANG DATA REGIS PASIEN DI CSV AKUN
void loadData_Pasien()
{
    ifstream file("akun.csv");
    if (!file.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string nama, no_bpjs, tanggal_lahir, jenis_kelamin, alamat;

        getline(ss, nama, ',');
        getline(ss, no_bpjs, ',');
        getline(ss, tanggal_lahir, ',');
        getline(ss, jenis_kelamin, ',');
        getline(ss, alamat, ',');

        try
        {
            user *user_baru = new user;
            user_baru->nama = nama;
            user_baru->no_bpjs = stoll(no_bpjs);
            user_baru->tanggal_lahir = tanggal_lahir;
            user_baru->jenis_kelamin = (jenis_kelamin == "L") ? 'L' : 'P'; // untuk konversi char nya
            user_baru->alamat = alamat;

            // Simpan data pasien ke linked list
            user_baru->next = head;
            head = user_baru;
        }
        catch (const invalid_argument &e)
        {
            cout << "Error converting data: " << e.what() << endl;
        }
    }

    file.close();
}

// -------- DAFTAR ONLINE ---------

// UNTUK MENULIS DATA DAFTAR ONLINE PASIEN KE DAFTAR CSV
void writeFileDaftar(daftar *front, poli *headPoli)
{
    ofstream file;
    file.open("daftar.csv", ios::out | ios::trunc); // Buka file dalam mode tulis dan hapus isi sebelumnya (trunc)

    if (!file.is_open())
    {
        cout << "Gagal membuka file" << endl;
        return;
    }

    // Tulis ulang seluruh antrian ke dalam file
    daftar *current = front;
    while (current != NULL)
    {
        string namaPoliDaftar = current->nama_poli;
        transform(namaPoliDaftar.begin(), namaPoliDaftar.end(), namaPoliDaftar.begin(), ::tolower); // Ubah ke huruf kecil

        // Cari nama poli yang cocok dalam headPoli
        poli *currentPoli = headPoli;
        while (currentPoli != NULL)
        {
            string namaPoliHead = currentPoli->nama_poli;
            transform(namaPoliHead.begin(), namaPoliHead.end(), namaPoliHead.begin(), ::tolower); // Ubah ke huruf kecil

            if (namaPoliHead == namaPoliDaftar)
            {
                file << current->id_pasien << "," << current->nama_pasien << "," << currentPoli->nama_poli << "\n";
                break; // Keluar dari loop jika sudah ditemukan nama poli yang cocok
            }
            currentPoli = currentPoli->next;
        }
        current = current->next;
    }

    file.close();
}

// UNTUK MEMASUKKAN NEW NODE NYA KE ANTRIAN
void insertDaftar(daftar **front, daftar **rear, daftar *newNode)
{
    if (*front == NULL)
    {
        *front = *rear = newNode;
    }
    else
    {
        (*rear)->next = newNode;
        *rear = newNode;
    }
}

// UNTUK MEMUAT ULANG DATA DAFTAR ONLINE PASIEN DI DAFTAR CSV
void loadData_Daftar(daftar **front, daftar **rear, poli *head, int &currentQueueNumber)
{
    string line;
    *front = *rear = NULL;
    poli *tempPoli = head;
    ifstream file("daftar.csv");
    if (!file.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    int jumlahPoli = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue; // Skip empty lines

        stringstream ss(line);
        daftar *newNode = new daftar;

        try
        {
            getline(ss, line, ','); // Get id_pasien
            newNode->id_pasien = stoi(line);
            getline(ss, newNode->nama_pasien, ',');
            getline(ss, newNode->nama_poli, ','); // Update to read the "Poli" field
            getline(ss, newNode->nama, '\n');     // Update to read the "Username" field
            newNode->next = NULL;

            // Insert the new node into the queue
            // Prepare the next queue number
            insertDaftar(front, rear, newNode);

            // Update currentQueueNumber if necessary
            if (newNode->id_pasien > currentQueueNumber)
            {
                currentQueueNumber = newNode->id_pasien;
            }
        }
        catch (const std::invalid_argument &e)
        {
            cout << "Ada kesalahan dalam format file: " << e.what() << endl;
            // Handle the error, e.g., by skipping this line or aborting the function
            delete newNode; // Clean up the new node
            continue;       // Skip this line and continue with the next
        }
    }
    file.close();
}

// UNTUK MEMERIKSA ENTRI DALAM ANTRIAN YANG SESUAI DENGAN USERNAME DAN POLINAME
bool checkAntrianSaya(daftar *front, string &username, string &Poli)
{
    while (front != NULL)
    {
        if (front->nama == username && front->nama_poli == PoliName)
        {
            return true;
        }

        front = front->next;
    }

    return false;
}

// UNTUK MENAMPILKAN SELURUH ANTRIAN PER-USER
void displayAntrian(daftar *front)
{
    if (front == NULL)
    {
        cout << "-----------------------------" << endl;
        cout << "Antrian Kosong" << endl;
        cout << "-----------------------------" << endl;
        return;
    }

    int queuePosition = 1;
    daftar *current = front;

    time_t currTime;
    time(&currTime);
    struct tm newTime;

    cout << "-----------------------------" << endl;
    while (current != NULL)
    {
        if (current->nama_pasien == username)
        {
            cout << "Nomor Antrian: " << current->id_pasien << endl;
            cout << "Nama Pasien  : " << current->nama_pasien << endl;
            cout << "Nama Poli    : " << current->nama_poli << endl;
            cout << "Tanggal      : " << current->tanggal << "/" << current->bulan << endl;
            cout << "Waktu Mulai  : " << setfill('0') << setw(2) << current->waktu_mulai << ":00" << endl;
            cout << "Waktu Selesai: " << setfill('0') << setw(2) << current->waktu_selesai << ":00" << endl;

            newTime.tm_year = 123; // Asumsikan tahun 2023
            newTime.tm_mon = current->bulan - 1;
            newTime.tm_mday = current->tanggal;
            newTime.tm_hour = current->waktu_selesai;
            newTime.tm_min = 0;
            newTime.tm_sec = 0;
            time_t deadline = mktime(&newTime);
            double secondsUntilEnd = difftime(deadline, currTime);
            int hoursUntilEnd = secondsUntilEnd / 3600;
            cout << "Waktu Sampai Pelayanan Poli Berakhir: " << hoursUntilEnd << " jam " << endl;

            cout << "-----------------------------" << endl;
        }
        current = current->next;
        queuePosition++;
    }
    cout << endl;
}

//  UNTUK MENGURANGI JUMLAH PASIEN DALAM SUATU POLI TERTENTU DI POLI CSV
poli *decrementPoliPatientCount(poli *&headPoli, const string &poliName)
{
    poli *current = headPoli;
    while (current != NULL)
    {
        if (current->nama_poli == poliName && current->jumlah_pasien > 0)
        {
            current->jumlah_pasien--;
            return current; // Return the updated poli
        }
        current = current->next;
    }
    return NULL; // Poli not found or no patients to decrement
}

// UNTUK MENCARI DAN MENGEMBALIKAN POINTER KE OBJEK POLI YANG MEMILIKI NAMA POLI YANG SESUAI DENGAN NAMA POLI YANG DIMASUKKAN 
poli *findPoliName(const string &PoliName)
{
    string lowerPoliName = PoliName;
    transform(lowerPoliName.begin(), lowerPoliName.end(), lowerPoliName.begin(), ::tolower); // Konversi ke huruf kecil

    poli *current = headPoli;
    while (current != NULL)
    {
        string currentLowerPoliName = current->nama_poli;
        transform(currentLowerPoliName.begin(), currentLowerPoliName.end(), currentLowerPoliName.begin(), ::tolower); // Konversi ke huruf kecil

        if (currentLowerPoliName == lowerPoliName)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// UNTUK MENYIMAN PERUBAHAN POLI YANG TELAH DILAKUKAN SETELAH MELAKUKAN PENDAFTARAN
void savePoliChanges(poli *headPoli)
{
    ofstream file("poli.csv", ios::out | ios::trunc);
    if (!file.is_open())
    {
        cout << "Failed to open poli.csv file for writing." << endl;
        return;
    }

    poli *current = headPoli;
    while (current != NULL)
    {
        string jadwal_poli = to_string(current->tanggal) + "/" + to_string(current->bulan) + " " +
                             to_string(current->waktu_mulai) + "-" + to_string(current->waktu_selesai);

        file << current->id_poli << ","
             << current->nama_poli << ","
             << current->jumlah_pasien << ","
             << jadwal_poli << "\n";
        current = current->next;
    }

    file.close();
}

// UNTUK MENAMBAHKAN PENDAFTARAN DENGAN QUEUE (ANTRIAN)
void enqueue(daftar **front, daftar **rear, string &username, const string &PoliName)
{
    string lowerPoliName = PoliName;
    transform(lowerPoliName.begin(), lowerPoliName.end(), lowerPoliName.begin(), ::tolower); // Konversi ke huruf kecil

    // Mencari poli yang sesuai
    poli *poliToEnroll = findPoliName(lowerPoliName);

    if (poliToEnroll != NULL && poliToEnroll->jumlah_pasien > 0)
    {
        // Decrement the available slots
        poliToEnroll->jumlah_pasien--;

        daftar *tempDaftar = *front;
        int currentQueue = 1;
        while (tempDaftar != NULL)
        {
            string tempLowerPoliName = tempDaftar->nama_poli;
            transform(tempLowerPoliName.begin(), tempLowerPoliName.end(), tempLowerPoliName.begin(), ::tolower); // Konversi ke huruf kecil

            if (tempLowerPoliName == lowerPoliName)
            {
                currentQueue++;
            }
            tempDaftar = tempDaftar->next;
        }

        // Jika ada slot, lakukan proses pendaftaran
        daftar *newPatient = new daftar();
        newPatient->nama_pasien = username;
        newPatient->nama_poli = PoliName;
        newPatient->tanggal = poliToEnroll->tanggal;
        newPatient->bulan = poliToEnroll->bulan;
        newPatient->waktu_mulai = poliToEnroll->waktu_mulai;
        newPatient->waktu_selesai = poliToEnroll->waktu_selesai;
        newPatient->id_pasien = currentQueue;

        if (*rear)
        {
            (*rear)->next = newPatient;
        }
        else
        {
            *front = newPatient;
        }
        *rear = newPatient;

        // Simpan perubahan ke dalam file
        savePoliChanges(headPoli);
        writeFileDaftar(*front, headPoli);

        cout << "Anda Berhasil Terdaftar pada poli " << PoliName << endl;
    }
    else
    {
        cout << "Poli tidak tersedia" << endl;
    }
}

// UNTUK MELAKUKAN PENCARIAN ATAU SEARCHING POLI
const int NO_OF_CHARS = 256;
// Fungsi untuk membuat tabel heuristik bad character
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS])
{
    // Inisialisasi semua entri badChar dengan -1
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badChar[i] = -1;
    }

    // Mengisi nilai indeks terakhir dari setiap karakter dalam pola
    for (int i = 0; i < size; i++)
    {
        badChar[(int)str[i]] = i;
    }
}

// Fungsi untuk melakukan pencarian menggunakan algoritma Boyer-Moore
void boyerMoore(poli *head, const string &pat)
{
    // Mengubah pola dan nama poli menjadi huruf kecil untuk pencarian yang case-insensitive
    string lowerPat = pat;
    transform(lowerPat.begin(), lowerPat.end(), lowerPat.begin(), ::tolower);

    int count = 0; // Untuk menghitung jumlah poli yang ditemukan

    poli *currentPoli = head;

    // Loop melalui setiap poli dalam linked list
    while (currentPoli != NULL)
    {
        // Mengubah nama poli menjadi huruf kecil untuk pencarian yang case-insensitive
        string lowerNamaPoli = currentPoli->nama_poli;
        transform(lowerNamaPoli.begin(), lowerNamaPoli.end(), lowerNamaPoli.begin(), ::tolower);

        // Mengecek apakah pola ditemukan dalam nama poli menggunakan algoritma Boyer-Moore
        if (lowerNamaPoli.find(lowerPat) != string::npos)
        {
            // Menampilkan informasi poli yang ditemukan
            cout << "ID Poli: " << currentPoli->id_poli << endl;
            cout << "Nama Poli : " << currentPoli->nama_poli << endl;
            cout << "Jumlah Pasien: " << currentPoli->jumlah_pasien << endl;
            cout << "Tanggal Pelayanan: " << currentPoli->tanggal << "/" << currentPoli->bulan << endl;
            cout << "Waktu Pelayanan: " << setfill('0') << setw(2) << currentPoli->waktu_mulai
                 << ":00 - " << setfill('0') << setw(2) << currentPoli->waktu_selesai << ":00" << endl;
            ++count;
        }

        // Pindah ke poli berikutnya dalam linked list
        currentPoli = currentPoli->next;
    }

    // Menampilkan pesan jika tidak ada poli yang ditemukan
    if (count == 0)
    {
        cout << "Data tidak ditemukan" << endl;
    }
}

// -------- MENU ADMIN ---------

//DIGUNAKAN UNTUK MENENTUKAN APAKAH TAHUN TERSEBUT KABISAT ATAU TIDAK
bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// UNTUK MENULIS/ CREATE POLI DAN MEMASUKKAN NYA KE POLI CSV
void writeFilePoli(poli *&head, int &currentID)
{
    // Membuat objek poli baru dan membukakan file "poli.csv" dalam mode append
    poli *newPoli = new poli();
    ofstream file;
    file.open("poli.csv", ios::out | ios::app); // Append to the file

    // Mengecek apakah file berhasil dibuka
    if (!file.is_open())
    {
        cout << "Gagal membuka file" << endl;
        return;
    }

    // Menetapkan ID poli baru dan menambahkannya ke awal daftar poli
    newPoli->id_poli = currentID++;
    newPoli->next = head;
    head = newPoli;

    // Meminta admin memasukkan detail poli baru
    cout << "Nama Poli: ";
    cin >> newPoli->nama_poli;

    cout << "Jumlah Pasien: ";
    cin >> newPoli->jumlah_pasien;

    // Memvalidasi input jumlah pasien
    while (cin.fail() || newPoli->jumlah_pasien <= 0)
    {
        cin.clear();           // Bersihkan input stream dari kesalahan sebelumnya
        cin.ignore(100, '\n'); // Abaikan hingga 100 karakter berikutnya atau sampai newline
        cout << "Jumlah Pasien harus berupa angka dan di atas 0." << endl;

        // Minta input ulang
        cout << "Jumlah Pasien: ";
        cin >> newPoli->jumlah_pasien;
    }

    // Mendapatkan tahun saat ini secara manual (atau bisa dimasukkan oleh admin)
    int year = 2023; // Ganti dengan tahun saat ini atau masukan admin

    // Meminta admin memasukkan bulan (1-12) dan memvalidasi input
    cout << "Bulan (1-12): ";
    while (!(cin >> newPoli->bulan) || newPoli->bulan < 1 || newPoli->bulan > 12)
    {
        cin.clear();  // Bersihkan flag kesalahan
        cin.ignore(); // Abaikan input yang salah hingga newline berikutnya
        cout << "Input tidak valid. Harap masukkan bulan (1-12): ";
    }

    // Menetapkan jumlah maksimum hari berdasarkan bulan
    int maxDays = 31; // Jumlah hari maksimum default
    if (newPoli->bulan == 4 || newPoli->bulan == 6 || newPoli->bulan == 9 || newPoli->bulan == 11)
    {
        maxDays = 30; // April, Juni, September, November memiliki 30 hari
    }
    else if (newPoli->bulan == 2)
    {
        maxDays = isLeapYear(year) ? 29 : 28; // Jumlah hari Februari tergantung pada tahun kabisat
    }

    // Meminta admin memasukkan tanggal (1-maxDays) dan memvalidasi input
    cout << "Tanggal (1-" << maxDays << "): ";
    while (!(cin >> newPoli->tanggal) || newPoli->tanggal < 1 || newPoli->tanggal > maxDays)
    {
        cin.clear();  // Bersihkan flag kesalahan
        cin.ignore(); // Abaikan input yang salah hingga newline berikutnya
        cout << "Input tidak valid. Harap masukkan tanggal (1-" << maxDays << "): ";
    }

    // Meminta admin memasukkan jam mulai (0-23) dan memvalidasi input
    cout << "Jam Mulai (0-23): ";
    cin >> newPoli->waktu_mulai;
    while (cin.fail() || newPoli->waktu_mulai < 0 || newPoli->waktu_mulai > 23)
    {
        cin.clear();  // Bersihkan flag kesalahan
        cin.ignore(); // Abaikan karakter yang salah
        cout << "Input tidak valid. Harap masukkan angka dari 0 hingga 23 untuk jam mulai." << endl;
        cout << "Jam Mulai (0-23): ";
        cin >> newPoli->waktu_mulai;
    }

    // Meminta admin memasukkan jam selesai (0-23) dan memvalidasi input
    cout << "Jam Selesai (0-23): ";
    cin >> newPoli->waktu_selesai;
    while (cin.fail() || newPoli->waktu_selesai < 0 || newPoli->waktu_selesai > 23)
    {
        cin.clear();  // Bersihkan flag kesalahan
        cin.ignore(); // Abaikan karakter yang salah
        cout << "Input tidak valid. Harap masukkan angka dari 0 hingga 23 untuk jam selesai." << endl;
        cout << "Jam Selesai (0-23): ";
        cin >> newPoli->waktu_selesai;
    }

    // Membuat string jadwal_poli
    newPoli->jadwal_poli = to_string(newPoli->tanggal) + "/" + to_string(newPoli->bulan) +
                           " " + to_string(newPoli->waktu_mulai) + "-" + to_string(newPoli->waktu_selesai);

    // Menambahkan data poli baru ke dalam file
    file << newPoli->id_poli << "," << newPoli->nama_poli << ","
         << newPoli->jumlah_pasien << "," << newPoli->tanggal << ","
         << newPoli->bulan << "," << newPoli->waktu_mulai << ","
         << newPoli->waktu_selesai << "," << newPoli->jadwal_poli << "\n";

    cout << "Poli berhasil ditambahkan dengan ID: " << newPoli->id_poli << endl;
    file.close();
}

// UNTUK MENAMPILKAN (DISPLAY) POLI YANG TELAH DIBUAT KE ADMIN
void displayPoli(poli *headPoli)
{
    // Pengecekan apakah daftar poli kosong
    if (headPoli == NULL)
    {
        cout << "Tidak ada data poli untuk ditampilkan." << endl;
        return;
    }

    // Header tabel untuk menampilkan informasi poli
    cout << "+--------+-----------------+-----------------+-----------------+-----------------+--------------+-------------------+\n";
    cout << "|  Id    |    Nama Poli    |      Tanggal    |      Waktu Pelayanan        | Batas Waktu Pelayanan | Jumlah Pasien  |\n";
    cout << "+--------+-----------------+-----------------+-----------------+-----------------+--------------+-------------------+\n";

    // Mendapatkan waktu saat ini
    time_t current_time;
    time(&current_time);
    struct tm deadline_time;

    // Pointer ke poli saat ini
    poli *currentPoli = headPoli;

    // Iterasi melalui setiap elemen dalam daftar poli
    while (currentPoli != NULL)
    {
        // Menampilkan informasi poli seperti Id, Nama Poli, Tanggal, Waktu Pelayanan, dll.
        cout << "| " << setw(6) << setfill(' ') << left << currentPoli->id_poli << " | ";
        cout << setw(15)<< setfill(' ') << left << currentPoli->nama_poli << " | ";
        cout << setw(12) << setfill(' ')<< left << currentPoli->tanggal << "/" << currentPoli->bulan << " | ";
        cout << setw(18) << setfill(' ')<< left << currentPoli->waktu_mulai << ":00-" << setw(1) << left << currentPoli->waktu_selesai << ":00 | ";

        // Convert the date and time to a time_t structure
        deadline_time.tm_year = 123;                   // Year 2023
        deadline_time.tm_mon = currentPoli->bulan - 1; // Adjust the month to be zero-based
        deadline_time.tm_mday = currentPoli->tanggal;
        deadline_time.tm_hour = currentPoli->waktu_selesai;
        deadline_time.tm_min = 0;
        deadline_time.tm_sec = 0;

        // Mengonversi waktu batas pelayanan poli ke dalam time_t
        time_t deadline = mktime(&deadline_time);

        // Menghitung sisa waktu hingga batas waktu pelayanan
        double remaining = difftime(deadline, current_time);
        int remaining_hours = static_cast<int>(remaining) / 3600;

        // Menampilkan informasi waktu pelayanan
        if (remaining_hours <= 0)
        {
            cout << setw(15) << setfill(' ') << left << "Poli Telah Tutup"
                 << " | ";
        }
        else
        {
            cout << setw(17) << setfill(' ') << left << remaining_hours << " jam"
                 << " | ";
        }

        // Menampilkan jumlah pasien
        cout << setw(14) << setfill(' ') << left << currentPoli->jumlah_pasien << " |\n";

        // Garis batas untuk setiap baris tabel dan pindah ke poli berikutnya
        cout << "+--------+-----------------+-----------------+-----------------+-----------------+-----------------+----------------+\n";
        currentPoli = currentPoli->next;
    }
}

//UNTUK MELAKUKAN SORTING POLI BERDASARKAN NAMA POLI SECARA ASC

//FUNGSI UNTUK MENGHITUNG & MENGEMBALIKAN JUMLAH ELEMEN DARI DAFTAR POLI YANG DIWAKILI OLEH HEADPOLI
int length(poli *headPoli)
{
    int panjang = 0;
    poli *temp = headPoli;
    while (temp != NULL)
    {
        temp = temp->next;
        panjang++;
    }
    return panjang;
}

//FUNGSI MENUKAR POSISI DUA POLI A & B JUGA MENANGANI PEMBARUAN HEAD JIKA ELEMEN YANG DITUKAR ADALAH POLI PERTAMA
void swapPoli(poli **head, poli *a, poli *b)
{
    if (a == b)
        return;
    poli *prevA = NULL, *prevB = NULL, *curr = *head;
    while (curr)
    {
        if (curr->next == a)
            prevA = curr;
        if (curr->next == b)
            prevB = curr;
        curr = curr->next;
    }
    if (a == *head)
        *head = b;
    else if (b == *head)
        *head = a;
    if (prevA)
        prevA->next = b;
    if (prevB)
        prevB->next = a;
    poli *temp = a->next;

    a->next = b->next;
    b->next = temp;
}

//FUNGSI MENCARI DAN MENGEMBALIKAN POINTER KE ELEMEN POLI PADA IDX TERTERNTU DALAM DAFTAR YANG DIWAKILI OLEH HEAD
poli *findpoli(poli *head, int idx)
{
    poli *currentPoli = head;
    for (int i = 0; i < idx; i++)
    {
        currentPoli = currentPoli->next;
    }
    return currentPoli;
}

//FUNGSI INI MERUPAKAN ALGORITMA SHELL SORT
void shellSortASC(poli **headPoli, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            poli *temp = findpoli(*headPoli, i);
            int j;
            for (j = i; j >= gap && findpoli(*headPoli, j - gap)->nama_poli > temp->nama_poli; j -= gap)

            {
                swapPoli(headPoli, findpoli(*headPoli, j), findpoli(*headPoli, j - gap));
            }
        }
    }
}

// UNTUK MENULIS ULANG DATA POLI YANG TELAH DIUPDATE KE POLI CSV
void replace(poli *headPoli)
{
    // Membuka file poli.csv untuk ditulis, mode "ios::out" akan menimpa file yang sudah ada
    ofstream file;
    file.open("poli.csv", ios::out);

    // Memeriksa apakah file berhasil dibuka
    if (!file.is_open())
    {
        cout << "Gagal membuka file." << endl;
        return;
    }

    // Iterasi melalui linked list dan menulis data setiap poli ke dalam file
    poli *currentPoli = headPoli;
    while (currentPoli != NULL)
    {
        // Menulis data poli ke dalam file CSV dengan pemisah koma
        file << currentPoli->id_poli << "," << currentPoli->nama_poli << ","
             << currentPoli->jumlah_pasien << "," << currentPoli->tanggal << ","
             << currentPoli->bulan << "," << currentPoli->waktu_mulai << ","
             << currentPoli->waktu_selesai << "," << currentPoli->jadwal_poli << "\n";

        // Pindah ke poli berikutnya dalam iterasi
        currentPoli = currentPoli->next;
    }

    // Menutup file setelah selesai menulis
    file.close();
}

// UNTUK MELAKUKAN UPDATE POLI 

// Fungsi untuk memperbarui informasi poliklinik berdasarkan ID
void updatePoli(poli *&headPoli, int id)
{
    // Membuat pointer ke poli saat ini dan menginisialisasi dengan headPoli
    poli *currentPoli = headPoli;

    // Menelusuri linked list hingga menemukan poli dengan ID yang sesuai atau mencapai akhir list
    while (currentPoli != NULL && currentPoli->id_poli != id)
    {
        currentPoli = currentPoli->next;
    }

    // Memeriksa apakah poli dengan ID yang dicari ditemukan
    if (currentPoli == NULL)
    {
        cout << "Poli dengan ID " << id << " tidak ditemukan." << endl;
        return;
    }

    // Menampilkan opsi untuk data yang dapat diperbarui
    cout << "Pilih data yang akan diperbarui:" << endl;
    cout << "1. Nama Poli" << endl;
    cout << "2. Jumlah Pasien" << endl;
    cout << "3. Tanggal" << endl;
    cout << "4. Bulan" << endl;
    cout << "5. Waktu Mulai" << endl;
    cout << "6. Waktu Selesai" << endl;
    cout << "Pilih data yang akan diperbarui : ";

    // Meminta admin untuk memilih opsi
    int option;
    cin >> option;

    // Menangani pembaruan berdasarkan opsi yang dipilih
    if (option == 1)
    {
        // Meminta dan memperbarui nama poli
        cout << "Masukkan nama baru untuk Poli: ";
        cin >> currentPoli->nama_poli;
    }
    else if (option == 2)
    {
        // Meminta dan memperbarui jumlah pasien
        cout << "Masukkan jumlah pasien baru: ";
        cin >> currentPoli->jumlah_pasien;
        while (cin.fail() || currentPoli->jumlah_pasien <= 0)
        {
            // Menangani kesalahan input
            cin.clear();           
            cin.ignore(100, '\n'); 
            cout << "Jumlah Pasien harus berupa angka dan di atas 0." << endl;
            cout << "Jumlah Pasien: ";
            cin >> currentPoli->jumlah_pasien;
        }
    }
    else if (option == 3)
    {
        // Meminta dan memperbarui tanggal
        int year = 2023; // Ganti dengan tahun saat ini atau input admin
        int maxDays = 31;
        if (currentPoli->bulan == 4 || currentPoli->bulan == 6 || currentPoli->bulan == 9 || currentPoli->bulan == 11)
        {
            maxDays = 30;
        }
        else if (currentPoli->bulan == 2)
        {
            maxDays = isLeapYear(year) ? 29 : 28;
        }
        cout << "Masukkan tanggal baru (1-" << maxDays << "): ";
        while (!(cin >> currentPoli->tanggal) || currentPoli->tanggal < 1 || currentPoli->tanggal > maxDays)
        {
            cin.clear();  
            cin.ignore(); 
            cout << "Input tidak valid. Harap masukkan tanggal (1-" << maxDays << "): ";
        }
    }
    else if (option == 4)
    {
        // Meminta dan memperbarui bulan
        cout << "Masukkan Bulan baru (1-12): ";
        while (!(cin >> currentPoli->bulan) || currentPoli->bulan < 1 || currentPoli->bulan > 12)
        {
            cin.clear();  
            cin.ignore(); 
            cout << "Input tidak valid. Harap masukkan bulan (1-12): ";
        }
    }
    else if (option == 5)
    {
        // Meminta dan memperbarui waktu mulai
        cout << "Masukkan waktu mulai baru (0-23): ";
        cin >> currentPoli->waktu_mulai;
        while (cin.fail() || currentPoli->waktu_mulai < 0 || currentPoli->waktu_mulai > 23)
        {
            cin.clear();  
            cin.ignore(); 
            cout << "Input tidak valid. Harap masukkan angka dari 0 hingga 23 untuk waktu mulai." << endl;
            cout << "Masukkan waktu mulai baru (0-23): ";
            cin >> currentPoli->waktu_mulai;
        }
    }
    else if (option == 6)
    {
        // Meminta dan memperbarui waktu selesai
        cout << "Masukkan waktu selesai baru (0-23): ";
        cin >> currentPoli->waktu_selesai;
        while (cin.fail() || currentPoli->waktu_selesai < 0 || currentPoli->waktu_selesai > 23)
        {
            cin.clear();  
            cin.ignore(); 
            cout << "Input tidak valid. Harap masukkan angka dari 0 hingga 23 untuk waktu selesai." << endl;
            cout << "Masukkan waktu selesai baru (0-23): ";
            cin >> currentPoli->waktu_selesai;
        }
    }
    else
    {
        cout << "Opsi tidak valid." << endl;
        return;
    }

    // Memperbarui jadwal poli setelah perubahan
    currentPoli->jadwal_poli = to_string(currentPoli->tanggal) + "/" +
                               to_string(currentPoli->bulan) + " " +
                               to_string(currentPoli->waktu_mulai) + "-" +
                               to_string(currentPoli->waktu_selesai);

    // Menyimpan perubahan ke dalam file
    replace(headPoli);

    // Memberikan pesan bahwa poli telah berhasil diperbarui
    cout << "Poli berhasil diperbarui." << endl;
}

// UNTUK MENGHAPUS ANTRIAN DAFTAR PASIEN 

// Fungsi untuk dequeue (menghapus elemen dari depan) dari antrian pasien
void dequeue(daftar **front)
{
    // Memeriksa apakah antrian kosong
    if (*front == NULL)
    {
        cout << "Antrian Kosong" << endl;
        return;
    }

    // Menyimpan pointer ke elemen yang akan dihapus
    daftar *temp = *front;

    // Menampilkan informasi pasien yang dihapus
    cout << "Menghapus pasien dengan nama: " << temp->nama_pasien << endl;

    // Memindahkan front ke elemen berikutnya dalam antrian
    *front = (*front)->next;

    // Setelah menghapus dari antrian, tulis ulang daftar.csv dengan antrian yang diperbarui
    writeFileDaftar(*front, headPoli);

    // Menghapus node yang didequeue dari memori
    delete temp;
}

// FUNGSI UNTUK MEMUAT DATA DARI FILE DAFTAR CSV DAN MENAMPILKANNYA 
void loadAndDisplayDaftar()
{
    // Membuka file daftar.csv
    ifstream daftarFile("daftar.csv");
    string line, id_pasien, nama_pasien, nama_poli, no_bpjs;

    // Memeriksa apakah file berhasil dibuka
    if (!daftarFile.is_open())
    {
        cout << "File daftar.csv tidak ditemukan atau gagal dibuka." << endl;
        return;
    }

    // Menampilkan header rekap pasien
    cout << "=========== Rekap Seluruh Pasien ============" << endl;
    cout << "+-------+-------------+-----------------+------------------+-------+" << endl;
    cout << "| No Urut    | Nama Pasien    | Nama Poli        | No BPJS         |" << endl;
    cout << "+-------+-------------+-----------------+------------------+-------+" << endl;

    // Membaca setiap baris dari file daftar.csv
    while (getline(daftarFile, line))
    {
        // Menggunakan stringstream untuk memproses data dari setiap baris
        stringstream ss(line);

        // Mendapatkan data dari setiap kolom
        getline(ss, id_pasien, ',');
        getline(ss, nama_pasien, ',');
        getline(ss, nama_poli, ',');

        // Mencari nomor BPJS di file akun.csv
        ifstream akunFile("akun.csv");
        string akunLine;

        // Memeriksa apakah file akun.csv berhasil dibuka
        if (!akunFile.is_open())
        {
            cout << "File akun.csv tidak ditemukan atau gagal dibuka." << endl;
            continue;
        }

        bool found = false;
        // Membaca setiap baris dari file akun.csv untuk mencocokkan nama pasien
        while (getline(akunFile, akunLine))
        {
            // Menggunakan stringstream untuk memproses data dari setiap baris
            stringstream akunss(akunLine);
            string akun_nama, akun_no_bpjs;

            // Mendapatkan data dari setiap kolom
            getline(akunss, akun_nama, ',');
            getline(akunss, akun_no_bpjs, ',');

            // Mengecek apakah nama pasien di akun.csv cocok dengan nama pasien di daftar.csv
            if (akun_nama == nama_pasien)
            {
                no_bpjs = akun_no_bpjs;
                found = true;
                break;
            }
        }
        akunFile.close();

        // Menampilkan data pasien jika ditemukan, jika tidak, menampilkan pesan kesalahan
        if (!found)
        {
            cout << "Data untuk pasien dengan nama " << nama_pasien << " tidak ditemukan di akun.csv." << endl;
            continue;
        }

        // Menampilkan data pasien ke layar
        cout << "| " << setw(11) << setfill(' ') << left << id_pasien
             << "| " << setw(15) << setfill(' ') << left << nama_pasien
             << "| " << setw(17) << setfill(' ') << left << nama_poli
             << "| " << setw(16) << setfill(' ') << left << no_bpjs
             << "|" << endl;
    }

    // Menampilkan footer tabel
    cout << "+-------+-------------+-----------------+------------------+-------+" << endl;

    // Menutup file daftar.csv setelah selesai membaca
    daftarFile.close();
}

//FUNGSI UNTUK MEMUAT ULANG DATA POLI DARI FILE POLI.CSV SAAT PROGRAM DIJALANKAN KEMBALI
void loadData_Poli(int &currentID)
{
    // Membuka file poli.csv
    ifstream file("poli.csv");
    
    // Memeriksa apakah file berhasil dibuka
    if (!file.is_open())
    {
        cout << "File tidak ditemukan" << endl;
        return;
    }
    
    // Menginisialisasi headPoli menjadi NULL
    headPoli = NULL;

    string line;

    // Membaca setiap baris dari file poli.csv
    while (getline(file, line))
    {
        // Mengabaikan baris kosong
        if (line.empty())
            continue;

        // Menggunakan stringstream untuk memproses data dari setiap baris
        stringstream iss(line);
        poli *poli2 = new poli; // Membuat node baru untuk poli

        // Membaca dan mengisi data dari setiap kolom
        getline(iss, line, ',');
        poli2->id_poli = stoi(line);
        
        // Memeriksa apakah ID poli baru lebih besar dari currentID dan mengupdate currentID jika ya
        if (poli2->id_poli >= currentID)
        {
            currentID = poli2->id_poli + 1;
        }

        try
        {
            getline(iss, poli2->nama_poli, ',');
            getline(iss, line, ',');
            poli2->jumlah_pasien = stoi(line);
            getline(iss, line, ',');
            poli2->tanggal = stoi(line);
            getline(iss, line, ',');
            poli2->bulan = stoi(line);
            getline(iss, line, ',');
            poli2->waktu_mulai = stoi(line);
            getline(iss, line, ',');
            poli2->waktu_selesai = stoi(line);
            getline(iss, line, '\n');

            // Construct the jadwal_poli string
            poli2->jadwal_poli = to_string(poli2->tanggal) + "/" + to_string(poli2->bulan) +
                                 " " + to_string(poli2->waktu_mulai) + "-" + to_string(poli2->waktu_selesai);
        }
        catch (const std::invalid_argument &e)
        {
            // Menangani kesalahan format file
            cout << e.what() << endl;
            continue; // Lewati baris ini dan lanjutkan ke berikutnya
        }

        // Memasukkan poli ke dalam linked list
        poli2->next = headPoli;
        headPoli = poli2;
    }
    file.close(); // Menutup file setelah selesai membaca
}


//----------BAGIAN PEMANGGILAN FUNGSI------
int main()
{
    system("cls");
    cout << "====================================" << endl;
    cout << "          SELAMAT DATANG DI          " << endl;
    cout << "       PENDAFTARAN ONLINE BPJS       " << endl;
    cout << "====================================" << endl;
    daftar *head = NULL;
    poli *currentPoli = new poli;
    daftar *front = NULL, *rear = NULL;
    int currentID = 1;
    int queuePosition = 1;
    loadData_Poli(currentID);
    loadData_Daftar(&front, &rear, headPoli, currentQueueNumber);
    int pilih, idx, pilihuser, pilihAdmin;

    while (true)
    {
        cout << "===========Masuk Sebagai============" << endl;
        cout << "\t1. Pasien" << endl;
        cout << "\t2. Admin" << endl;
        cout << "\t3. Keluar" << endl;
        cout << "====================================" << endl;
        cout << "Pilih opsi: ";
        cin >> pilih;
        cin.clear();
        cin.ignore();
        if (pilih == 1)
        {
            // Kode untuk Pasien
            while (true)
            {
                loadData_Pasien();
                system("cls");
                cout << "======== MENU PASIEN ========" << endl;
                cout << "\t1. Registrasi" << endl;
                cout << "\t2. Login" << endl;
                cout << "\t3. Keluar" << endl;
                cout << "==============================" << endl;
                cout << "Pilih opsi: ";
                cin >> pilih;
                cin.clear();
                cin.ignore();
                if (pilih == 1)
                {
                    regis_csv();
                }
                else if (pilih == 2)
                {
                    bool loginSukses = loginPasien();
                    if (loginSukses)
                    {
                        int pilihuser;
                        while (true)
                        {
                            cout << "======== MENU PASIEN ========" << endl;
                            cout << "\t1. Daftar" << endl;
                            cout << "\t2. Searching Poli" << endl;
                            cout << "\t3. Lihat Jadwal " << endl;
                            cout << "\t4. Keluar" << endl;
                            cout << "==============================" << endl;
                            cout << "Pilih: ";
                            cin >> pilihuser;
                            cin.clear();
                            cin.ignore();
                            if (pilihuser == 1)
                            {
                                if (pilihuser == 1)
                                {
                                    if (checkAntrianSaya(front, username, PoliName))
                                    {
                                        cout << "Anda Sudah Terdaftar" << endl;
                                        system("pause");
                                        system("cls");
                                    }
                                    else
                                    {
                                        cout << "Nama Poli: ";
                                        getline(cin >> ws, PoliName); // Get the full line, including spaces.
                                        enqueue(&front, &rear, username, PoliName);
                                        // displayAntrian(front);
                                        system("pause");
                                        system("cls");
                                    }
                                }
                            }
                            else if (pilihuser == 2)
                            {
                                poli *head = NULL;
                                string namaPoli;
                                string pat;
                                cout << "Masukkan Nama Poli: ";
                                cin >> namaPoli;
                                boyerMoore(headPoli, namaPoli);
                            }
                            else if (pilihuser == 3)
                            {
                                displayAntrian(front);
                            }
                            else if (pilihuser == 4)
                            {
                                system("cls");
                                break;
                            }
                            else
                            {
                                cout << "Opsi tidak valid. Silakan coba lagi." << endl;
                                system("pause");
                                system("cls");
                            }
                        }
                    }
                }
                else if (pilih == 3)
                {
                    system("cls");
                    break;
                }
                else
                {
                    cout << "Harap pilih sesuai dengan pilihan menu!" << endl;
                    system("pause");
                    system("cls");
                }
            }
        }
        else if (pilih == 2)
        {
            // Admin Section
            bool adminLoggedIn = admin_login();

            if (adminLoggedIn)
            {
                int pilihAdmin;
                poli *head = NULL;

                while (true)
                {
                    cout << "============ Menu Admin ============\n"
                         << endl;
                    cout << "\t1. Create Poli" << endl;
                    cout << "\t2. Display Poli" << endl;
                    cout << "\t3. Sorting Poli" << endl;
                    cout << "\t4. Update Poli" << endl;
                    cout << "\t5. Delete Jadwal Pasien" << endl;
                    cout << "\t6. Rekap Pasien" << endl;
                    cout << "\t7. Kembali ke Menu Utama\n"
                         << endl;
                    cout << "====================================" << endl;
                    cout << "Pilih Menu Admin: ";
                    cin >> pilihAdmin;
                    cin.clear();

                    if (pilihAdmin == 1)
                    {
                        writeFilePoli(head, currentID);
                    }
                    else if (pilihAdmin == 2)
                    {
                        loadData_Poli(currentID);
                        displayPoli(headPoli);
                    }
                    else if (pilihAdmin == 3)
                    {
                        int n = length(headPoli);
                        shellSortASC(&headPoli, n);
                        displayPoli(headPoli);
                        loadData_Poli(currentID);
                    }
                    else if (pilihAdmin == 4)
                    {

                        cout << "Masukkan ID Poli yang akan diperbarui: ";
                        cin >> idx;
                        updatePoli(headPoli, idx);
                        loadData_Poli(currentID);
                    }
                    else if (pilihAdmin == 5)
                    {
                        dequeue(&front);
                        loadAndDisplayDaftar();
                        // Implement logic to delete pasien here
                    }
                    else if (pilihAdmin == 6)
                    {
                        daftar *head = NULL;
                        loadAndDisplayDaftar();
                    }
                    else if (pilihAdmin == 7)
                    {
                        system("cls");
                        break;
                    }
                    else
                    {
                        cout << "Opsi admin tidak valid. Silakan coba lagi." << endl;
                        system("pause");
                        system("cls");
                    }
                }
            }
        }
        else if (pilih == 3)
        {
            cout << "Terima Kasih Sudah Mengunjungi Sistem Ini, Lo Siento!" << endl;
            exit(0);
        }
        else
        {
            cout << "Harap pilih sesuai dengan pilihan menu!" << endl;
            system("pause");
            system("cls");
        }
    }
    return 0;
}
