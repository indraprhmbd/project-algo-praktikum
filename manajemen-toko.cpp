#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

const int MAX_PRODUK = 250;
const int MAX_TRANSAKSI = 200;
const int MAX_ITEM = 100;

string produk_filename = "produk.csv";
string transaksi_filename = "transaksi.csv";
string log_transaksi_filename = "transaksi_log.txt";

struct produk {
    string kode;
    string nama;
    int harga;
    int stok;
};

struct itemTransaksi {
    string kodeProduk;
    int jumlah;
    int subtotal;
};

struct transaksi {
    string kodeTransaksi;
    string namaPelanggan;
    int jumlahItem;
    long totalHarga;
    itemTransaksi item[MAX_ITEM];
};

transaksi dataTransaksi[MAX_TRANSAKSI];
produk dataProduk[MAX_PRODUK]; 

int jumlahProduk = 0;
int jumlahTransaksi = 0;

string formatting(const string& input_str) {
    string result = input_str;
    for (char& c : result) {
        if (c == '_') c = ' ';
        else if (c == ' ') c = '_';
    }
    return result;
}
string toLower(const string& str) {
    string lower = str;
    for (char& c : lower) c = tolower(c);
    return lower;
}
string toUpper(const string& str) {
    string upper = str;
    for (char& c : upper) c = toupper(c);
    return upper;
}

void muatProdukDariFile(){
    ifstream file(produk_filename);

    jumlahProduk = 0;
    string line;
    string kode,nama,harga_str,stok_str;
    int harga,stok;

    while(getline(file,line)){
        if(line==" ") break;
        stringstream ss(line);
        getline(ss,kode,',');
        getline(ss,nama,',');
        getline(ss,harga_str,',');
        getline(ss,stok_str,',');
        
        kode = formatting(kode);
        nama = formatting(nama);
        harga = stoi(harga_str);
        stok = stoi(stok_str);

        dataProduk[jumlahProduk].kode = kode;
        dataProduk[jumlahProduk].nama = nama;
        dataProduk[jumlahProduk].harga = harga;
        dataProduk[jumlahProduk].stok = stok;

        jumlahProduk++;
    }
    file.close();
}

void cetakProdukKeFile(){
    ofstream out(produk_filename);
    if (!out.is_open()) {
        cout << "File: " << produk_filename << " gagal dibuka!\n";
        return;
    }

    for (int i = 0; i < jumlahProduk; i++) {
        
        string kode = formatting(dataProduk[i].kode);
        string nama = formatting(dataProduk[i].nama);
        int harga = dataProduk[i].harga;
        int stok  = dataProduk[i].stok;

        out << kode << ","
            << nama << ","
            << harga << ","
            << stok << endl;
    }

    out.close();
}


bool kodeSudahAda(const string& kode) {
    for (int i = 0; i < jumlahProduk; i++) {
        if (dataProduk[i].kode == kode) return true;
    }
    return false;
}

void tambahProduk(){
    string kode;
    cout << "Masukkan kode produk: ";
    cin.ignore();
    getline(cin,kode);
    
    if (kodeSudahAda(kode)) {
        cout << "Kode produk sudah digunakan. Gagal menambahkan produk.\n";
        return;
    }
    dataProduk[jumlahProduk].kode = kode;

    cout << "Masukkan nama produk: ";
    getline(cin, dataProduk[jumlahProduk].nama);
    cout << "Masukkan harga produk: ";
    cin >> dataProduk[jumlahProduk].harga;
    cout << "Masukkan jumlah stok: ";
    cin >> dataProduk[jumlahProduk].stok;
    cout << "Produk berhasil ditambahkan.\n";
    jumlahProduk++;

    cetakProdukKeFile();
}

void lihatProduk(){
  
    cout<<"\n==========Daftar Produk=========="<<endl;
    cout << left << setw(8) << "Kode" << setw(25) << "Nama" << setw(10) << "Harga" << setw(8) << "Stok"
        << setw(8) << "Kode" << setw(25) << "Nama" << setw(10) << "Harga" << "Stok" << endl;
    cout << string(100, '-') << endl;

    for(int i = 0; i < jumlahProduk; i += 2){
        // Produk pertama (selalu ada)
        cout << left << setw(8) << dataProduk[i].kode 
            << setw(25) << dataProduk[i].nama 
            << "Rp." << setw(7) << dataProduk[i].harga 
            << setw(8) << dataProduk[i].stok;
        
        // Produk kedua (jika ada)
        if(i + 1 < jumlahProduk){
            cout << setw(8) << dataProduk[i+1].kode 
                << setw(25) << dataProduk[i+1].nama 
                << "Rp." << setw(7) << dataProduk[i+1].harga 
                << dataProduk[i+1].stok;
        }
        
        cout << endl;
    }
    cout << string(100, '-') << endl;
}

void logTransaksiKeFile(const transaksi& t){
    ofstream log(log_transaksi_filename, ios::app);
    if (!log.is_open()) return;

    time_t now = time(0);
    char* dt = ctime(&now); 

    log << "Waktu: " << dt;
    log << "Kode Transaksi : " << t.kodeTransaksi << endl;
    log << "Nama Pelanggan : " << t.namaPelanggan << endl;
    log << "Total Harga    : Rp." << t.totalHarga << endl;
    log << "Jumlah Item    : " << t.jumlahItem << endl;
    log << "----------------------------------------\n";
    log.close();
}


void cetakTransaksiKeFile(){
    if(jumlahTransaksi <= 0){
        cout << "Tidak ada transaksi untuk dicetak!" << endl;
        return;
    }
    
    ofstream tOut(transaksi_filename, ios::app);
    if(!tOut.is_open()){
        cout << "FILE : " << transaksi_filename << " gagal dibuka!" <<endl;
        return;
    }

    // Gunakan index terakhir (jumlahTransaksi - 1)
    int indexTerakhir = jumlahTransaksi - 1;
    
    string* kode = &dataTransaksi[indexTerakhir].kodeTransaksi;
    string* nama = &dataTransaksi[indexTerakhir].namaPelanggan;
    long* harga = &dataTransaksi[indexTerakhir].totalHarga;
    int* jumlah = &dataTransaksi[indexTerakhir].jumlahItem;

    tOut << formatting(*kode) << "," << formatting(*nama) << "," << *harga << "," << *jumlah;
     
    for(int i=0;i<*jumlah;i++){
        string* iKode = &dataTransaksi[indexTerakhir].item[i].kodeProduk;
        int* iJumlah = &dataTransaksi[indexTerakhir].item[i].jumlah;
        int* iSubtotal = &dataTransaksi[indexTerakhir].item[i].subtotal; 
        
        tOut << "," << *iKode << "," << *iJumlah << "," << *iSubtotal ;
    }
    tOut << endl;

    tOut.close();
}

void buatTransaksi(){
    int jumlah = 0,
        itemIndex = 0;
    string tKode,tNama,pKode;
    char loop;
    bool produkDitemukan;

    lihatProduk();
    cout<<endl;
    
    cout << "Masukkan kode transaksi: "; cin.ignore();getline(cin,tKode);
    cout << "Nama pelanggan: "; getline(cin,tNama);

    dataTransaksi[jumlahTransaksi].kodeTransaksi = tKode;
    dataTransaksi[jumlahTransaksi].namaPelanggan = tNama;

    produk* Pptr = nullptr ;
    do{
        produkDitemukan = false;
        cout << "Masukkan kode produk yang dibeli: "; cin >> pKode;
        for (int i = 0; i < jumlahProduk; i++) {
            if (dataProduk[i].kode == toUpper(pKode)) {
                produkDitemukan = true;
                Pptr = &dataProduk[i];
                break;
            }
        }
        
        if (produkDitemukan) {
            do{
                cout<<"Jumlah : ";cin>>jumlah;
                if (jumlah > Pptr->stok) {
                    cout << "Stok tidak cukup! Stok tersedia: " << Pptr->stok << "\n";
                }else 
                    break;
            }while(true);
    
            Pptr->stok -= jumlah;
    
            itemTransaksi* Tptr = &dataTransaksi[jumlahTransaksi].item[itemIndex];
    
            Tptr->kodeProduk = toUpper(pKode) ;
            Tptr->jumlah = jumlah ; 
            Tptr->subtotal =  Pptr->harga * jumlah ;
            
            cout << "Tambah Produk Lain (y/n): "; cin >> loop;
            itemIndex++;
        }
        else {
            loop = 'y';
            cout << "Produk dengan kode " << pKode << " tidak ditemukan.\n";
        }
    }while(loop=='y');

    //total harga
    long totalHarga = 0;
    for(int i=0;i<itemIndex;i++){
        totalHarga += dataTransaksi[jumlahTransaksi].item[i].subtotal;
    }
    transaksi* Tptr = &dataTransaksi[jumlahTransaksi];
    Tptr->totalHarga = totalHarga; 
    Tptr->jumlahItem = itemIndex ;

    
    cout << "\n========== DETAIL TRANSAKSI ==========\n";
    cout << "Kode Transaksi : " << Tptr->kodeTransaksi << endl;
    cout << "Nama Pelanggan : " << Tptr->namaPelanggan << endl;
    cout << "Total Harga    : Rp." << Tptr->totalHarga << endl;
    cout << "Jumlah Item    : " << Tptr->jumlahItem << endl;

    for(int i = 0; i < itemIndex; i++){
        cout << "Item ke-" << i+1 << endl;
        cout << "Kode Produk : " << dataTransaksi[jumlahTransaksi].item[i].kodeProduk << endl;
        cout << "Jumlah      : " << dataTransaksi[jumlahTransaksi].item[i].jumlah << endl;
        cout << "Subtotal    : Rp." << dataTransaksi[jumlahTransaksi].item[i].subtotal << endl;
        cout << "--------------------------------------" << endl;
    }

    cout << "=====================================\n";
    cout << "Transaksi berhasil dibuat.\n";
    
    jumlahTransaksi++;
    cetakProdukKeFile();
    cetakTransaksiKeFile();
    logTransaksiKeFile(*Tptr);
}

bool isNumber(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void muatTransaksiDariFile() {
    ifstream tIn(transaksi_filename);
    if (!tIn.is_open()) {
        cout << "FILE : " << transaksi_filename << " gagal dibuka!" << endl;
        return;
    }

    string line;
    while (getline(tIn, line)) {
        if (jumlahTransaksi >= MAX_TRANSAKSI) {
            cout << "Kapasitas transaksi penuh.\n";
            break;
        }

        stringstream ss(line);
        string tKode, tNama, tHarga_str, tJumlah_str;
        string iKode, iJumlah_str, iSubtotal_str;
        long tHarga;
        int tJumlah;

        getline(ss, tKode, ',');
        getline(ss, tNama, ',');
        getline(ss, tHarga_str, ',');
        getline(ss, tJumlah_str, ',');

        if (!isNumber(tHarga_str) || !isNumber(tJumlah_str)) {
            cout << "Data transaksi rusak, dilewati.\n";
            continue;
        }

        tHarga = stol(tHarga_str);
        tJumlah = stoi(tJumlah_str);

        transaksi& t = dataTransaksi[jumlahTransaksi];
        t.kodeTransaksi = formatting(tKode);
        t.namaPelanggan = formatting(tNama);
        t.totalHarga = tHarga;
        t.jumlahItem = tJumlah;

        for (int i = 0; i < tJumlah; i++) {
            getline(ss, iKode, ',');
            getline(ss, iJumlah_str, ',');
            getline(ss, iSubtotal_str, ',');

            if (!isNumber(iJumlah_str) || !isNumber(iSubtotal_str)) {
                cout << "Data item rusak di transaksi " << tKode << ", dilewati.\n";
                break;
            }

            t.item[i].kodeProduk = iKode;
            t.item[i].jumlah = stoi(iJumlah_str);
            t.item[i].subtotal = stoi(iSubtotal_str);
        }

        jumlahTransaksi++;
    }

    tIn.close();
}


void lihatTransaksi(){
    
    cout<<"\n========== Daftar Transaksi =========="<<endl;
    for(int i = 0; i < jumlahTransaksi; i++){
        cout<<"Transaksi ke-"<<i+1<<endl;
        cout << "Kode Transaksi : " << dataTransaksi[i].kodeTransaksi << endl;
        cout << "Nama Pelanggan : " << dataTransaksi[i].namaPelanggan << endl;
        cout << "Total Harga    : Rp." << dataTransaksi[i].totalHarga << endl;
        cout << "Jumlah Item   : " << dataTransaksi[i].jumlahItem << endl;

        for(int j = 0; j < dataTransaksi[i].jumlahItem; j++){
            cout<<"Item ke-"<<j+1<<endl;
            cout<<"Kode Produk  : "<<dataTransaksi[i].item[j].kodeProduk<<endl;
            cout<<"Jumlah       : "<<dataTransaksi[i].item[j].jumlah<<endl;
            cout<<"Subtotal     : Rp."<<dataTransaksi[i].item[j].subtotal<<endl;
            cout<<"-------------------------------------------"<<endl;
        }
        cout<<"==========================================="<<endl;
    }
}



void cariProduk(){
    cout << "============ CARI PRODUK ============\n";

    //bubblesort, kode
    for (int i = 0; i < jumlahProduk - 1; i++) {
        for (int j = i + 1; j < jumlahProduk; j++) {
            if (dataProduk[i].kode > dataProduk[j].kode) {
                swap(dataProduk[i], dataProduk[j]);
            }
        }
    }
    
    string keyword;
    cout << "Masukkan kode/nama produk yang dicari : ";
    cin.ignore();
    getline(cin, keyword);
    
    bool found = false;
    
    for (int i = 0; i < jumlahProduk; i++) {
        //case sensitive
        if (toLower(dataProduk[i].kode).find(toLower(keyword)) != string::npos ||
            toLower(dataProduk[i].nama).find(toLower(keyword)) != string::npos) {
    
            cout << "\nProduk ditemukan:\n";
            cout << "Kode  : " << dataProduk[i].kode << endl;
            cout << "Nama  : " << dataProduk[i].nama << endl;
            cout << "Harga : Rp." << dataProduk[i].harga << endl;
            cout << "Stok  : " << dataProduk[i].stok << endl;
            cout << "-----------------------------------\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "Tidak ada produk yang cocok dengan \"" << keyword << "\"\n";
    }
}    

void hapusProduk(){
    if (jumlahProduk == 0) {
        cout << "Tidak ada produk yang dapat dihapus." << endl;
        return;
    } 

    lihatProduk(); 
    cout << "\n========== HAPUS PRODUK ==========" << endl;
    string inputKode;
    cout << "Masukkan (sebagian) kode produk yang ingin dihapus: ";
    cin >> inputKode;

    int* kandidat = new int[50];
    int count=0;
    cout << "\nProduk yang cocok dengan input:\n";
    for (int i = 0; i < jumlahProduk; i++) {
        if (dataProduk[i].kode.find(inputKode) != string::npos) {
            cout << count+1 << ". " << dataProduk[i].kode << " - "
                 << dataProduk[i].nama << " - Rp" << dataProduk[i].harga
                 << " - Stok: " << dataProduk[i].stok << endl;
            kandidat[count] = i;
            count++;
        }
    }

    if (count==0) {
        cout << "Tidak ada produk yang cocok dengan \"" << inputKode << "\"." << endl;
        return;
    }

    int pilihan;
    cout << "Pilih nomor produk yang ingin dihapus [1 - " << count << "]: ";
    cin >> pilihan;

    if (pilihan < 1 || pilihan > count) {
        cout << "Pilihan tidak valid." << endl;
        return;
    }

    int hapusIndex = kandidat[pilihan - 1];
    string kodeHapus = dataProduk[hapusIndex].kode;
    
    // Geser data
    for (int j = hapusIndex; j < jumlahProduk - 1; j++) {
        dataProduk[j] = dataProduk[j + 1];
    }
    jumlahProduk--;
    
    cetakProdukKeFile();
    cout << "Produk dengan kode \"" << kodeHapus << "\" berhasil dihapus." << endl;
    delete[] kandidat;
}


void laporanPendapatan(){
    long totalPendapatan = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        totalPendapatan += dataTransaksi[i].totalHarga;
    }
    cout << "\n========== LAPORAN PENDAPATAN ==========\n";
    cout << "Jumlah Transaksi : " << jumlahTransaksi << endl;
    cout << "Total Pendapatan : Rp." << totalPendapatan << endl;
    cout << "========================================\n";
}

int main(){
    muatProdukDariFile();
    muatTransaksiDariFile();
    int pil;
        string inputUsername, inputPassword;
        string username = "admin";
        string password = "admin123";
        
        int sisa = 3;
        for (int i = 0; i < 3; i++) {
            system("cls");
            cout << "=================== LOGIN ===================" << endl;
            cout << "Username: "; cin >> inputUsername;
            cout << "Password: "; cin >> inputPassword;

            if (inputUsername == username && inputPassword == password) {
                cout << "Login berhasil!" << endl;
                break;
            } else {
                cout << "Username atau password salah. Silakan coba lagi.(Kesempatan mencoba = " <<sisa-i-1<< " lagi)" << endl;
                if (i == 2) {
                    cout << "Anda telah mencoba 3 kali. Program akan keluar." << endl;
                    return 0;
                }
                system("pause");
            }
        }
    
     do{
        system("cls");
    cout<<"==============================================================="<<endl;
    cout<<"|             MANAJEMEN TOKO 'BIYAN DAN MAS ARSYA             |"<<endl;
    cout<<"==============================================================="<<endl;
    cout<<"| 1. Tambah Produk                                            |"<<endl;
    cout<<"| 2. Lihat Produk                                             |"<<endl;
    cout<<"| 3. Buat Transaksi                                           |"<<endl;
    cout<<"| 4. Lihat Transaksi                                          |"<<endl;
    cout<<"| 5. Cari Produk                                              |"<<endl;
    cout<<"| 6. Hapus Produk                                             |"<<endl;
    cout<<"| 7. Laporan Pendapatan                                       |"<<endl;
    cout<<"| 0. Keluar                                                   |"<<endl;
    cout<<"==============================================================="<<endl;
    cout<<"Pilih menu: "; cin >> pil;
    switch(pil){
        case 1:
            tambahProduk(); break;
        case 2:
            if (jumlahProduk == 0) {
                cout << "Produk belum tersedia."<<endl; break;
            }
            lihatProduk(); break;
        case 3:
            if(jumlahProduk==0){
                cout << "Belum ada produk!" << endl; break;
            }
            buatTransaksi(); break;
        case 4:
            if(jumlahTransaksi==0){
                cout << "Belum ada transaksi!" << endl; break;
            }
            lihatTransaksi(); break;
        case 5:
            if(jumlahProduk==0){
                cout<<"Belum ada produk!"<<endl; break;
            }
            cariProduk(); break;
        case 6:
            if(jumlahProduk==0){
                cout<<"Belum ada produk!"<<endl; break;
            }
            hapusProduk(); break;
        case 7 :
            laporanPendapatan(); break;
        case 0 :
            cout << "Terima kasih telah menggunakan program ini.\n";
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n"; break;
        }
        system("pause");
    } while (pil != 0);

    return 0;
}