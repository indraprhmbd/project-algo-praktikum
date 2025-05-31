#include<iostream>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_PRODUK = 100;
const int MAX_TRANSAKSI = 200;
const int MAX_ITEM = 100;
string produk_filename = "produk.csv";
string transaksi_filename = "transaksi.csv";
string akun_filename = "akun.txt";

struct produk {
    string kode;
    string nama;
    float harga;
    int stok;
};

struct itemTransaksi {
    string kodeProduk;
    int jumlah;
    float subtotal;
};

struct transaksi {
    string kodeTransaksi;
    string namaPelanggan;
    int jumlahItem;
    float totalHarga;
    itemTransaksi item[100];
};

itemTransaksi dataItem[MAX_ITEM];
transaksi dataTransaksi[MAX_TRANSAKSI];
produk dataProduk[MAX_PRODUK];


int jumlahProduk = 0;
int jumlahTransaksi = 0;

void formatting(string& input_str){
    for(char& s : input_str ){
        if (s=='_') s=' ';
        else if(s==' ') s='_';
    }
}


void muatProdukDariFile(){
    ifstream file(produk_filename);
    file.seekg(0);

    jumlahProduk = 0;
    string line;
    string kode,nama,harga_str,stok_str;
    float harga;
    int stok;

    while(getline(file,line)){
        stringstream ss(line);
        getline(ss,kode,',');
        getline(ss,nama,',');
        getline(ss,harga_str,',');
        getline(ss,stok_str,',');
        
        harga = stof(harga_str);
        stok = stoi(stok_str);
        formatting(kode);
        formatting(nama);

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
    if(!out.is_open()){
        cout<<"file : "<<produk_filename<<" gagal dibuka!"<<endl;
        return;
    }

    for(int i=0; i < jumlahProduk ; i++ ){
        string* kode = &dataProduk[i].kode;
        string* nama = &dataProduk[i].nama;
        float* harga = &dataProduk[i].harga;
        int* stok = &dataProduk[i].stok;

        formatting(*kode);
        formatting(*nama);

        out << *kode << ","
            << *nama << ","
            << *harga << ","
            << *stok <<endl;
    }
    out.close();
}

void tambahProduk(){
    cout << "Masukkan kode produk: ";
    cin >> dataProduk[jumlahProduk].kode;
    cout << "Masukkan nama produk: ";
    cin.ignore();
    getline(cin, dataProduk[jumlahProduk].nama);
    cout << "Masukkan harga produk: ";
    cin >> dataProduk[jumlahProduk].harga;
    cout << "Masukkan jumlah stok: ";
    cin >> dataProduk[jumlahProduk].stok;
    jumlahProduk++;
    cout << "Produk berhasil ditambahkan.\n";

    cetakProdukKeFile();
}

void lihatProduk(){
    if (jumlahProduk == 0) {
        cout << "Produk belum tersedia."<<endl;
        return;
    }
    cout<<"\n==========Daftar Produk=========="<<endl;
    for(int i = 0; i < jumlahProduk; i++){
        cout<<"Produk ke-"<<i+1<<endl;
        cout << "Kode  : " << dataProduk[i].kode<<endl;
        cout << "Nama  : " << dataProduk[i].nama<<endl;
        cout << "Harga : Rp." << dataProduk[i].harga <<endl;
        cout << "Stok  : " << dataProduk[i].stok << endl;
        cout<<"-------------------------------------------"<<endl;
    }
}

void buatTransaksi(){
    int jumlah,index;
    string tKode,tNama,pKode;
    char loop;
    bool produkDitemukan = false;
    
    cout << "Masukkan kode transaksi: "; cin >> tKode;
    cout << "Nama pelanggan: "; cin.ignore();getline(cin,tNama);

    dataTransaksi[jumlahTransaksi].kodeTransaksi = tKode;
    dataTransaksi[jumlahTransaksi].namaPelanggan = tNama;
    
    produk* Pptr = nullptr ;
    do{
        cout << "Masukkan kode produk yang dibeli: "; cin >> pKode;
        for (int i = 0; i < jumlahProduk; i++) {
            if (dataProduk[i].kode == pKode) {
                produkDitemukan = true;
                Pptr = &dataProduk[i];
                break;
            }
        }
        
        if (!produkDitemukan) {
            cout << "Produk dengan kode " << pKode << " tidak ditemukan.\n";
            return;
        }
    
        cout<<"Jumlah : ";cin>>jumlah;
        if (jumlah > Pptr->stok) {
            cout << "Stok tidak cukup! Stok tersedia: " << Pptr->stok << "\n";
        }
        //belum selesai di sini
        
        
        cout << "Tambah Produk Lain (y/n): "; cin >> loop;
    }while(loop=='y');

}

int main(){
    muatProdukDariFile();
    int pil;
    do{

    cout<<"===============================================================";
    cout<<"|             MANAJEMEN TOKO 'BIYAN DAN MAS ARSYA             |"<<endl;
    cout<<"==============================================================="<<endl;
    cout<<"| 1. Tambah Produk                                            |"<<endl;
    cout<<"| 2. Lihat Produk                                             |"<<endl;
    cout<<"| 3. Buat Transaksi                                           |"<<endl;
    cout<<"| 4. Lihat Transaksi                                          |"<<endl;
    cout<<"| 5. Cari Produk                                              |"<<endl;
    cout<<"| 6. Hapus Produk                                             |"<<endl;
    cout<<"| 0. Keluar                                                   |"<<endl;
    cout<<"==============================================================="<<endl;
    cout<<"Pilih menu: "; cin>>pil;
    switch(pil){
        case 1:
            tambahProduk();
            break;
        case 2:
           lihatProduk();
            break;
        case 3:
            buatTransaksi();
            break;
        case 4:
            // lihatTransaksi();
            break;
        case 5:
            // cariProduk();
            break;
        case 6:
            // hapusProduk();
            break;
        case 0:
            cout << "Terima kasih telah menggunakan program ini.\n";
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";    
    }
} while (pil != 0);

    return 0;
}