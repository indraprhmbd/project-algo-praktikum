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
    itemTransaksi item[100];
};

itemTransaksi dataItem[MAX_ITEM];
transaksi dataTransaksi[MAX_TRANSAKSI];
produk dataProduk[MAX_PRODUK];


int jumlahProduk = 0;
int jumlahTransaksi = 0;

string formatting(string& input_str){
    for(char& s : input_str ){
        if (s=='_') s=' ';
        else if(s==' ') s='_';
    }
    return input_str;
}


void muatProdukDariFile(){
    ifstream file(produk_filename);
    file.seekg(0);

    jumlahProduk = 0;
    string line;
    string kode,nama,harga_str,stok_str;
    int harga,stok;

    while(getline(file,line)){
        stringstream ss(line);
        getline(ss,kode,',');
        getline(ss,nama,',');
        getline(ss,harga_str,',');
        getline(ss,stok_str,',');
        
        harga = stoi(harga_str);
        stok = stoi(stok_str);
        kode = formatting(kode);
        nama = formatting(nama);

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
        int* harga = &dataProduk[i].harga;
        int* stok = &dataProduk[i].stok;

        *kode = formatting(*kode);
        *nama = formatting(*nama);

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

void cetakTransaksiKeFile(){
    ofstream tOut(transaksi_filename, ios::app);
    if(!tOut.is_open()){
        cout << "FILE : " << transaksi_filename << " gagal dibuka!" <<endl;
        return;
    }

    string kode = dataTransaksi[jumlahTransaksi].kodeTransaksi;
    string nama = dataTransaksi[jumlahTransaksi].namaPelanggan;
    long harga = dataTransaksi[jumlahTransaksi].totalHarga;
    int jumlah = dataTransaksi[jumlahTransaksi].jumlahItem;

    tOut << kode << "," <<nama<< "," << harga << "," << jumlah << ",";
     
    for(int i=0;i<=jumlah;i++){
        string iKode = dataTransaksi[jumlahTransaksi].item[jumlah].kodeProduk;
        int iJumlah = dataTransaksi[jumlahTransaksi].item[jumlah].jumlah;
        int iSubtotal = dataTransaksi[jumlahTransaksi].item[jumlah].subtotal; 
        
        tOut << iKode << "," << iJumlah << "," << iSubtotal ;
    }

    tOut.close();
}

void buatTransaksi(){
    int jumlah,
        jumlahItem = 0;
    string tKode,tNama,pKode;
    char loop;
    bool produkDitemukan = false;

    lihatProduk();
    cout<<endl;
    
    cout << "Masukkan kode transaksi: "; cin >> tKode;
    cout << "Nama pelanggan: "; cin.ignore(); getline(cin,tNama);

    dataTransaksi[jumlahTransaksi].kodeTransaksi = tKode;
    dataTransaksi[jumlahTransaksi].namaPelanggan = formatting(tNama);

    
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
    
        do{
            cout<<"Jumlah : ";cin>>jumlah;
            if (jumlah > Pptr->stok) {
                cout << "Stok tidak cukup! Stok tersedia: " << Pptr->stok << "\n";
            }else 
                break;
        }while(true);
        
        dataTransaksi[jumlahTransaksi].item[jumlahItem].kodeProduk = pKode ;
        dataTransaksi[jumlahTransaksi].item[jumlahItem].jumlah = jumlah ; 
        dataTransaksi[jumlahTransaksi].item[jumlahItem].subtotal =  Pptr->harga * jumlah ;
        
        cout << "Tambah Produk Lain (y/n): "; cin >> loop;
        jumlahItem++;
    }while((loop=='y'));

    //total harga
    long totalHarga = 0;
    for(int i=0;i<=jumlahItem;i++){
        int* hargaPerItem = &dataTransaksi[jumlahTransaksi].item[i].subtotal;
        totalHarga += *hargaPerItem;
    }

    dataTransaksi[jumlahTransaksi].totalHarga = totalHarga;
    dataTransaksi[jumlahTransaksi].jumlahItem = jumlahItem ;

    cetakTransaksiKeFile();
}

void muatTransaksiDariFile(){

    ifstream tIn(transaksi_filename);
    if(!tIn.is_open()){
        cout << "FILE : " << transaksi_filename << " gagal dibuka!" << endl;
        return;
    }

    int* a = &jumlahTransaksi;

    string line,tKode,tNama,tHarga_str,tJumlah_str;
    string iKode,iJumlah_str,iSubtotal_str;
    long tHarga,iSubtotal;
    int tJumlah,iJumlah;

    tIn.seekg(0);
    while(getline(tIn,line)){
        stringstream ss(line);
        
        getline(ss,tKode,',');
        getline(ss,tNama,',');
        getline(ss,tHarga_str,',');
        getline(ss,tJumlah_str,',');
    
        tHarga = stol(tHarga_str);
        tJumlah = stoi(tJumlah_str);
        
        dataTransaksi[*a].kodeTransaksi = tKode ;
        dataTransaksi[*a].namaPelanggan = tNama;
        dataTransaksi[*a].totalHarga = tHarga;
        dataTransaksi[*a].jumlahItem = tJumlah;
        

        for(int i=0;i<tJumlah;i++){
            getline(ss,iKode,',');
            getline(ss,iJumlah_str,',');
            getline(ss,iSubtotal_str,',');

            iJumlah = stoi(iJumlah_str);
            iSubtotal = stoi(iSubtotal_str);

            dataTransaksi[*a].item[i].kodeProduk = iKode;
            dataTransaksi[*a].item[i].jumlah = iJumlah;
            dataTransaksi[*a].item[i].subtotal = iSubtotal;
        }
        *a++;
    }
    tIn.close();
}

void lihatTransaksi(){
    if (jumlahTransaksi == 0) {
        cout << "Belum ada transaksi." << endl;
        return;
    }
    
    cout<<"\n==========Daftar Transaksi=========="<<endl;
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
    int cari;

    cout<<"============CARI PRODUK============"<<endl;
    cout<<"Cari berdasarkan : "<<endl;
    cout<<"1. Sequential Search"<<endl;
    cout<<"2. Binary Search"<<endl;
    cout<<"Pilih metode pencarian (1/2): "; cin>>cari;
    if(cari == 1){
        string kode;
        cout<<"Masukkan kode produk yang dicari: "; cin>>kode;
        bool found = false;

        for(int i=0; i<jumlahProduk; i++){
            if(dataProduk[i].kode == kode){
                cout<<"Produk ditemukan!"<<endl;
                cout<<"Kode  : "<<dataProduk[i].kode<<endl;
                cout<<"Nama  : "<<dataProduk[i].nama<<endl;
                cout<<"Harga : Rp."<<dataProduk[i].harga<<endl;
                cout<<"Stok  : "<<dataProduk[i].stok<<endl;
                found = true;
                break;
            }
        }
        if(!found) cout<<"Produk tidak ditemukan!"<<endl;
    
    }else if(cari == 2){
        bool sorting = false;
        for(int i=0; i<jumlahProduk-1; i++){
            for(int j=i+1; j<jumlahProduk; j++){
                if(dataProduk[i].kode > dataProduk[j].kode){
                    swap(dataProduk[i], dataProduk[j]);
                }
            }
        }
        sorting = true;

        string kode;
        cout<<"Masukkan kode produk yang dicari: "; cin>>kode;
        int left = 0, right = jumlahProduk - 1;
        bool found = false;
        while(left <= right){
            int mid = left + (right - left) / 2;
            if(dataProduk[mid].kode == kode){
                cout<<"Produk ditemukan!"<<endl;
                cout<<"Kode  : "<<dataProduk[mid].kode<<endl;
                cout<<"Nama  : "<<dataProduk[mid].nama<<endl;
                cout<<"Harga : Rp."<<dataProduk[mid].harga<<endl;
                cout<<"Stok  : "<<dataProduk[mid].stok<<endl;
                found = true;
                break;
            }else if(dataProduk[mid].kode < kode){
                left = mid + 1;
            }else{
                right = mid - 1;
            }
        }
        if(!found) cout<<"Produk tidak ditemukan!"<<endl;
    }else{
        cout<<"Pilihan tidak valid!"<<endl;
    }
}

void hapusProduk(){
    string kode;
    cout<<"Masukkan kode produk yang ingin dihapus: "; cin>>kode;

    for(int i=0; i<jumlahProduk; i++){
        if(dataProduk[i].kode == kode){
            for(int j=i; j<jumlahProduk-1; j++){
                dataProduk[j] = dataProduk[j+1];
            }
            jumlahProduk--;
            cout<<"Produk dengan kode "<<kode<<" berhasil dihapus."<<endl;
            cetakProdukKeFile();
            return;
        }
    }
    cout<<"Produk dengan kode "<<kode<<" tidak ditemukan."<<endl;
}


int main(){
    muatProdukDariFile();
    muatTransaksiDariFile();
    int pil;
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
    cout<<"| 0. Keluar                                                   |"<<endl;
    cout<<"==============================================================="<<endl;
    cout<<"Pilih menu: "; cin >> pil;
    switch(pil){
        case 1:
            tambahProduk();
            break;
        case 2:
            lihatProduk();
            break;
        case 3:
            if(jumlahProduk==0){
                cout << "Belum ada produk!" << endl;
                break;
            }
            buatTransaksi();
            break;
        case 4:
            if(jumlahTransaksi==0){
                cout << "Belum ada transaksi!" << endl;
                break;
            }
            lihatTransaksi();
            break;
        case 5:
            cariProduk();
            break;
        case 6:
            hapusProduk();
            break;
        case 0:
            cout << "Terima kasih telah menggunakan program ini.\n";
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";    
            break;
        }
        system("pause");
} while (pil != 0);

    return 0;
}