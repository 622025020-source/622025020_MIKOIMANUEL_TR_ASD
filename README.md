# Sistem Tiket Transportasi Antarkota (TR ASD 2026)

Aplikasi konsol berbasis Bahasa C untuk pemesanan tiket transportasi antarkota yang menerapkan seluruh materi Praktikum Algoritma dan Struktur Data (Topik 1–8).

## Identitas
- **Nama**: Nama Lengkap Anda
- **NIM**: NIM Anda

## Fitur & Penerapan Struktur Data
1. **Refreshing C**: Menu interaktif berbasis fungsi modular dengan validasi input aman.
2. **Array, Pointer, & File**: Array dinamis (`malloc`/`realloc`/`free`) dan penyimpanan persisten ke berkas `.txt` di folder `data/`.
3. **Sorting**: Bubble Sort, Selection Sort, dan Insertion Sort (Ascending & Descending).
4. **Stack & Queue**: Stack untuk fitur *Undo* pemesanan sesi, Queue untuk antrean tunggu (*Waiting List*).
5. **Searching**: Sequential Search (kota asal/tujuan) dan Binary Search (pencarian ID rute terurut).
6. **Hash Table**: Manajemen akun (`users.txt`) dengan fungsi hash *djb2* dan penanganan collision *separate chaining*.
7. **Binary Search Tree (BST)**: Indeks ID rute dengan operasi insert, search, delete, serta traversal Pre-Order, In-Order, dan Post-Order.
8. **Graph**: Jaringan antarkota dengan penelusuran BFS, DFS, dan pencarian rute transit minimum.

## Akun Default Admin
- **Username**: `admin`
- **Password**: `admin123`

## Cara Kompilasi & Menjalankan di VS Code
Buka folder proyek di Visual Studio Code, lalu tekan **`Ctrl + Shift + B`** (atau jalankan perintah berikut di terminal VS Code):

```bash
gcc src/*.c -Iinclude -o app
./app
```
"# 622025020_MIKOIMANUEL_TR_ASD" 
