#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>
using namespace std;

struct Produk {
    string kode;
    string nama;
    float harga;
    int stok;
};
struct ItemTransaksi {
    string kodeProduk;
    int jumlah;
    float subtotal;
};

struct Transaksi {
    string kodeTransaksi;
    string namaPelanggan;
    ItemTransaksi item;
    float totalHarga;
};

void buatTransaksi(vector<Transaksi>& daftarTransaksi, vector<Produk>& daftarProduk);
void simpanTransaksiKeFile(const vector<Transaksi>& daftarTransaksi, const string& namaFile);
void tampilkanSemuaTransaksi(const vector<Transaksi>& daftarTransaksi);
void muatTransaksiDariFile(vector<Transaksi>& daftarTransaksi, const string& namaFile);

void tambahProduk(vector<Produk>& daftarProduk);
void tampilkanProduk(const vector<Produk>& daftarProduk);
void simpanProdukKeFile(const vector<Produk>& daftarProduk, const string& namaFile);
void muatProdukDariFile(vector<Produk>& daftarProduk, const string& namaFile);

void buatTransaksi(vector<Transaksi>& daftarTransaksi, vector<Produk>& daftarProduk) {
    Transaksi t;
    cout << "Masukkan kode transaksi: ";
    cin >> t.kodeTransaksi;
    cout << "Nama pelanggan: ";
    cin.ignore();
    getline(cin, t.namaPelanggan);

    float total = 0.0f;
    char lagi;
    do {
        string kode;
        int jumlah;
        cout << "Kode produk yang dibeli: ";
        cin >> kode;

        Produk* produkPtr = nullptr;
        for (auto& p : daftarProduk) {
            if (p.kode == kode) {
                produkPtr = &p;
                break;
            }
        }

        if (produkPtr) {
            cout << "Jumlah: ";
            cin >> jumlah;

            if (jumlah > produkPtr->stok) {
                cout << "Stok tidak cukup! Stok tersedia: " << produkPtr->stok << "\n";
                continue;
            }

            produkPtr->stok -= jumlah;
            ItemTransaksi item{kode, jumlah, produkPtr->harga * jumlah};
            t.item.push_back(item);
            total += item.subtotal;
        }

        cout << "Tambah produk lain? (y/n): ";
        cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');

    if (total >= 100000) {
        float diskon = total * 0.1f;
        cout << "Diskon 10%: Rp" << diskon << "\n";
        total -= diskon;
    }

    string metode;
    int metodeIn;
    cout << "Pilih metode pembayaran (1.Tunai/2.Transfer/3.E-wallet): ";
    cin >> metodeIn;
    if(metodeIn==1) metode="Tunai";
    else if(metodeIn==2) metode="Transfer";
    else if(metodeIn==3) metode="E-Wallet";


    t.totalHarga = total;
    t.kodeTransaksi += "-" + metode;

    daftarTransaksi.push_back(t);
    cout << "Transaksi berhasil dibuat. Total: Rp" << total << "\n";
}

void simpanTransaksiKeFile(const vector<Transaksi>& daftarTransaksi, const string& namaFile) {
    ofstream file(namaFile);
    if (!file.is_open()) {
        cerr << "Gagal membuka file transaksi.\n";
        return;
    }

    file << "kode_transaksi,nama_pelanggan,kode_produk,jumlah,subtotal,total_harga\n";
    for (const auto& t : daftarTransaksi) {
        for (const auto& item : t.item) {
            file << t.kodeTransaksi << ","
                 << t.namaPelanggan << ","
                 << item.kodeProduk << ","
                 << item.jumlah << ","
                 << item.subtotal << ","
                 << t.totalHarga << "\n";
        }
    }

    file.close();
    cout << "Transaksi berhasil disimpan ke file.\n";
}

void tampilkanSemuaTransaksi(const vector<Transaksi>& daftarTransaksi) {
    cout << "\n=== Riwayat Transaksi ===\n";
    for (const auto& t : daftarTransaksi) {
        cout << "Kode: " << t.kodeTransaksi
             << " | Pelanggan: " << t.namaPelanggan
             << " | Total: Rp" << fixed << setprecision(2) << t.totalHarga << "\n";
        for (const auto& item : t.item) {
            cout << "  - Produk: " << item.kodeProduk
                 << ", Jumlah: " << item.jumlah
                 << ", Subtotal: Rp" << item.subtotal << "\n";
        }
        cout << "-------------------------\n";
    }
}

void muatTransaksiDariFile(vector<Transaksi>& daftarTransaksi, const string& namaFile) {
    ifstream file(namaFile);
    if (!file.is_open()) return;

    string baris;
    getline(file, baris); // Header

    unordered_map<string, Transaksi> mapTransaksi;

    while (getline(file, baris)) {
        stringstream ss(baris);
        string kodeTrx, namaPlg, kodeProduk, jumlahStr, subtotalStr, totalStr;

        getline(ss, kodeTrx, ',');
        getline(ss, namaPlg, ',');
        getline(ss, kodeProduk, ',');
        getline(ss, jumlahStr, ',');
        getline(ss, subtotalStr, ',');
        getline(ss, totalStr, ',');

        ItemTransaksi item{kodeProduk, stoi(jumlahStr), stof(subtotalStr)};
        mapTransaksi[kodeTrx].kodeTransaksi = kodeTrx;
        mapTransaksi[kodeTrx].namaPelanggan = namaPlg;
        mapTransaksi[kodeTrx].item.push_back(item);
        mapTransaksi[kodeTrx].totalHarga = stof(totalStr);
    }

    for (const auto& [_, t] : mapTransaksi) {
        daftarTransaksi.push_back(t);
    }

    file.close();
    cout << "Transaksi berhasil dimuat dari file.\n";
}


void tambahProduk(vector<Produk>& daftarProduk) {
    Produk p;
    cout << "Masukkan kode produk : ";
    cin >> p.kode;
    cout << "Masukkan nama produk : ";
    cin >> p.nama;
    cout << "Masukkan harga produk : ";
    cin >> p.harga;
    cout << "Masukkan jumlah stok : ";
    cin >> p.stok;

    daftarProduk.push_back(p);
    cout << "Produk berhasil ditambahkan.\n";
}

void tampilkanProduk(const vector<Produk>& daftarProduk) {
    cout << "\n=== Daftar Produk ===\n";
    for (const auto& p : daftarProduk) {
        cout << "Kode : " << p.kode
             << " | Nama : " << p.nama
             << " | Harga : " << p.harga
             << " | Stok : " << p.stok << '\n';
    }
    if (daftarProduk.empty()) {
        cout << "Belum ada produk.\n";
    }
}

void simpanProdukKeFile(const vector<Produk>& daftarProduk, const string& namaFile) {
    ofstream file(namaFile);
    if (!file.is_open()) {
        cerr << "Gagal membuka file untuk menulis.\n";
        return;
    }

    file << "kode,nama,harga,stok\n";
    for (const auto& p : daftarProduk) {
        file << p.kode << "," << p.nama << "," << p.harga << "," << p.stok << "\n";
    }

    file.close();
    cout << "Data produk berhasil disimpan ke file.\n";
}

void muatProdukDariFile(vector<Produk>& daftarProduk, const string& namaFile) {
    ifstream file(namaFile);
    if (!file.is_open()) {
        cerr << "Gagal membuka file untuk membaca. Mungkin file belum dibuat.\n";
        return;
    }

    string baris;
    getline(file, baris); // Lewati header

    while (getline(file, baris)) {
        if (baris.empty()) continue; // Lewati baris kosong

        stringstream ss(baris);
        string kode, nama, hargaStr, stokStr;

        getline(ss, kode, ',');
        getline(ss, nama, ',');
        getline(ss, hargaStr, ',');
        getline(ss, stokStr, ',');

        if (kode.empty() || nama.empty() || hargaStr.empty() || stokStr.empty()) {
            cerr << "Baris tidak valid dilewati: " << baris << '\n';
            continue; // Lewati baris tidak lengkap
        }

        try {
            Produk p;
            p.kode = kode;
            p.nama = nama;
            p.harga = stof(hargaStr);
            p.stok = stoi(stokStr);
            daftarProduk.push_back(p);
        } catch (...) {
            cerr << "Gagal parsing baris: " << baris << '\n';
            continue;
        }
    }

    file.close();
    cout << "Data produk berhasil dimuat dari file.\n";
}

int main() {
    vector<Produk> daftarProduk;
    vector<Transaksi> daftarTransaksi;
    string fileProduk = "produk.csv";
    string fileTransaksi = "transaksi.csv";

    muatProdukDariFile(daftarProduk, fileProduk);
    muatTransaksiDariFile(daftarTransaksi, fileTransaksi);

    int pilihan;
    do {
        cout << "\n=== Menu Manajemen Toko ===\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Lihat Produk\n";
        cout << "3. Buat Transaksi\n";
        cout << "4. Lihat Transaksi\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahProduk(daftarProduk); break;
            case 2: tampilkanProduk(daftarProduk); break;
            case 3: buatTransaksi(daftarTransaksi, daftarProduk); break;
            case 4: tampilkanSemuaTransaksi(daftarTransaksi); break;
            case 0:
                simpanProdukKeFile(daftarProduk, fileProduk);
                simpanTransaksiKeFile(daftarTransaksi, fileTransaksi);
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}
