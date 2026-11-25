#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "util.h"
#include "sorting.h"
#include "searching.h"
#include "service.h"

static void handle_view_sorted_routes(App *app)
{
    int algo, fieldChoice, orderChoice, i;
    Route **copy;

    if (app->routes.count == 0) {
        printf("Belum ada data rute.\n");
        return;
    }
    print_title("DAFTAR & URUTKAN RUTE");
    printf("Pilih Algoritma Sorting:\n");
    printf("1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n");
    algo = read_int("Pilihan (1-3): ", 1, 3);

    printf("\nUrutkan Berdasarkan:\n");
    printf("1. ID Rute\n2. Harga\n3. Jarak\n4. Jam Berangkat\n5. Sisa Kursi\n6. Kota Asal\n");
    fieldChoice = read_int("Pilihan (1-6): ", 1, 6);

    printf("\nUrutan:\n1. Ascending (Naik)\n2. Descending (Turun)\n");
    orderChoice = read_int("Pilihan (1-2): ", 1, 2);

    copy = (Route **)malloc(sizeof(Route *) * (size_t)app->routes.count);
    if (copy == NULL) {
        printf("Memori tidak cukup.\n");
        return;
    }
    for (i = 0; i < app->routes.count; i++) {
        copy[i] = app->routes.items[i];
    }

    if (algo == 1) {
        bubble_sort(copy, app->routes.count, (SortField)(fieldChoice - 1), (SortOrder)orderChoice);
    } else if (algo == 2) {
        selection_sort(copy, app->routes.count, (SortField)(fieldChoice - 1), (SortOrder)orderChoice);
    } else {
        insertion_sort(copy, app->routes.count, (SortField)(fieldChoice - 1), (SortOrder)orderChoice);
    }

    route_print_array(copy, app->routes.count);
    free(copy);
}

static void handle_search_routes(App *app)
{
    int mode;

    print_title("PENCARIAN RUTE");
    printf("1. Cari berdasarkan Kota Asal & Tujuan (Sequential Search)\n");
    printf("2. Cari berdasarkan ID Rute (Binary Search pada Array Terurut)\n");
    printf("3. Cari berdasarkan ID Rute (Binary Search Tree / BST)\n");
    mode = read_int("Pilihan (1-3): ", 1, 3);

    if (mode == 1) {
        char asal[MAX_CITY_NAME], tujuan[MAX_CITY_NAME];
        int results[MAX_ROUTE];
        int found, i;

        read_line("Kota Asal (kosongkan untuk semua): ", asal, sizeof(asal));
        read_line("Kota Tujuan (kosongkan untuk semua): ", tujuan, sizeof(tujuan));
        found = sequential_search_route(app->routes.items, app->routes.count,
                                        asal, tujuan, results, MAX_ROUTE);
        if (found == 0) {
            printf("Rute tidak ditemukan.\n");
            return;
        }
        route_print_header();
        for (i = 0; i < found; i++) {
            route_print_row(app->routes.items[results[i]]);
        }
        print_line();
        printf("Ditemukan %d rute.\n", found);
    } else if (mode == 2) {
        int id = read_int("Masukkan ID Rute: ", 1, MAX_ID);
        Route **copy = (Route **)malloc(sizeof(Route *) * (size_t)app->routes.count);
        int i, idx;

        if (copy == NULL) {
            return;
        }
        for (i = 0; i < app->routes.count; i++) {
            copy[i] = app->routes.items[i];
        }
        insertion_sort(copy, app->routes.count, SORT_BY_ID, ORDER_ASC);
        idx = binary_search_id(copy, app->routes.count, id);
        if (idx == -1) {
            printf("Rute dengan ID %d tidak ditemukan.\n", id);
        } else {
            route_print_header();
            route_print_row(copy[idx]);
            print_line();
        }
        free(copy);
    } else {
        int id = read_int("Masukkan ID Rute: ", 1, MAX_ID);
        Route *r = bst_search(app->routeTree, id);
        if (r == NULL) {
            printf("Rute dengan ID %d tidak ditemukan di BST.\n", id);
        } else {
            route_print_header();
            route_print_row(r);
            print_line();
        }
    }
}

static void handle_book_ticket(App *app)
{
    int id, jumlah, bookingId;
    Route *r;

    print_title("PESAN TIKET");
    route_print_array(app->routes.items, app->routes.count);
    id = read_int("Masukkan ID Rute yang ingin dipesan (0 batal): ", 0, MAX_ID);
    if (id == 0) {
        return;
    }
    r = bst_search(app->routeTree, id);
    if (r == NULL) {
        printf("Rute tidak ditemukan.\n");
        return;
    }
    jumlah = read_int("Jumlah tiket (1-5): ", 1, MAX_TICKET_PER_ORDER);
    if (r->kursi >= jumlah) {
        bookingId = service_create_booking(app, app->current->username, r, jumlah);
        if (bookingId > 0) {
            printf("Pemesanan berhasil! ID Booking: %d | Total Bayar: Rp %d\n",
                   bookingId, r->harga * jumlah);
        } else {
            printf("Gagal memproses pemesanan.\n");
        }
    } else {
        int join;
        printf("Kursi tidak mencukupi (sisa kursi: %d).\n", r->kursi);
        join = read_int("Masuk ke antrean tunggu (Waiting List)? (1=Ya, 0=Tidak): ", 0, 1);
        if (join == 1) {
            if (service_join_waiting(app, app->current->username, r->id, jumlah)) {
                printf("Anda berhasil masuk ke antrean tunggu untuk rute ID %d.\n", r->id);
            }
        }
    }
}

static void handle_my_bookings(App *app)
{
    int i, count = 0;
    const Booking *b;
    const Route *r;

    print_title("RIWAYAT PEMESANAN SAYA");
    print_line();
    printf("%-6s %-8s %-24s %-8s %-12s %s\n",
           "ID", "ID Rute", "Rute", "Jumlah", "Total (Rp)", "Status");
    print_line();
    for (i = 0; i < app->bookings.count; i++) {
        b = &app->bookings.items[i];
        if (strcmp(b->username, app->current->username) == 0) {
            char routeDesc[64] = "(Rute dihapus)";
            int total = 0;
            r = bst_search(app->routeTree, b->routeId);
            if (r != NULL) {
                snprintf(routeDesc, sizeof(routeDesc), "%s-%s", r->asal, r->tujuan);
                total = r->harga * b->jumlah;
            }
            printf("%-6d %-8d %-24s %-8d %-12d %s\n",
                   b->id, b->routeId, routeDesc, b->jumlah, total,
                   b->status == BOOKING_ACTIVE ? "AKTIF" : "DIBATALKAN");
            count++;
        }
    }
    print_line();
    if (count == 0) {
        printf("Belum ada riwayat pemesanan.\n");
    }
}

static void handle_cancel_or_undo(App *app)
{
    int mode;

    print_title("BATALKAN PEMESANAN / UNDO");
    printf("1. Batalkan berdasarkan ID Booking\n");
    printf("2. Undo Pemesanan Terakhir di Sesi Ini (Stack)\n");
    mode = read_int("Pilihan (1-2): ", 1, 2);

    if (mode == 1) {
        int id = read_int("Masukkan ID Booking yang akan dibatalkan: ", 1, MAX_ID);
        int res = service_cancel_booking(app, id, app->current->username);
        if (res == CANCEL_OK) {
            printf("Booking ID %d berhasil dibatalkan.\n", id);
        } else if (res == CANCEL_NOT_FOUND) {
            printf("ID Booking tidak ditemukan.\n");
        } else if (res == CANCEL_NOT_OWNER) {
            printf("Anda tidak berhak membatalkan booking milik akun lain.\n");
        } else {
            printf("Booking tersebut sudah dibatalkan sebelumnya.\n");
        }
    } else {
        int undone = service_undo_last(app, app->current->username);
        if (undone > 0) {
            printf("Undo berhasil! Booking ID %d telah dibatalkan.\n", undone);
        } else {
            printf("Tidak ada pemesanan aktif di sesi ini yang dapat di-undo.\n");
        }
    }
}

static void handle_graph_menu(App *app)
{
    int choice;

    print_title("JARINGAN RUTE & PENELUSURAN KOTA (GRAPH)");
    printf("1. Tampilkan Seluruh Koneksi Kota\n");
    printf("2. Penelusuran BFS dari Kota Awal\n");
    printf("3. Penelusuran DFS dari Kota Awal\n");
    printf("4. Cari Jalur Transit Minimum Antar Dua Kota (BFS Shortest Hop)\n");
    choice = read_int("Pilihan (1-4): ", 1, 4);

    if (choice == 1) {
        graph_print_connections(&app->graph);
    } else if (choice == 2 || choice == 3) {
        char city[MAX_CITY_NAME];
        int idx;
        graph_print_cities(&app->graph);
        read_text("Masukkan nama kota awal: ", city, sizeof(city));
        idx = graph_find_city(&app->graph, city);
        if (idx == -1) {
            printf("Kota '%s' tidak ditemukan di jaringan.\n", city);
        } else if (choice == 2) {
            graph_print_bfs(&app->graph, idx);
        } else {
            graph_print_dfs(&app->graph, idx);
        }
    } else {
        char asal[MAX_CITY_NAME], tujuan[MAX_CITY_NAME];
        int u, v;
        graph_print_cities(&app->graph);
        read_text("Kota Asal: ", asal, sizeof(asal));
        read_text("Kota Tujuan: ", tujuan, sizeof(tujuan));
        u = graph_find_city(&app->graph, asal);
        v = graph_find_city(&app->graph, tujuan);
        if (u == -1 || v == -1) {
            printf("Salah satu atau kedua kota tidak ditemukan.\n");
        } else {
            graph_print_shortest_transit(&app->graph, u, v);
        }
    }
}

static void handle_bst_menu(App *app)
{
    int choice;

    print_title("INDEKS RUTE (BINARY SEARCH TREE)");
    printf("Tinggi BST saat ini: %d\n", bst_height(app->routeTree));
    printf("1. Traversal Pre-Order\n");
    printf("2. Traversal In-Order (Terurut ID)\n");
    printf("3. Traversal Post-Order\n");
    choice = read_int("Pilihan (1-3): ", 1, 3);

    route_print_header();
    if (choice == 1) {
        bst_preorder(app->routeTree);
    } else if (choice == 2) {
        bst_inorder(app->routeTree);
    } else {
        bst_postorder(app->routeTree);
    }
    print_line();
}

static void handle_admin_add_route(App *app)
{
    char asal[MAX_CITY_NAME], tujuan[MAX_CITY_NAME], jenis[MAX_TYPE_NAME];
    int harga, jarak, jam, menit, kursi, id;
    Route *r;

    print_title("TAMBAH RUTE BARU (ADMIN)");
    read_text("Kota Asal: ", asal, sizeof(asal));
    read_text("Kota Tujuan: ", tujuan, sizeof(tujuan));
    read_text("Jenis Transportasi (Bus/Kereta/Travel): ", jenis, sizeof(jenis));
    harga = read_int("Harga per kursi (Rp): ", 1000, MAX_PRICE);
    jarak = read_int("Jarak tempuh (km): ", 1, MAX_DISTANCE);
    jam = read_int("Jam berangkat (0-23): ", 0, 23);
    menit = read_int("Menit berangkat (0-59): ", 0, 59);
    kursi = read_int("Jumlah kursi tersedia: ", 1, MAX_SEATS);

    id = route_next_id(&app->routes);
    r = route_create(id, asal, tujuan, jenis, harga, jarak, jam * 60 + menit, kursi);
    if (r != NULL && route_list_add(&app->routes, r)) {
        app->routeTree = bst_insert(app->routeTree, r);
        graph_build_from_routes(&app->graph, &app->routes);
        app_save(app);
        printf("Rute baru berhasil ditambahkan dengan ID %d.\n", id);
    } else {
        free(r);
        printf("Gagal menambahkan rute.\n");
    }
}

static void handle_admin_delete_route(App *app)
{
    int id;

    print_title("HAPUS RUTE (ADMIN)");
    route_print_array(app->routes.items, app->routes.count);
    id = read_int("Masukkan ID Rute yang akan dihapus (0 batal): ", 0, MAX_ID);
    if (id == 0) {
        return;
    }
    if (bst_search(app->routeTree, id) == NULL) {
        printf("ID Rute tidak ditemukan.\n");
        return;
    }
    service_remove_route(app, id);
    printf("Rute ID %d berhasil dihapus.\n", id);
}

static void handle_admin_view_waiting(const App *app)
{
    QueueNode *curr;

    print_title("DAFTAR ANTREAN TUNGGU (QUEUE)");
    if (queue_is_empty(&app->waiting)) {
        printf("Antrean tunggu kosong.\n");
        return;
    }
    print_line();
    printf("%-5s %-20s %-10s %s\n", "No", "Username", "ID Rute", "Jumlah Tiket");
    print_line();
    int no = 1;
    for (curr = app->waiting.front; curr != NULL; curr = curr->next) {
        printf("%-5d %-20s %-10d %d\n",
               no++, curr->data.username, curr->data.routeId, curr->data.jumlah);
    }
    print_line();
}

void menu_run(App *app)
{
    int choice;

    while (1) {
        if (app->current == NULL) {
            print_title("SISTEM TIKET TRANSPORTASI ANTARKOTA");
            printf("1. Login\n");
            printf("2. Register Akun Baru\n");
            printf("0. Keluar\n");
            choice = read_int("Pilihan: ", 0, 2);

            if (choice == 1) {
                app->current = auth_login(&app->users);
                if (app->current != NULL) {
                    stack_free(&app->undoStack);
                }
            } else if (choice == 2) {
                auth_register(&app->users);
            } else {
                printf("Terima kasih telah menggunakan aplikasi.\n");
                break;
            }
        } else if (app->current->role == ROLE_USER) {
            print_title("MENU PENUMPANG");
            printf("Login sebagai: %s\n", app->current->username);
            printf("1. Lihat & Urutkan Jadwal Rute (Sorting)\n");
            printf("2. Cari Rute (Sequential / Binary Search / BST)\n");
            printf("3. Pesan Tiket (atau Masuk Waiting List)\n");
            printf("4. Riwayat Pemesanan Saya\n");
            printf("5. Batalkan Pemesanan / Undo Terakhir (Stack)\n");
            printf("6. Jaringan Kota & Rute Transit (Graph BFS/DFS)\n");
            printf("7. Lihat Indeks Pohon Rute (BST Traversal)\n");
            printf("8. Logout\n");
            choice = read_int("Pilihan: ", 1, 8);

            switch (choice) {
            case 1: handle_view_sorted_routes(app); break;
            case 2: handle_search_routes(app); break;
            case 3: handle_book_ticket(app); break;
            case 4: handle_my_bookings(app); break;
            case 5: handle_cancel_or_undo(app); break;
            case 6: handle_graph_menu(app); break;
            case 7: handle_bst_menu(app); break;
            case 8:
                stack_free(&app->undoStack);
                app->current = NULL;
                printf("Anda telah logout.\n");
                break;
            }
        } else {
            print_title("MENU ADMINISTRATOR");
            printf("Login sebagai: %s (ADMIN)\n", app->current->username);
            printf("1. Lihat & Urutkan Semua Rute\n");
            printf("2. Tambah Rute Baru\n");
            printf("3. Hapus Rute\n");
            printf("4. Lihat Antrean Tunggu / Waiting List (Queue)\n");
            printf("5. Statistik Hash Table Akun\n");
            printf("6. Traversal Indeks BST Rute\n");
            printf("7. Jaringan Kota (Graph BFS/DFS)\n");
            printf("8. Logout\n");
            choice = read_int("Pilihan: ", 1, 8);

            switch (choice) {
            case 1: handle_view_sorted_routes(app); break;
            case 2: handle_admin_add_route(app); break;
            case 3: handle_admin_delete_route(app); break;
            case 4: handle_admin_view_waiting(app); break;
            case 5:
                print_title("STATISTIK HASH TABLE AKUN");
                table_print_stats(&app->users);
                break;
            case 6: handle_bst_menu(app); break;
            case 7: handle_graph_menu(app); break;
            case 8:
                app->current = NULL;
                printf("Admin telah logout.\n");
                break;
            }
        }
    }
}
