#include<iostream>
using namespace std;

struct produk {
    string kode;
    string nama;
    float harga;
    int stok;
};
produk dataProduk[100];

struct itemTransaksi {
    string kodeProduk;
    int jumlah;
    float subtotal;
};
itemTransaksi dataItem[100];

struct transaksi {
    string kodeTransaksi;
    string namaPelanggan;
    itemTransaksi item[100];
    int jumlahItem;
    float totalHarga;
};
transaksi dataTransaksi[100];

int jumlahProduk = 0;
int jumlahTransaksi = 0;

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

    string kodeProduk;
    cout << "Masukkan kode transaksi: "; cin >> dataTransaksi[jumlahTransaksi].kodeTransaksi;
    cout << "Nama pelanggan: "; cin.ignore();getline(cin, dataTransaksi[jumlahTransaksi].namaPelanggan);
    cout << "Masukkan kode produk yang dibeli: "; cin >> kodeProduk;
    bool produkDitemukan = false;
    for (int i = 0; i < jumlahProduk; i++) {
        if (dataProduk[i].kode == kodeProduk) {
            produkDitemukan = true;
            dataTransaksi[jumlahTransaksi].item[0].kodeProduk = dataProduk[i].kode;
            cout << "Masukkan jumlah: "; cin >> dataTransaksi[jumlahTransaksi].item[0].jumlah;
            dataTransaksi[jumlahTransaksi].item[0].subtotal = dataProduk[i].harga * dataTransaksi[jumlahTransaksi].item[0].jumlah;
            dataTransaksi[jumlahTransaksi].totalHarga += dataTransaksi[jumlahTransaksi].item[0].subtotal;
            cout << "Subtotal: Rp." << dataTransaksi[jumlahTransaksi].item[0].subtotal << endl;
            produkDitemukan = true;
            break;
        }
    }

    
    if (!produkDitemukan) {
        cout << "Produk dengan kode " << kodeProduk << " tidak ditemukan.\n";
        return;
    }

    // yang buat mengurangi stok belum okeeee

}



   





int main(){
    do{
    int pil;

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
    }
} while (pil != 0);

    return 0;
}